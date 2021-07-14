#ifndef MHSTCLFMT_HPP
#define MHSTCLFMT_HPP

#ifdef __GNUC__
#define PACKED __attribute__ ((__packed__))
#elif __clang__
#define PACKED __attribute__ ((__packed__))
#else
#define PACKED
#endif

namespace MH33 {
namespace GFX {

#ifdef _WIN32
#pragma pack(push,1)
#endif
template <typename ColourResolution> struct PACKED RG {
	ColourResolution r,g;
};
template <typename ColourResolution> struct PACKED RGB {
	ColourResolution r,g,b;
};
template <typename ColourResolution> struct PACKED BGR {
	ColourResolution b,g,r;
};
template <typename ColourResolution> struct PACKED RGBA {
	ColourResolution r,g,b,a;
};
template <typename ColourResolution> struct PACKED BGRA {
	ColourResolution b,g,r,a;
};
template <typename ColourResolution> struct PACKED ARGB {
	ColourResolution a,r,g,b;
};
#ifdef _WIN32
#pragma pack(pop)
#endif

}
}
#endif // MHSTCLFMT_HPP
