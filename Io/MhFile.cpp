#include "MhFile.hpp"
namespace MH33 {
namespace Io {

File::~File()
{
	if(fdev) fclose(fdev);
}

File::File(File &&mov)
	: fdev(mov.fdev)
{
	mov.fdev = nullptr;
}

File &File::operator=(File &&mov)
{
	if(fdev) fclose(fdev);
	this->fdev = mov.fdev;
	mov.fdev = nullptr;
	return *this;
}

File::File()
	: fdev(nullptr)
{

}

File::File(const char *path, Mode mode)
	: fdev(nullptr), mode(mode)
{
	switch (mode) {
	case Mode::READ:
		fdev = fopen(path,"rb");
		break;
	case Mode::WRITE:
		fdev = fopen(path,"wb");
		break;
	case Mode::APPEND:
		fdev = fopen(path,"ab");
		break;
	case Mode::READ_WRITE:
		if(!(fdev = fopen(path,"r+b"))) fdev = fopen(path,"w+b");
		break;
	case Mode::READ_APPEND:
		fdev = fopen(path,"a+b");
		break;
	}
}

File::File(const std::string &path, Mode mode)
	: fdev(nullptr), mode(mode)
{
	switch (mode) {
	case Mode::READ:
		fdev = fopen(path.c_str(),"rb");
		break;
	case Mode::WRITE:
		fdev = fopen(path.c_str(),"wb");
		break;
	case Mode::APPEND:
		fdev = fopen(path.c_str(),"ab");
		break;
	case Mode::READ_WRITE:
		if(!(fdev = fopen(path.c_str(),"r+b"))) fdev = fopen(path.c_str(),"w+b");
		break;
	case Mode::READ_APPEND:
		fdev = fopen(path.c_str(),"a+b");
		break;
	}
}

bool File::flush()
{
	return fflush(fdev) == 0;
}

bool File::seek(SeekOrigin whence, intptr_t offset)
{
	switch (whence) {
	case SeekOrigin::CUR:
		return fseek(fdev,offset,SEEK_CUR) == 0;
	case SeekOrigin::SET:
		return fseek(fdev,offset,SEEK_SET) == 0;
	case SeekOrigin::END:
		return fseek(fdev,offset,SEEK_END) == 0;
	}
	return false;
}

intptr_t File::tell()
{
	return ftell(fdev);
}

intptr_t File::size()
{
	auto originalPos = ftell(fdev);
	fseek(fdev,0,SEEK_END);
	auto theEnd = ftell(fdev);
	fseek(fdev,originalPos,SEEK_SET);
	return theEnd;
}

size_t File::write(const void *data, size_t dataSize)
{
	return fwrite(data,1,dataSize,fdev);
}

size_t File::read(void *destination, size_t dataSize)
{
	return fread(destination,1,dataSize,fdev);
}

Mode File::getMode() const
{
	return mode;
}

}
}
