#include "MhDDS.hpp"
#include <Io/MhBufferWrapper.hpp>
namespace MH33 {
namespace GFX {
namespace DDS {
static const uint32_t FOURCC_DXT1 = 0x31545844;
static const uint32_t FOURCC_DXT3 = 0x33545844;
static const uint32_t FOURCC_DXT5 = 0x35545844;
static const uint32_t FOURCC_DX10 = 0x30315844;

enum Type {
	INVALID = 0,
	DXT1,
	DXT3,
	DXT5,
	DX10
};

struct Header
{
	// We read this from the file
	uint32_t dwSize;
	uint32_t dwFlags;
	uint32_t dwHeight;
	uint32_t dwWidth;
	uint32_t dwPitchOrLinearSize;
	uint32_t dwDepth;
	uint32_t dwMipMapCount;
	std::array<uint32_t,11> dwReserved1;
	struct {
		uint32_t dwSize;
		uint32_t dwFlags;
		uint32_t dwFourCC;
		uint32_t dwRGBBitCount;
		uint32_t dwRBitMask;
		uint32_t dwGBitMask;
		uint32_t dwBBitMask;
		uint32_t dwABitMask;
	} ddspf;
	uint32_t dwCaps;
	uint32_t dwCaps2;
	uint32_t dwCaps3;
	uint32_t dwCaps4;
	uint32_t dwReserved2;
	// This part is optional
	struct {
		uint32_t dxgiFormat;
		uint32_t resourceDimension;
		uint32_t miscFlag;
		uint32_t arraySize;
		uint32_t miscFlags2;
	} headerDXT10;
	// We infer this from the file
	Type type;
	struct Mipmap {
		uint32_t width, height;
		std::vector<std::byte> bytes;
	};
	std::vector<Mipmap> mipmaps;
	void load(Io::Device& input);
};

void Header::load(Io::Device &input)
{
	input.seek(Io::SeekOrigin::SET,4);
	Io::DataStream<Util::Endian::Little> ddsInput(input); // Implying we already read the magic word at the beginning.
	ddsInput >> dwSize;
	if(dwSize != 124) {
		this->type = Type::INVALID;
		return;
	}
	ddsInput >> dwFlags;
	ddsInput >> dwHeight;
	ddsInput >> dwWidth;
	ddsInput >> dwPitchOrLinearSize;
	ddsInput >> dwDepth;
	ddsInput >> dwMipMapCount;
	ddsInput >> dwReserved1;
	ddsInput >> ddspf.dwSize;
	ddsInput >> ddspf.dwFlags;
	ddsInput >> ddspf.dwFourCC;
	ddsInput >> ddspf.dwRGBBitCount;
	ddsInput >> ddspf.dwRBitMask;
	ddsInput >> ddspf.dwGBitMask;
	ddsInput >> ddspf.dwBBitMask;
	ddsInput >> ddspf.dwABitMask;
	ddsInput >> dwCaps;
	ddsInput >> dwCaps2;
	ddsInput >> dwCaps3;
	ddsInput >> dwCaps4;
	ddsInput >> dwReserved2;
	switch(ddspf.dwFourCC)
	{
	case FOURCC_DXT1:
		this->type = Type::DXT1;
		break;
	case FOURCC_DXT3:
		this->type = Type::DXT3;
		break;
	case FOURCC_DXT5:
		this->type = Type::DXT5;
		break;
	case FOURCC_DX10:
		this->type = Type::DX10;
		ddsInput >> headerDXT10.dxgiFormat;
		ddsInput >> headerDXT10.resourceDimension;
		ddsInput >> headerDXT10.miscFlag;
		ddsInput >> headerDXT10.arraySize;
		ddsInput >> headerDXT10.miscFlags2;
		break;
	default:
		this->type = Type::INVALID;
		return;
	}
	uint32_t width = dwWidth;
	uint32_t height = dwHeight;
	const uint32_t buffsize = dwMipMapCount > 1 ? dwPitchOrLinearSize * 2 : dwPitchOrLinearSize;
	std::vector<std::byte> buffer(buffsize);
	ddsInput.read(buffer.data(),buffsize);
	const uint32_t blockSize = (this->type == Type::DXT1) ? 8 : 16;
	uint32_t offset = 0;
	mipmaps.resize(dwMipMapCount);
	for(Mipmap& x : mipmaps) {
		const unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize;
		x.width = width;
		x.height = height;
		x.bytes.resize(size);
		memcpy(x.bytes.data(),buffer.data() + offset,size);
		offset += size;
		width  /= 2;
		height /= 2;
	}
}

const unsigned FI16_444_RED_MASK = 0x0F00;
const unsigned FI16_444_GREEN_MASK = 0x00F0;
const unsigned FI16_444_BLUE_MASK = 0x000F;
const unsigned FI16_444_RED_SHIFT	= 8;
const unsigned FI16_444_GREEN_SHIFT = 4;
const unsigned FI16_444_BLUE_SHIFT	= 0;
const unsigned FI16_555_RED_MASK = 0x7C00;
const unsigned FI16_555_GREEN_MASK	= 0x03E0;
const unsigned FI16_555_BLUE_MASK = 0x001F;
const unsigned FI16_555_RED_SHIFT = 10;
const unsigned FI16_555_GREEN_SHIFT = 5;
const unsigned FI16_555_BLUE_SHIFT = 0;
const unsigned FI16_565_RED_MASK = 0xF800;
const unsigned FI16_565_GREEN_MASK = 0x07E0;
const unsigned FI16_565_BLUE_MASK = 0x001F;
const unsigned FI16_565_RED_SHIFT = 11;
const unsigned FI16_565_GREEN_SHIFT = 5;
const unsigned FI16_565_BLUE_SHIFT = 0;

void decode(Io::Device &iodev, DecodeTarget &destination)
{
	Header head;
	head.load(iodev);
	switch (head.type) {
	case Type::DXT1:
		destination.format = Format::B_DXT1;
		if ((head.ddspf.dwRBitMask == FI16_444_RED_MASK) && (head.ddspf.dwGBitMask == FI16_444_GREEN_MASK) && (head.ddspf.dwBBitMask == FI16_444_BLUE_MASK)) {
			destination.format = Format::DXT1_RGB444;
		}
		if ((head.ddspf.dwRBitMask == FI16_555_RED_MASK) && (head.ddspf.dwGBitMask == FI16_555_GREEN_MASK) && (head.ddspf.dwBBitMask == FI16_555_BLUE_MASK)) {
			destination.format = Format::DXT1_RGB555;
		}
		if ((head.ddspf.dwRBitMask == FI16_565_RED_MASK) && (head.ddspf.dwGBitMask == FI16_565_GREEN_MASK) && (head.ddspf.dwBBitMask == FI16_565_BLUE_MASK)) {
			destination.format = Format::DXT1_RGB565;
		}
		break;
	case Type::DXT3:
		destination.format = Format::B_DXT3;
		if ((head.ddspf.dwRBitMask == FI16_444_RED_MASK) && (head.ddspf.dwGBitMask == FI16_444_GREEN_MASK) && (head.ddspf.dwBBitMask == FI16_444_BLUE_MASK)) {
			destination.format = Format::DXT3_RGB444;
		}
		if ((head.ddspf.dwRBitMask == FI16_555_RED_MASK) && (head.ddspf.dwGBitMask == FI16_555_GREEN_MASK) && (head.ddspf.dwBBitMask == FI16_555_BLUE_MASK)) {
			destination.format = Format::DXT3_RGB555;
		}
		if ((head.ddspf.dwRBitMask == FI16_565_RED_MASK) && (head.ddspf.dwGBitMask == FI16_565_GREEN_MASK) && (head.ddspf.dwBBitMask == FI16_565_BLUE_MASK)) {
			destination.format = Format::DXT3_RGB565;
		}
		break;
	case Type::DXT5:
		destination.format = Format::B_DXT5;
		if ((head.ddspf.dwRBitMask == FI16_444_RED_MASK) && (head.ddspf.dwGBitMask == FI16_444_GREEN_MASK) && (head.ddspf.dwBBitMask == FI16_444_BLUE_MASK)) {
			destination.format = Format::DXT5_RGB444;
		}
		if ((head.ddspf.dwRBitMask == FI16_555_RED_MASK) && (head.ddspf.dwGBitMask == FI16_555_GREEN_MASK) && (head.ddspf.dwBBitMask == FI16_555_BLUE_MASK)) {
			destination.format = Format::DXT5_RGB555;
		}
		if ((head.ddspf.dwRBitMask == FI16_565_RED_MASK) && (head.ddspf.dwGBitMask == FI16_565_GREEN_MASK) && (head.ddspf.dwBBitMask == FI16_565_BLUE_MASK)) {
			destination.format = Format::DXT5_RGB565;
		}
		break;
	default:
		destination.format = Format::INVALID; return;
	}
	destination.isAnimated = false;
	for(auto& it : head.mipmaps) {
		destination.frames.push_back(Frame());
		destination.frames.back().width = it.width;
		destination.frames.back().height = it.height;
		destination.frames.back().stride = 0;
		destination.frames.back().imageData = std::move(it.bytes);
	}
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
