#ifndef MHDECODETARGET_HPP
#define MHDECODETARGET_HPP
#include <Io/MhIoDevice.hpp>
#include <IoSys/MhIoSystem.hpp>
#include "MhStandardColourFormat.hpp"
#include <optional>


namespace MH33 {
namespace GFX {

enum class Format {
	INVALID = 0x0000,
	STANDARD_TYPE_MASK = 0x007F,
	SPECIAL_TYPE_START = 0x0080,
	/// Standard type bases
	COLOR_COMPONENT_MASK = 0x0007,
	COLOR_RESOLUTION_MASK = 0x0078,
	ST_GREYSCALE = 0x0001,
	ST_RG = 0x0002,
	ST_RGB = 0x0003,
	ST_BGR = 0x0004,
	ST_RGBA = 0x0005,
	ST_BGRA = 0x0006,
	ST_ARGB = 0x0007,
	SC_UINT8 = 0x0008,
	SC_INT8 = 0x0010,
	SC_UINT16 = 0x0018,
	SC_INT16 = 0x0020,
	SC_UINT32 = 0x0028,
	SC_INT32 = 0x0030,
	SC_F16 = 0x0038,
	SC_F32 = 0x0040,
	SC_F64 = 0x0048,
	B_DXT1 = 0x50,
	B_DXT3 = 0x58,
	B_DXT5 = 0x60,
	/// Standard types bases
	// Greyscale
	R8U = ST_GREYSCALE | SC_UINT8,
	R8S = ST_GREYSCALE | SC_INT8,
	R16U = ST_GREYSCALE | SC_UINT16,
	R16S = ST_GREYSCALE | SC_INT16,
	R16F = ST_GREYSCALE | SC_F16,
	R32U = ST_GREYSCALE | SC_UINT32,
	R32S = ST_GREYSCALE | SC_INT32,
	R32F = ST_GREYSCALE | SC_F32,
	R64F = ST_GREYSCALE | SC_F64,
	// Two-color
	RG8U = ST_RG | SC_UINT8,
	RG8S = ST_RG | SC_INT8,
	RG16U = ST_RG | SC_UINT16,
	RG16S = ST_RG | SC_INT16,
	RG16F = ST_RG | SC_F16,
	RG32U = ST_RG | SC_UINT32,
	RG32S = ST_RG | SC_INT32,
	RG32F = ST_RG | SC_F32,
	RG64F = ST_RG | SC_F64,
	// RGB
	RGB8U = ST_RGB | SC_UINT8,
	RGB8S = ST_RGB | SC_INT8,
	RGB16U = ST_RGB | SC_UINT16,
	RGB16S = ST_RGB | SC_INT16,
	RGB16F = ST_RGB | SC_F16,
	RGB32U = ST_RGB | SC_UINT32,
	RGB32S = ST_RGB | SC_INT32,
	RGB32F = ST_RGB | SC_F32,
	RGB64F = ST_RGB | SC_F64,
	// BGR
	BGR8U = ST_BGR | SC_UINT8,
	BGR8S = ST_BGR | SC_INT8,
	BGR16U = ST_BGR | SC_UINT16,
	BGR16S = ST_BGR | SC_INT16,
	BGR16F = ST_BGR | SC_F16,
	BGR32U = ST_BGR | SC_UINT32,
	BGR32S = ST_BGR | SC_INT32,
	BGR32F = ST_BGR | SC_F32,
	BGR64F = ST_BGR | SC_F64,
	// RGBA
	RGBA8U = ST_RGBA | SC_UINT8,
	RGBA8S = ST_RGBA | SC_INT8,
	RGBA16U = ST_RGBA | SC_UINT16,
	RGBA16S = ST_RGBA | SC_INT16,
	RGBA16F = ST_RGBA | SC_F16,
	RGBA32U = ST_RGBA | SC_UINT32,
	RGBA32S = ST_RGBA | SC_INT32,
	RGBA32F = ST_RGBA | SC_F32,
	RGBA64F = ST_RGBA | SC_F64,
	// BGRA
	BGRA8U = ST_BGRA | SC_UINT8,
	BGRA8S = ST_BGRA | SC_INT8,
	BGRA16U = ST_BGRA | SC_UINT16,
	BGRA16S = ST_BGRA | SC_INT16,
	BGRA16F = ST_BGRA | SC_F16,
	BGRA32U = ST_BGRA | SC_UINT32,
	BGRA32S = ST_BGRA | SC_INT32,
	BGRA32F = ST_BGRA | SC_F32,
	BGRA64F = ST_BGRA | SC_F64,
	// ARGB
	ARGB8U = ST_ARGB | SC_UINT8,
	ARGB8S = ST_ARGB | SC_INT8,
	ARGB16U = ST_ARGB | SC_UINT16,
	ARGB16S = ST_ARGB | SC_INT16,
	ARGB16F = ST_ARGB | SC_F16,
	ARGB32U = ST_ARGB | SC_UINT32,
	ARGB32S = ST_ARGB | SC_INT32,
	ARGB32F = ST_ARGB | SC_F32,
	ARGB64F = ST_ARGB | SC_F64,
	/// Special texture types
	INDEXED = SPECIAL_TYPE_START + 0,
	RGB565 = INDEXED + 1,
	RGB555 = RGB565 + 1,
	RGB444 = RGB555 + 1,
	RGBA444 = RGB444 + 1,
	// DXT - RGB444
	DXT1_RGB444 = B_DXT1 | RGB444,
	DXT3_RGB444 = B_DXT3 | RGB444,
	DXT5_RGB444 = B_DXT5 | RGB444,
	// DXT - RGB555
	DXT1_RGB555 = B_DXT1 | RGB555,
	DXT3_RGB555 = B_DXT3 | RGB555,
	DXT5_RGB555 = B_DXT5 | RGB555,
	// DXT - RGB565
	DXT1_RGB565 = B_DXT1 | RGB565,
	DXT3_RGB565 = B_DXT3 | RGB565,
	DXT5_RGB565 = B_DXT5 | RGB565,
};

struct Palette {
	std::vector<std::byte> palette;
	Format format;
	int transparentColorIndex;
};

struct Frame {
	std::vector<std::byte> imageData;
	unsigned width,height,stride;
};

struct DecodeTarget {
	std::vector<Frame> frames;
	std::optional<Palette> palette; // Used for INDEXED only.
	std::optional<float> delayTime; // Used for animated pictures only.
	bool isAnimated; // If false, then the frames are mipmaps. If true, then we except width and height to remain constant.
	Format format;
};

inline constexpr size_t byteSize(Format format) {
	switch (format) {
	case Format::R8U: return sizeof(uint8_t);
	case Format::R8S: return sizeof(int8_t);
	case Format::R16U: return sizeof(uint16_t);
	case Format::R16S: return sizeof(int16_t);
	case Format::R16F: return sizeof(uint16_t);
	case Format::R32U: return sizeof(uint32_t);
	case Format::R32S: return sizeof(int32_t);
	case Format::R32F: return sizeof(float);
	case Format::R64F: return sizeof(double);
	case Format::RG8U: return sizeof(RG<uint8_t>);
	case Format::RG8S: return sizeof(RG<int8_t>);
	case Format::RG16U: return sizeof(RG<uint16_t>);
	case Format::RG16S: return sizeof(RG<int16_t>);
	case Format::RG16F: return sizeof(RG<uint16_t>);
	case Format::RG32U: return sizeof(RG<uint32_t>);
	case Format::RG32S: return sizeof(RG<int32_t>);
	case Format::RG32F: return sizeof(RG<float>);
	case Format::RG64F: return sizeof(RG<double>);
	case Format::RGB8U: return sizeof(RGB<uint8_t>);
	case Format::RGB8S: return sizeof(RGB<int8_t>);
	case Format::RGB16U: return sizeof(RGB<uint16_t>);
	case Format::RGB16S: return sizeof(RGB<int16_t>);
	case Format::RGB16F: return sizeof(RGB<uint16_t>);
	case Format::RGB32U: return sizeof(RGB<uint32_t>);
	case Format::RGB32S: return sizeof(RGB<int32_t>);
	case Format::RGB32F: return sizeof(RGB<float>);
	case Format::RGB64F: return sizeof(RGB<double>);
	case Format::BGR8U: return sizeof(BGR<uint8_t>);
	case Format::BGR8S: return sizeof(BGR<int8_t>);
	case Format::BGR16U: return sizeof(BGR<uint16_t>);
	case Format::BGR16S: return sizeof(BGR<int16_t>);
	case Format::BGR16F: return sizeof(BGR<uint16_t>);
	case Format::BGR32U: return sizeof(BGR<uint32_t>);
	case Format::BGR32S: return sizeof(BGR<int32_t>);
	case Format::BGR32F: return sizeof(BGR<float>);
	case Format::BGR64F: return sizeof(BGR<double>);
	case Format::RGBA8U: return sizeof(RGBA<uint8_t>);
	case Format::RGBA8S: return sizeof(RGBA<int8_t>);
	case Format::RGBA16U: return sizeof(RGBA<uint16_t>);
	case Format::RGBA16S: return sizeof(RGBA<int16_t>);
	case Format::RGBA16F: return sizeof(RGBA<uint16_t>);
	case Format::RGBA32U: return sizeof(RGBA<uint32_t>);
	case Format::RGBA32S: return sizeof(RGBA<int32_t>);
	case Format::RGBA32F: return sizeof(RGBA<float>);
	case Format::RGBA64F: return sizeof(RGBA<double>);
	case Format::BGRA8U: return sizeof(BGRA<uint8_t>);
	case Format::BGRA8S: return sizeof(BGRA<int8_t>);
	case Format::BGRA16U: return sizeof(BGRA<uint16_t>);
	case Format::BGRA16S: return sizeof(BGRA<int16_t>);
	case Format::BGRA16F: return sizeof(BGRA<uint16_t>);
	case Format::BGRA32U: return sizeof(BGRA<uint32_t>);
	case Format::BGRA32S: return sizeof(BGRA<int32_t>);
	case Format::BGRA32F: return sizeof(BGRA<float>);
	case Format::BGRA64F: return sizeof(BGRA<double>);
	case Format::ARGB8U: return sizeof(ARGB<uint8_t>);
	case Format::ARGB8S: return sizeof(ARGB<int8_t>);
	case Format::ARGB16U: return sizeof(ARGB<uint16_t>);
	case Format::ARGB16S: return sizeof(ARGB<int16_t>);
	case Format::ARGB16F: return sizeof(ARGB<uint16_t>);
	case Format::ARGB32U: return sizeof(ARGB<uint32_t>);
	case Format::ARGB32S: return sizeof(ARGB<int32_t>);
	case Format::ARGB32F: return sizeof(ARGB<float>);
	case Format::ARGB64F: return sizeof(ARGB<double>);
	case Format::INDEXED: return sizeof(uint8_t);
	case Format::RGB565: return sizeof(uint16_t);
	case Format::RGB555: return sizeof(uint16_t);
	case Format::RGBA444: return sizeof(uint16_t);
	default: return 0;
	}
}

}
}
#endif // MHDECODETARGET_HPP
