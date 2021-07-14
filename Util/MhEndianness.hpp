#ifndef MHENDIANNESS_HPP
#define MHENDIANNESS_HPP
#include <cstdint>
#include <array>
#include <algorithm>
#include "half.hpp"
namespace MH33 {
namespace Util {

enum class Endian
{
#ifdef _WIN32
	Little = 0,
	Big    = 1,
	Native = Little
#else
	Little = __ORDER_LITTLE_ENDIAN__,
	Big    = __ORDER_BIG_ENDIAN__,
	Native = __BYTE_ORDER__
#endif
};

template <typename T> struct _endianness_swp {
	inline static void do_swp(T &val) {
		union U {
			T val;
			std::array<std::uint8_t, sizeof(T)> raw;
		} src, dst;

		src.val = val;
		std::reverse_copy(src.raw.begin(), src.raw.end(), dst.raw.begin());
		val = dst.val;
	}
};
template <> struct _endianness_swp<uint8_t> {
	typedef uint8_t T;
	inline static void do_swp(T &val) {
		(void)val;
	}
};
// Fallbacks for processors that don't have bswap
template <> struct _endianness_swp<uint16_t> {
	typedef uint16_t T;
	inline static void do_swp(T &val) {
		val = (val >> 8) | (val << 8);
	}
};
template <> struct _endianness_swp<uint32_t> {
	typedef uint32_t T;
	inline static void do_swp(T &val) {
		uint32_t tmp = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
		val = (tmp << 16) | (tmp >> 16);
	}
};
template <> struct _endianness_swp<uint64_t> {
	typedef uint64_t T;
	inline static void do_swp(T &val) {
		val = ((val & 0x00000000FFFFFFFFull) << 32) | ((val & 0xFFFFFFFF00000000ull) >> 32);
		val = ((val & 0x0000FFFF0000FFFFull) << 16) | ((val & 0xFFFF0000FFFF0000ull) >> 16);
		val = ((val & 0x00FF00FF00FF00FFull) << 8)  | ((val & 0xFF00FF00FF00FF00ull) >> 8);
	}
};
// Aliases for signed types and others.
template <> struct _endianness_swp<int8_t> {
	typedef int8_t T;
	inline static void do_swp(T &val) {
		_endianness_swp<uint8_t>::do_swp(*reinterpret_cast<uint8_t*>(&val));
	}
};
template <> struct _endianness_swp<int16_t> {
	typedef int16_t T;
	inline static void do_swp(T &val) {
		_endianness_swp<uint16_t>::do_swp(*reinterpret_cast<uint16_t*>(&val));
	}
};
template <> struct _endianness_swp<int32_t> {
	typedef int32_t T;
	inline static void do_swp(T &val) {
		_endianness_swp<uint32_t>::do_swp(*reinterpret_cast<uint32_t*>(&val));
	}
};
template <> struct _endianness_swp<int64_t> {
	typedef int64_t T;
	inline static void do_swp(T &val) {
		_endianness_swp<uint64_t>::do_swp(*reinterpret_cast<uint64_t*>(&val));
	}
};
template <> struct _endianness_swp<half_float::half> {
	typedef half_float::half T;
	inline static void do_swp(T &val) {
		static_assert (sizeof(half_float::half) == sizeof(uint16_t),"Half-float isn't 16-bit! This means that this kind of system isn't supported!");
		_endianness_swp<uint16_t>::do_swp(*reinterpret_cast<uint16_t*>(&val));
	}
};
template <> struct _endianness_swp<float> {
	typedef float T;
	inline static void do_swp(T &val) {
		static_assert (sizeof(float) == sizeof(uint32_t),"Float isn't 32-bit! This means that this kind of system isn't supported!");
		_endianness_swp<uint32_t>::do_swp(*reinterpret_cast<uint32_t*>(&val));
	}
};
template <> struct _endianness_swp<double> {
	typedef double T;
	inline static void do_swp(T &val) {
		static_assert (sizeof(double) == sizeof(uint64_t),"Double isn't 64-bit! This means that this kind of system isn't supported!");
		_endianness_swp<uint64_t>::do_swp(*reinterpret_cast<uint64_t*>(&val));
	}
};

template <Endian src, Endian dst, typename T> void convert_endian(T& val) {
	if constexpr(src == dst) (void)val;
	else _endianness_swp<T>::do_swp(val);
};

}
}
#endif // MHENDIANNESS_HPP
