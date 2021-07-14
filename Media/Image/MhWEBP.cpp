#include "MhWEBP.hpp"
#include <webp/decode.h>
#include <webp/encode.h>
#include <webp/demux.h>
#include <cmath>
#include <limits>
#include <cstring>
#include <memory>
#include <stdexcept>
namespace MH33 {
namespace GFX {
namespace WEBP {

bool decode(const Util::Buffer &srcBuffer, DecodeTarget &target)
{
	if(srcBuffer.empty()) return false;
	int width,height;
	if(!WebPGetInfo(reinterpret_cast<const uint8_t*>(srcBuffer.data()),srcBuffer.size(),&width,&height)) return false;
	int stride = width*byteSize(target.format);
	target.isAnimated = false;
	target.frames.resize(1);
	target.frames[0].imageData.resize(stride*height);
	switch (target.format) {
	case Format::BGR8U:
		return WebPDecodeBGRInto(reinterpret_cast<const uint8_t*>(srcBuffer.data()),srcBuffer.size(),
								 reinterpret_cast<uint8_t*>(target.frames[0].imageData.data()),target.frames[0].imageData.size(),stride) != nullptr;
	case Format::BGRA8U:
		return WebPDecodeBGRAInto(reinterpret_cast<const uint8_t*>(srcBuffer.data()),srcBuffer.size(),
								  reinterpret_cast<uint8_t*>(target.frames[0].imageData.data()),target.frames[0].imageData.size(),stride) != nullptr;
	case Format::RGB8U:
		return WebPDecodeRGBInto(reinterpret_cast<const uint8_t*>(srcBuffer.data()),srcBuffer.size(),
								 reinterpret_cast<uint8_t*>(target.frames[0].imageData.data()),target.frames[0].imageData.size(),stride) != nullptr;
	case Format::RGBA8U:
		return WebPDecodeRGBAInto(reinterpret_cast<const uint8_t*>(srcBuffer.data()),srcBuffer.size(),
								  reinterpret_cast<uint8_t*>(target.frames[0].imageData.data()),target.frames[0].imageData.size(),stride) != nullptr;
	case Format::ARGB8U:
		return WebPDecodeARGBInto(reinterpret_cast<const uint8_t*>(srcBuffer.data()),srcBuffer.size(),
								  reinterpret_cast<uint8_t*>(target.frames[0].imageData.data()),target.frames[0].imageData.size(),stride) != nullptr;
	default:
		target.format = Format::RGBA8U;
		return WebPDecodeRGBAInto(reinterpret_cast<const uint8_t*>(srcBuffer.data()),srcBuffer.size(),
								  reinterpret_cast<uint8_t*>(target.frames[0].imageData.data()),target.frames[0].imageData.size(),stride) != nullptr;
	}
}

bool decode(Io::Device &iodev, DecodeTarget &target)
{
	auto buff = iodev.readAll();
	return decode(buff,target);
}
size_t encode(const uint8_t* pixdat, int width, int height, int stride, Format format, float compressionLevel, uint8_t** ptr)
{
	if(compressionLevel <= std::numeric_limits<float>::epsilon()) { // Lossless
		switch (format) {
		case Format::BGR8U:
			return WebPEncodeLosslessBGR(pixdat,width,height,stride,ptr);
		case Format::BGRA8U:
			return WebPEncodeLosslessBGRA(pixdat,width,height,stride,ptr);
		case Format::RGB8U:
			return WebPEncodeLosslessRGB(pixdat,width,height,stride,ptr);
		case Format::RGBA8U:
			return WebPEncodeLosslessRGBA(pixdat,width,height,stride,ptr);
		default:
			return 0;
		}
	} else {
		const float quality_factor = (1.0f-std::clamp(compressionLevel,0.0f,1.0f))*100.f;
		switch (format) {
		case Format::BGR8U:
			return WebPEncodeBGR(pixdat,width,height,stride,quality_factor,ptr);
		case Format::BGRA8U:
			return WebPEncodeBGRA(pixdat,width,height,stride,quality_factor,ptr);
		case Format::RGB8U:
			return WebPEncodeRGB(pixdat,width,height,stride,quality_factor,ptr);
		case Format::RGBA8U:
			return WebPEncodeRGBA(pixdat,width,height,stride,quality_factor,ptr);
		default:
			return 0;
		}
	}
}
bool encode(const Util::Buffer &srcBuffer, int width, int height, int stride, Format format, float compressionLevel, Util::Buffer &dstBuffer)
{
	if(srcBuffer.empty()) return false;
	uint8_t* out;
	auto encoded = encode(reinterpret_cast<const uint8_t*>(srcBuffer.data()),width,height,stride,format,compressionLevel,&out);
	if(!encoded) return false;
	dstBuffer.resize(encoded);
	std::memcpy(dstBuffer.data(),out,encoded);
	WebPFree(out);
	return true;
}

bool encode(const Util::Buffer &srcBuffer, int width, int height, int stride, Format format, float compressionLevel, Io::Device &dst)
{
	if(srcBuffer.empty()) return false;
	uint8_t* out;
	auto encoded = encode(reinterpret_cast<const uint8_t*>(srcBuffer.data()),width,height,stride,format,compressionLevel,&out);
	if(!encoded) return false;
	dst.write(out,encoded);
	WebPFree(out);
	return true;
}

bool demux(const Util::Buffer &srcBuffer, DecodeTarget &target)
{
	if(srcBuffer.empty()) return false;
	typedef std::unique_ptr<WebPDemuxer,decltype (&WebPDemuxDelete)> Configuration;
	WebPData riff;
	riff.bytes = reinterpret_cast<const uint8_t*>(srcBuffer.data());
	riff.size = srcBuffer.size();
	Configuration config = Configuration(WebPDemux(&riff),WebPDemuxDelete);
	if(!config) return false;
	uint32_t width = WebPDemuxGetI(config.get(), WEBP_FF_CANVAS_WIDTH);
	uint32_t height = WebPDemuxGetI(config.get(), WEBP_FF_CANVAS_HEIGHT);
	//uint32_t flags = WebPDemuxGetI(config.get(), WEBP_FF_FORMAT_FLAGS);
	//uint32_t frameCount = WebPDemuxGetI(config.get(), WEBP_FF_FRAME_COUNT);
	WebPIterator iter;
	if (WebPDemuxGetFrame(config.get(), 1, &iter)) {
		do {
			int stride = width*byteSize(target.format);
			int targetSize = stride*height;
			target.frames.push_back(Frame());
			target.frames.back().imageData.resize(targetSize);
			target.frames.back().width = width;
			target.frames.back().height = height;
			target.frames.back().stride = stride;
			uint8_t* data = nullptr;
			switch (target.format) {
			/*case Format::BGR8U:
				data = WebPDecodeBGR(iter.fragment.bytes,iter.fragment.size,nullptr,nullptr);
				break;*/
			case Format::BGR8U:
				WebPDecodeBGRInto(iter.fragment.bytes,iter.fragment.size,
								reinterpret_cast<uint8_t*>(target.frames.back().imageData.data()),target.frames.back().imageData.size(),stride);
				break;
			case Format::BGRA8U:
				WebPDecodeBGRAInto(iter.fragment.bytes,iter.fragment.size,
								   reinterpret_cast<uint8_t*>(target.frames.back().imageData.data()),target.frames.back().imageData.size(),stride);
				break;
			case Format::RGB8U:
				WebPDecodeRGBInto(iter.fragment.bytes,iter.fragment.size,
								  reinterpret_cast<uint8_t*>(target.frames.back().imageData.data()),target.frames.back().imageData.size(),stride);
				break;
			case Format::RGBA8U:
				WebPDecodeRGBAInto(iter.fragment.bytes,iter.fragment.size,
								   reinterpret_cast<uint8_t*>(target.frames.back().imageData.data()),target.frames.back().imageData.size(),stride);
				break;
			case Format::ARGB8U:
				WebPDecodeARGBInto(iter.fragment.bytes,iter.fragment.size,
								   reinterpret_cast<uint8_t*>(target.frames.back().imageData.data()),target.frames.back().imageData.size(),stride);
				break;
			default:
				target.format = Format::RGB8U;
				stride = width*byteSize(target.format);
				target.frames.back().stride = stride;
				targetSize = width*stride;
				target.frames.back().imageData.resize(targetSize);
				WebPDecodeRGBInto(iter.fragment.bytes,iter.fragment.size,
								  reinterpret_cast<uint8_t*>(target.frames.back().imageData.data()),target.frames.back().imageData.size(),stride);
				break;
			}
		} while (WebPDemuxNextFrame(&iter));
		WebPDemuxReleaseIterator(&iter);
	}
	if(target.frames.size() > 1) target.isAnimated = true;
	else target.isAnimated = false;
	return true;
}

bool demux(Io::Device &iodev, DecodeTarget &target)
{
	auto buff = iodev.readAll();
	return demux(buff,target);
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
void demux(Io::System &iosys, const char *path, DecodeTarget &destination)
{
	std::unique_ptr<Io::Device> iodev(iosys.open(path,Io::Mode::READ));
	if(iodev) {
		demux(*iodev,destination);
	}
}

void demux(Io::System &iosys, const std::string &path, DecodeTarget &destination)
{
	std::unique_ptr<Io::Device> iodev(iosys.open(path,Io::Mode::READ));
	if(iodev) {
		demux(*iodev,destination);
	}
}


}
}
}
