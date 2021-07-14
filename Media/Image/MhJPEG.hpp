#ifndef MHJPEG_HPP
#define MHJPEG_HPP
#include <Io/MhIoDevice.hpp>
#include "MhDecodeTarget.hpp"
namespace MH33 {
namespace GFX {

namespace JPEG
{
bool decode(Util::Buffer& sourceBuff, DecodeTarget& destination);
bool decode(Io::Device& input, DecodeTarget& destination);
void decode(Io::System& iosys, const char* path, DecodeTarget &destination);
void decode(Io::System& iosys, const std::string& path, DecodeTarget &destination);
bool encode(const Util::Buffer& sourceBuff, int width, int height, int pixelFormat,
			Util::Buffer& destinationBuffer, unsigned long &jpegSize, int jpegSubsamp, float jpegQual);
bool encode(const Util::Buffer& sourceBuff, int width, int height, int pixelFormat,
			Io::Device& destination, int jpegSubsamp, float jpegQual);
};

}
}
#endif // MHJPEG_HPP
