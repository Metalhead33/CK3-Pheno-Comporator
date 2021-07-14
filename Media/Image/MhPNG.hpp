#ifndef MHPNG_HPP
#define MHPNG_HPP
#include <Io/MhIoDevice.hpp>
#include "MhDecodeTarget.hpp"
#include <memory>

namespace MH33 {
namespace GFX {
namespace PNG
{
	void decode(Io::Device& iodev, DecodeTarget &destination);
	void decode(Io::System& iosys, const char* path, DecodeTarget &destination);
	void decode(Io::System& iosys, const std::string& path, DecodeTarget &destination);
	void encode(Io::Device& iodev, uint16_t width, uint16_t height, uint8_t color_type, uint8_t bit_depth, Util::Buffer &pixelData, float compressionLevel);
};

}
}

#endif // MHPNG_HPP
