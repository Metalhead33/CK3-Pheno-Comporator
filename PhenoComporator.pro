TEMPLATE = app
CONFIG += c++2a console
CONFIG -= app_bundle
QT -= gui widgets

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH ''= $$top_srcdir/include

# PNG
LIBS += -lpng
# GIF
LIBS += -lgif
# JPEG
LIBS += -lturbojpeg
# WEBP
LIBS += -lwebpdemux -lwebpmux -lwebp -lwebpdecoder

SOURCES += \
        Io/MhBufferWrapper.cpp \
        Io/MhFile.cpp \
        Io/MhStderr.cpp \
        Io/MhStdin.cpp \
        Io/MhStdout.cpp \
        IoSys/MhFilesystem.cpp \
        Media/Image/MhDDS.cpp \
        Media/Image/MhGIF.cpp \
        Media/Image/MhJPEG.cpp \
        Media/Image/MhPNG.cpp \
        Media/Image/MhTGA.cpp \
        Media/Image/MhWEBP.cpp \
        Util/MhRNG.cpp \
        Util/MhUUID.cpp \
        main.cpp

HEADERS += \
	Io/MhBufferWrapper.hpp \
	Io/MhDataStream.hpp \
	Io/MhFile.hpp \
	Io/MhIoDevice.hpp \
	Io/MhStderr.hpp \
	Io/MhStdin.hpp \
	Io/MhStdout.hpp \
	IoSys/MhFilesystem.hpp \
	IoSys/MhIoSystem.hpp \
	Media/Image/MhDDS.hpp \
	Media/Image/MhDecodeTarget.hpp \
	Media/Image/MhGIF.hpp \
	Media/Image/MhJPEG.hpp \
	Media/Image/MhPNG.hpp \
	Media/Image/MhStandardColourFormat.hpp \
	Media/Image/MhTGA.hpp \
	Media/Image/MhWEBP.hpp \
	Util/MhBuffer.hpp \
	Util/MhEndianness.hpp \
	Util/MhIntegralIterator.hpp \
	Util/MhNormDenorm.hpp \
	Util/MhRNG.hpp \
	Util/MhUUID.hpp \
	Util/half.hpp
