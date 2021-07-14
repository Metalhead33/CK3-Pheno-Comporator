#ifndef GIF_HPP
#define GIF_HPP
#include <Io/MhIoDevice.hpp>
#include "MhDecodeTarget.hpp"

namespace MH33 {
namespace GFX {
namespace GIF {

void decode(Io::Device& iodev, DecodeTarget& target);
void decode(Io::System& iosys, const char* path, DecodeTarget &destination);
void decode(Io::System& iosys, const std::string& path, DecodeTarget &destination);

}
}
}
#endif // GIF_HPP
