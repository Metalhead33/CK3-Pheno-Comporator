#ifndef MHBUFFER_HPP
#define MHBUFFER_HPP
#include <cstddef>
#include <cstdint>
#include <vector>
#include <array>
namespace MH33 {
namespace Util {
typedef std::vector<std::byte> Buffer;
template <std::size_t size> using StaticBuffer = std::array<std::byte,size>;
}
}
#endif // MHBUFFER_HPP
