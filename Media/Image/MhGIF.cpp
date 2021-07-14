#include "MhGIF.hpp"
#include <gif_lib.h>
#include <memory>
#include <cstring>

namespace MH33 {
namespace GFX {
namespace GIF {

static int LATEST_ERROR;
static void CLOSE_GIF(GifFileType* ptr) { DGifCloseFile(ptr,&LATEST_ERROR); }
static int GIF_INPUT(GifFileType* container, GifByteType* buffer, int size)
{
	if(!container) return -1;
	Io::Device* filehandle = reinterpret_cast<Io::Device*>(container->UserData);
	if(!filehandle) return -1;
	return int(filehandle->read(buffer,size));
}
typedef std::unique_ptr<GifFileType, decltype (&CLOSE_GIF)> HandleType;

void decode(Io::Device &iodev, DecodeTarget &target)
{
	HandleType handle(DGifOpen(&iodev,&GIF_INPUT,&LATEST_ERROR),CLOSE_GIF);
	DGifSlurp(handle.get());
	int tmpFramerate = 0;
	target.format = Format::INDEXED;
	target.isAnimated = true;
	target.palette.emplace();
	target.palette->transparentColorIndex = -1;
	for(int i = 0; i < handle->ExtensionBlockCount; ++i)
	{
		GraphicsControlBlock block;
		if (DGifExtensionToGCB(handle->ExtensionBlocks[i].ByteCount,handle->ExtensionBlocks[i].Bytes,
							   &block)) {
			if(block.TransparentColor > target.palette->transparentColorIndex) target.palette->transparentColorIndex = block.TransparentColor;
			if(block.DelayTime > tmpFramerate) tmpFramerate = block.DelayTime;
		}
	}
	target.delayTime = float(tmpFramerate) * 0.01f;
	//target.width = handle->SWidth;
	//target.height = handle->SHeight;
	ColorMapObject *gifcolormap = handle->SColorMap;
	if(!gifcolormap) return;
	GifColorType *gifpalette = gifcolormap->Colors;
	if(!gifpalette) return;
	target.palette->palette.resize(gifcolormap->ColorCount * sizeof(RGB<uint8_t>));
	target.palette->format = Format::RGB8U;
	RGB<uint8_t>* ptr = reinterpret_cast<RGB<uint8_t>*>(target.palette->palette.data());
	for(int i = 0; i < gifcolormap->ColorCount; ++i) {
		ptr[i].r = gifcolormap->Colors[i].Red;
		ptr[i].g = gifcolormap->Colors[i].Green;
		ptr[i].b = gifcolormap->Colors[i].Blue;
	}
	target.frames.resize(handle->ImageCount);
	for(int i = 0; i < handle->ImageCount; ++i) {
		const int fullsize = handle->SWidth * handle->SHeight;
		target.frames[i].width = handle->SWidth;
		target.frames[i].height = handle->SHeight;
		target.frames[i].stride = handle->SWidth;
		target.frames[i].imageData.resize(fullsize);
		memcpy(target.frames[i].imageData.data(),handle->SavedImages[i].RasterBits,fullsize);
	}
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
