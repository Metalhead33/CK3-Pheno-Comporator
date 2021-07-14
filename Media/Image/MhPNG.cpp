#include "MhPNG.hpp"
#include <png.h>
#include <algorithm>

void mh33_set_read_callbacks(png_structp read_ptr, MH33::Io::Device* dev);
void mh33_set_write_callbacks(png_structp write_ptr, MH33::Io::Device* dev);
void mh33_read_data(png_structp png_ptr, png_bytep data, png_size_t length);
void mh33_write_data(png_structp png_ptr, png_bytep data, png_size_t length);
void mh33_flush_data(png_structp png_ptr);

namespace MH33 {
namespace GFX {
namespace PNG {

constexpr size_t type2bytes(uint8_t color_type, uint8_t bit_depth) {
	switch (color_type) {

	case PNG_COLOR_TYPE_RGB: return (bit_depth/8) * 3;
	case PNG_COLOR_TYPE_RGBA: return (bit_depth/8) * 4;
	case PNG_COLOR_TYPE_GRAY: return bit_depth/8;
	case PNG_COLOR_TYPE_PALETTE: return sizeof(uint8_t);
	default: return 0;
}
}

void decode(Io::Device& iodev, DecodeTarget &destination)
{
	uint8_t color_type;
	uint8_t bit_depth;
	auto pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING,nullptr,nullptr,nullptr);
	if(!pngPtr) return;
	auto infoPtr = png_create_info_struct(pngPtr);
	if(!infoPtr) {
		png_destroy_read_struct(&pngPtr,nullptr,nullptr);
		return;
	}
	auto endInfoPtr = png_create_info_struct(pngPtr);
	if(!endInfoPtr) {
		png_destroy_read_struct(&pngPtr,&infoPtr,nullptr);
		return;
	}
	png_set_read_fn(pngPtr,&iodev,mh33_read_data);
	if(iodev.tell()) png_set_sig_bytes(pngPtr, iodev.tell());
	png_read_info(pngPtr, infoPtr);
	destination.isAnimated = false;
	destination.frames.resize(1);
	destination.frames[0].width = png_get_image_width(pngPtr,infoPtr);
	destination.frames[0].height = png_get_image_height(pngPtr,infoPtr);
	color_type = png_get_color_type(pngPtr,infoPtr);
	bit_depth = png_get_bit_depth(pngPtr,infoPtr);
	switch (color_type) {
	case PNG_COLOR_TYPE_RGB: switch (bit_depth) {
		case 8: destination.format = Format::RGB8U; break;
		case 16: destination.format = Format::RGB16U; break;
		default: destination.format = Format::INVALID; break;
		} break;
	case PNG_COLOR_TYPE_RGBA: switch (bit_depth) {
		case 8: destination.format = Format::RGBA8U; break;
		case 16: destination.format = Format::RGBA16U; break;
		default: destination.format = Format::INVALID; break;
		} break;
	case PNG_COLOR_TYPE_GRAY: switch (bit_depth) {
		case 1:
		case 2:
		case 4: png_set_expand_gray_1_2_4_to_8(pngPtr); [[fallthrough]];
		case 8: destination.format = Format::R8U; break;
		case 16: destination.format = Format::R16U; break;
		default: destination.format = Format::INVALID; break;
		} break;
	case PNG_COLOR_TYPE_PALETTE: switch (bit_depth) {
		case 1:
		case 2:
		case 4: png_set_packing(pngPtr); [[fallthrough]];
		case 8: destination.format = Format::INDEXED; break;
		default: destination.format = Format::INVALID; break;
		} break;
	}
	if(destination.format == Format::INVALID) {
		png_destroy_read_struct(&pngPtr,&infoPtr,&endInfoPtr);
		return;
	}
	int number_of_passes;
	number_of_passes = png_set_interlace_handling(pngPtr);
	png_read_update_info(pngPtr, infoPtr);
	destination.frames[0].stride = png_get_rowbytes(pngPtr, infoPtr);
	destination.frames[0].imageData.resize(destination.frames[0].height*png_get_rowbytes(pngPtr, infoPtr));
	std::vector<png_bytep> rowPtrs(destination.frames[0].height);
	for (int y=0; y<destination.frames[0].height; y++) {
		rowPtrs[y] = reinterpret_cast<png_bytep>(&(destination.frames[0].imageData[y*png_get_rowbytes(pngPtr, infoPtr)]));
	}
	png_read_image(pngPtr, rowPtrs.data());
	if(color_type == PNG_COLOR_TYPE_PALETTE) {
		png_colorp png_palette = nullptr;
		int palette_entries = 0;
		png_get_PLTE(pngPtr,infoPtr, &png_palette, &palette_entries);
		destination.palette = Palette();
		destination.palette->format = Format::RGB8U;
		destination.palette->palette.resize(sizeof(RGB<uint8_t>)*palette_entries);
		RGB<uint8_t>* entries = reinterpret_cast<RGB<uint8_t>*>(destination.palette->palette.data());
		for(int i = 0; i < palette_entries; ++i) {
			entries[i].r = png_palette[i].red;
			entries[i].g = png_palette[i].green;
			entries[i].b = png_palette[i].blue;
		}
	}
	png_destroy_read_struct(&pngPtr,&infoPtr,&endInfoPtr);
}

void encode(Io::Device& iodev, uint16_t width, uint16_t height, uint8_t color_type, uint8_t bit_depth, Util::Buffer &pixelData, float compressionLevel)
{
	auto pngPtr = png_create_write_struct(PNG_LIBPNG_VER_STRING,nullptr,nullptr,nullptr);
	if(!pngPtr) return;
	auto infoPtr = png_create_info_struct(pngPtr);
	if(!infoPtr) {
		png_destroy_write_struct(&pngPtr,nullptr);
		return;
	}
	png_set_write_fn(pngPtr,&iodev,mh33_write_data,mh33_flush_data);
	png_set_IHDR(pngPtr,infoPtr,width,height,bit_depth,color_type,PNG_INTERLACE_NONE,
				 PNG_COMPRESSION_TYPE_DEFAULT,PNG_FILTER_TYPE_DEFAULT);
	png_set_compression_level(pngPtr,int(std::clamp(compressionLevel,0.0f,1.0f)*9.0f));
	std::vector<png_bytep> rowPtrs(height);
	for (int y=0; y<height; y++) {
		rowPtrs[y] = reinterpret_cast<png_bytep>(&pixelData[type2bytes(color_type,bit_depth)*width*y]);
	}
	png_write_info(pngPtr,infoPtr);
	png_write_rows(pngPtr,rowPtrs.data(),rowPtrs.size());
	png_write_end(pngPtr,infoPtr);
	png_destroy_write_struct(&pngPtr,&infoPtr);
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

void mh33_set_read_callbacks(png_structp read_ptr, MH33::Io::Device* dev) {
	png_set_read_fn(read_ptr, dev, mh33_read_data);
}
void mh33_set_write_callbacks(png_structp write_ptr, MH33::Io::Device* dev) {
	png_set_write_fn(write_ptr, dev, mh33_write_data, mh33_flush_data);
}
void mh33_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
	reinterpret_cast<MH33::Io::Device*>(png_get_io_ptr(png_ptr))->read(data,length);
}
void mh33_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
	reinterpret_cast<MH33::Io::Device*>(png_get_io_ptr(png_ptr))->write(data,length);
}
void mh33_flush_data(png_structp png_ptr) {
	reinterpret_cast<MH33::Io::Device*>(png_get_io_ptr(png_ptr))->flush();
}
