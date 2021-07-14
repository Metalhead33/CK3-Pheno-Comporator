#include "MhJPEG.hpp"
#include <turbojpeg.h>
#include <memory>

namespace MH33 {
namespace GFX {
namespace JPEG {

bool encode(const Util::Buffer& sourceBuff, int width, int height, int pixelFormat,
	Util::Buffer& destinationBuffer, unsigned long &jpegSize, int jpegSubsamp, float jpegQual)
{
	if(sourceBuff.empty()) return false;
	auto handle = std::unique_ptr<void,decltype(&tjDestroy) >(tjInitCompress(),tjDestroy);
	if(!handle) return false;
	destinationBuffer.resize(tjBufSize(width,height,jpegSubsamp));
	unsigned char* dstptr = reinterpret_cast<unsigned char*>(destinationBuffer.data());
	tjCompress2(handle.get(),reinterpret_cast<const unsigned char*>(sourceBuff.data()),width,width*tjPixelSize[pixelFormat],height,
				pixelFormat,&dstptr,&jpegSize,jpegSubsamp,int(((1.0f-std::clamp(jpegQual,0.0f,1.0f))*99.0f)+1.0f),TJFLAG_NOREALLOC | TJFLAG_FASTDCT);
	return true;
}

bool decode(Util::Buffer& sourceBuff, DecodeTarget &destination)
{
	if(sourceBuff.empty()) { destination.format = Format::INVALID; return false; }
	auto handle = std::unique_ptr<void,decltype(&tjDestroy) >(tjInitDecompress(),tjDestroy);
	int width,height,subsamp;
	tjDecompressHeader2(handle.get(), reinterpret_cast<unsigned char*>(sourceBuff.data()), sourceBuff.size(), &width, &height, &subsamp);
	destination.isAnimated = false;
	destination.format = Format::RGB8U;
	destination.frames.push_back(Frame());
	destination.frames[0].width = width;
	destination.frames[0].height = height;
	destination.frames[0].stride = width * 3;
	destination.frames[0].imageData.resize(width*height*3);
	tjDecompress2(handle.get(),reinterpret_cast<unsigned char*>(sourceBuff.data()),sourceBuff.size(),
				  reinterpret_cast<unsigned char*>(destination.frames[0].imageData.data()),width,width*3,height,TJPF_RGB,TJFLAG_FASTDCT);
	return true;
}

bool decode(Io::Device &input, DecodeTarget &destination)
{
	auto buff = input.readAll();
	return decode(buff,destination);
}

bool encode(const Util::Buffer &sourceBuff, int width, int height, int pixelFormat, Io::Device &destination, int jpegSubsamp, float jpegQual)
{
	Util::Buffer tmpBUff;
	unsigned long jpegSize;
	bool retval = encode(sourceBuff,width,height,pixelFormat,tmpBUff,jpegSize,jpegSubsamp,jpegQual);
	if(retval) destination.write(tmpBUff.data(),jpegSize);
	return retval;
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
