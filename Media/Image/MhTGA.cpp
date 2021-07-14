#include "MhTGA.hpp"

namespace MH33 {
namespace GFX {
namespace TGA {

enum class Type {
	INVALID = 0x00,
	RGB = 0x01,
	GREYSCALE = 0x02,
	PALETTIZED = 0x03
};

struct Header {
public:
	// Read information - straight from the file
	uint8_t idLen;
	uint8_t colmapType;
	uint8_t imageType;
	struct {
		uint16_t firstEntryIndex;
		uint16_t colorMapLength;
		uint8_t colorMapEntrySize;
	} colorMapSpecification;
	struct {
		uint16_t xOrigin;
		uint16_t yOrigin;
		uint16_t width;
		uint16_t height;
		uint8_t pixelDepth;
		uint8_t imageDescriptor;
	} imageSpecification;
	int32_t extensionOffset;
	int32_t developerAreaOffset;
	struct ExtensionInformation {
		uint16_t extensionSize;
		std::array<char,41> authorName;
		std::array<char,324> authorComment;
		std::array<uint16_t,6> timestamp;
		std::array<char,41> jobId;
		std::array<uint16_t,3> jobTime;
		std::array<char,41> softwareID;
		std::array<char,3> softwareVersion;
		uint32_t keyColour;
		std::array<uint16_t,2> pixelAspectRatio;
		std::array<uint16_t,2> gammaValue;
		uint32_t colourCorrectionOffset;
		uint32_t postageStampOffset;
		uint32_t scanlineOffset;
		uint8_t attributeType;
		void load(Io::DataStream<Util::Endian::Little>& input);
	};
	// Deduced information - deduced from read info
	int version;
	Type format;
	std::vector<std::byte> colorMap;
	std::vector<std::byte> imageData;
	// Commands
	void load(Io::Device& input);
private:
	void decodeImage(size_t imageSize, Io::Device& input);
	void decodeCompressedImage(size_t imageSize, Io::Device& input);
	void flipVert();
	void flipHoriz();
};

void Header::load(Io::Device &input)
{
	Io::DataStream<Util::Endian::Little> tgaInput(input); // TGA is little-endian.

	auto currPos = tgaInput.tell();
	tgaInput.seek(Io::SeekOrigin::END,26);
	tgaInput >> extensionOffset;
	tgaInput >> developerAreaOffset;
	char versionCheck[17] = "";
	tgaInput.read(versionCheck,16);
	version = 1;
	if ( strcmp( versionCheck, "TRUEVISION-XFILE" ) == 0 )
		version = 2;
	else {
		extensionOffset = -1;
		developerAreaOffset = -1;
	}
	tgaInput.seek(Io::SeekOrigin::SET,currPos);

	tgaInput >> idLen;
	tgaInput >> colmapType;
	tgaInput >> imageType;
	tgaInput >> colorMapSpecification.firstEntryIndex;
	tgaInput >> colorMapSpecification.colorMapLength;
	tgaInput >> colorMapSpecification.colorMapEntrySize;
	tgaInput >> imageSpecification.xOrigin;
	tgaInput >> imageSpecification.yOrigin;
	tgaInput >> imageSpecification.width;
	tgaInput >> imageSpecification.height;
	tgaInput >> imageSpecification.pixelDepth;
	tgaInput >> imageSpecification.imageDescriptor;
	bool isCompressed = false;
	switch (imageType) {
	case 0:
		format = Type::INVALID;
		break;
	case 1:
		format = Type::PALETTIZED;
		break;
	case 2:
		format = Type::RGB;
		break;
	case 3:
		format = Type::GREYSCALE;
		break;
	case 9:
		format = Type::PALETTIZED;
		isCompressed = true;
		break;
	case 10:
		format = Type::RGB;
		isCompressed = true;
		break;
	case 11:
		format = Type::GREYSCALE;
		isCompressed = true;
		break;
	default:
		format = Type::INVALID;
		break;
	}
	tgaInput.seek(Io::SeekOrigin::CUR,idLen); // Skip ID.
	if(colmapType) {
		size_t colMapSize = colorMapSpecification.colorMapLength * (colorMapSpecification.colorMapEntrySize / 8);
		colorMap.resize(colMapSize);
		tgaInput.read(colorMap.data(),colMapSize);
	}
	size_t imageSize = imageSpecification.width * imageSpecification.height * (imageSpecification.pixelDepth / 8);
	if(isCompressed) decodeCompressedImage(imageSize,tgaInput);
	else decodeImage(imageSize, tgaInput);
	const bool fliphoriz = (imageSpecification.imageDescriptor & 0x10) ? true : false;
	const bool flipvert = (imageSpecification.imageDescriptor & 0x20) ? false : true;
	if(flipvert) flipVert();
	if(fliphoriz) flipHoriz();
}

void Header::decodeImage(size_t imageSize, Io::Device& input)
{
	imageData.resize(imageSize);
	input.read(imageData.data(),imageSize); // Will read past the end of file in case of RLE.
}

void Header::decodeCompressedImage(size_t imageSize, Io::Device &input)
{
	imageData.resize(imageSize);
	input.read(imageData.data(),imageSize); // Will read past the end of file in case of RLE.
	Util::Buffer tempImageData = imageData;
	int indexAccum = 0;
	int bytesPerPixel = ( imageSpecification.pixelDepth / 8 );
	int bytesPerPixelRLE = bytesPerPixel + 1;
	for ( int i = 0; indexAccum < imageSize; ) {
		int runCount = ( 127 & reinterpret_cast<uint8_t&>(tempImageData[i]) ) + 1;
		// Compressed data
		if ( 128 & reinterpret_cast<uint8_t&>(tempImageData[i]) ) {
			for ( int j = 0; j < runCount; j++ ) {
				for ( int k = 1; k < bytesPerPixelRLE; k++ ) {
					imageData[indexAccum++] = tempImageData[i + k];
				}
			}
			i += bytesPerPixelRLE;
		}
		// Raw data?
		else if ( !( 128 & reinterpret_cast<uint8_t&>(tempImageData[i]) ) ) {
			i++;
			for ( int j = 0; j < runCount; j++ ) {
				for ( int k = 0; k < bytesPerPixel; k++ ) {
					imageData[indexAccum++] = tempImageData[i + k];
				}
				i += bytesPerPixel;
			}
		}
	}
}

void Header::flipVert()
{
	const uint32_t stride = imageSpecification.width * (imageSpecification.pixelDepth / 8);
	Util::Buffer reverseImage(imageData.size());
	for(uint32_t i = 0; i < imageSpecification.height; ++i) {
		memcpy(&reverseImage[stride*(imageSpecification.height-(i+1))], &imageData[i*stride], stride);
	}
	imageData = std::move(reverseImage);
	/*for(uint32_t i = 0; i < imageSpecification.height; ++i) {
		Buffer tmpbuffA(stride);
		Buffer tmpbuffB(stride);
		std::byte* ptrA = &imageData[stride*i];
		std::byte* ptrB = &imageData[stride*(imageSpecification.height-(i+1))];
		memcpy(tmpbuffA.data(),ptrA,stride);
		memcpy(tmpbuffB.data(),ptrB,stride);
		memcpy(ptrA,tmpbuffB.data(),stride);
		memcpy(ptrB,tmpbuffA.data(),stride);
	}*/
}

void Header::flipHoriz()
{
	const uint32_t stride = imageSpecification.width * (imageSpecification.pixelDepth / 8);
	for(uint32_t i = 0; i < imageSpecification.height; ++i) {
		std::byte* ptrA = &imageData[stride*i];
		switch (imageSpecification.pixelDepth) {
		case 8:
		{
			std::span<std::byte> spn(ptrA,stride);
			std::reverse(std::begin(spn),std::end(spn));
			break;
		}
		case 15:
		case 16:
		{
			std::span<uint16_t> spn(reinterpret_cast<uint16_t*>(ptrA),stride/2);
			std::reverse(std::begin(spn),std::end(spn));
			break;
		}
		case 24:
		{
			std::span<std::array<uint8_t,3>> spn(reinterpret_cast<std::array<uint8_t,3>*>(ptrA),stride/3);
			std::reverse(std::begin(spn),std::end(spn));
			break;
		}
		case 32:
		{
			std::span<uint32_t> spn(reinterpret_cast<uint32_t*>(ptrA),stride/4);
			std::reverse(std::begin(spn),std::end(spn));
			break;
		}
		default:
			break;
		}
	}
}

void Header::ExtensionInformation::load(Io::DataStream<Util::Endian::Little> &input)
{
	input >> extensionSize;
	input >> authorName;
	input >> authorComment;
	input >> timestamp;
	input >> jobId;
	input >> jobTime;
	input >> softwareID;
	input >> softwareVersion;
	input >> keyColour;
	input >> pixelAspectRatio;
	input >> gammaValue;
	input >> colourCorrectionOffset;
	input >> postageStampOffset;
	input >> scanlineOffset;
	input >> attributeType;
}

void decode(Io::Device &iodev, DecodeTarget &destination)
{
	Header head;
	head.load(iodev);
	switch (head.format) {
		case Type::INVALID: destination.format = Format::INVALID; return;
		case Type::RGB: switch (head.imageSpecification.pixelDepth) {
			case 15: destination.format = Format::RGB555; break;
			case 16: destination.format = Format::RGB565; break;
			case 24: destination.format = Format::BGR8U; break;
			case 32: destination.format = Format::BGRA8U; break;
			default: destination.format = Format::INVALID; return;
			} break;
		case Type::GREYSCALE: switch (head.imageSpecification.pixelDepth) {
			case 8: destination.format = Format::R8U; break;
			case 16: destination.format = Format::R16U; break;
			case 32: destination.format = Format::R32U; break;
			default: destination.format = Format::INVALID; return;
			} break;
		case Type::PALETTIZED: {
			destination.format = Format::INDEXED;
			destination.palette.emplace();
			destination.palette->palette = std::move(head.colorMap);
			switch (head.colorMapSpecification.colorMapEntrySize) {
			case 15: destination.palette->format = Format::RGB555; break;
			case 16: destination.palette->format = Format::RGB565; break;
			case 24: destination.palette->format = Format::BGR8U; break;
			case 32: destination.palette->format = Format::BGRA8U; break;
			default: destination.palette->format = Format::INVALID; return;
			} break;
		}
		default: destination.format = Format::INVALID; return;
	}
	destination.frames.push_back(Frame());
	destination.frames.back().width = head.imageSpecification.width;
	destination.frames.back().height = head.imageSpecification.height;
	destination.frames.back().stride = head.imageSpecification.width * byteSize(destination.format);
	destination.frames.back().imageData = std::move(head.imageData);
}
void decode(Io::System &iosys, const char *path, DecodeTarget &destination)
{
	std::unique_ptr<Io::Device> iodev(iosys.open(path,Io::Mode::READ));
	if(iodev) {
		decode(*iodev,destination);
	}
}

void decode(Io::System &iosys, const std::string &path, DecodeTarget &destination)
{
	std::unique_ptr<Io::Device> iodev(iosys.open(path,Io::Mode::READ));
	if(iodev) {
		decode(*iodev,destination);
	}
}

}
}
}
