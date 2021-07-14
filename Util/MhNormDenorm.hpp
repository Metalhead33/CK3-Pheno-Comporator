#include <limits>
#include <cmath>
#include "half.hpp"
namespace MH33 {
namespace Util {

template <typename T> struct _fnorm_fdenorm {
	constexpr static const float minF = -1.0f * float(std::numeric_limits<T>::min());
	constexpr static const float maxF = float(std::numeric_limits<T>::max());
	constexpr static const float recMinF = ((std::numeric_limits<T>::min())) ? 1.0f / minF : 0.0f;
	constexpr static const float recMaxF = 1.0f / maxF;
	inline static constexpr float normalize(const T& val) {
		return (val < 0) ? (float(val) * recMinF) : (float(val) * recMaxF);
	}
	inline static T denormalize(const float& val) {
		return (std::signbit(val)) ? T(val*minF) : T(val*maxF);
	}
};
template <> struct _fnorm_fdenorm<half_float::half> {
	inline static float normalize(const half_float::half& val) {
		return float(val);
	}
	inline static half_float::half denormalize(const float& val) {
		return half_float::half(val);
	}
};
template <> struct _fnorm_fdenorm<float> {
	inline constexpr static float normalize(const float& val) {
		return val;
	}
	inline constexpr static float denormalize(const float& val) {
		return val;
	}
};
template <> struct _fnorm_fdenorm<double> {
	inline constexpr static float normalize(const double& val) {
		return float(val);
	}
	inline constexpr static double denormalize(const float& val) {
		return double(val);
	}
};

template <typename T> inline float fnormalize(const T& val) { return _fnorm_fdenorm<T>::normalize(val); }
template <typename T> inline T fdenormalize(const float& val) { return _fnorm_fdenorm<T>::denormalize(val); }


template <typename T> struct _dnorm_ddenorm {
	constexpr static const double minF = -1.0f * double(std::numeric_limits<T>::min());
	constexpr static const double maxF = double(std::numeric_limits<T>::max());
	constexpr static const double recMinF = ((std::numeric_limits<T>::min())) ? 1.0 / minF : 0.0;
	constexpr static const double recMaxF = 1.0 / maxF;
	inline static constexpr double normalize(const T& val) {
		return (val < 0) ? (double(val) * recMinF) : (double(val) * recMaxF);
	}
	inline static T denormalize(const double& val) {
		return (std::signbit(val)) ? T(val*minF) : T(val*maxF);
	}
};
template <> struct _dnorm_ddenorm<half_float::half> {
	inline static double normalize(const half_float::half& val) {
		return double(val);
	}
	inline static half_float::half denormalize(const double& val) {
		return half_float::half(float(val));
	}
};
template <> struct _dnorm_ddenorm<float> {
	inline constexpr static double normalize(const float& val) {
		return double(val);
	}
	inline constexpr static float denormalize(const double& val) {
		return float(val);
	}
};
template <> struct _dnorm_ddenorm<double> {
	inline constexpr static double normalize(const double& val) {
		return val;
	}
	inline constexpr static double denormalize(const double& val) {
		return val;
	}
};

template <typename T> inline double dnormalize(const T& val) { return _dnorm_ddenorm<T>::normalize(val); }
template <typename T> inline T ddenormalize(const double& val) { return _dnorm_ddenorm<T>::denormalize(val); }

}
}
