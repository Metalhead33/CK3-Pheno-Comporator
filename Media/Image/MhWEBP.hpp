#ifndef MHWEBP_HPP
#define MHWEBP_HPP
#include <Io/MhIoDevice.hpp>
#include "MhDecodeTarget.hpp"

namespace MH33 {
namespace GFX {
namespace WEBP
{
bool decode(const Util::Buffer& srcBuffer, DecodeTarget& target);
bool decode(Io::Device& iodev, DecodeTarget& target);
void decode(Io::System& iosys, const char* path, DecodeTarget &destination);
void decode(Io::System& iosys, const std::string& path, DecodeTarget &destination);
bool encode(const Util::Buffer& srcBuffer, int width, int height, int stride, Format format, float compressionLevel, Util::Buffer& dstBuffer);
bool encode(const Util::Buffer& srcBuffer, int width, int height, int stride, Format format, float compressionLevel, Io::Device& dst);

bool demux(const Util::Buffer& srcBuffer, DecodeTarget& target);
bool demux(Io::Device& iodev, DecodeTarget& target);
void demux(Io::System& iosys, const char* path, DecodeTarget &destination);
void demux(Io::System& iosys, const std::string& path, DecodeTarget &destination);

}
}
}
#endif // MHWEBP_HPP
