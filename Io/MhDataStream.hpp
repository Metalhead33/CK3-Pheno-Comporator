#ifndef MHDATASTREAM_HPP
#define MHDATASTREAM_HPP
#include <Util/MhEndianness.hpp>
#include "MhIoDevice.hpp"
#include <string>
#include <vector>
#include <deque>
#include <array>
#include <list>
#include <forward_list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#if __cplusplus > 201703L
#include <span>
#endif
#include <stack>
#include <queue>

namespace MH33 {
namespace Io {

template <Util::Endian io_endianness> struct DataStream : public Device {
	Device& device;
	DataStream(Device& ndevice) : device(ndevice) {

	}
	// Proxy around the IoDevice
	bool flush() { return device.flush(); }
	bool seek(SeekOrigin whence, intptr_t offset) { return device.seek(whence,offset); }
	intptr_t tell() { return device.tell(); }
	intptr_t size() { return device.size(); }
	size_t write(const void* data, size_t dataSize) { return device.write(data,dataSize); }
	size_t read(void* destination, size_t dataSize) { return device.read(destination,dataSize); }
	Mode getMode() const { return device.getMode(); }
	// Now come the convenience functions
	inline DataStream& operator<<(uint8_t data) {
		device.write(&data,sizeof(uint8_t));
		return *this;
	}
	inline DataStream& operator>>(uint8_t& data) {
		device.read(&data,sizeof(uint8_t));
		return *this;
	}
	inline DataStream& operator<<(int8_t data) {
		device.write(&data,sizeof(int8_t));
		return *this;
	}
	inline DataStream& operator>>(int8_t& data) {
		device.read(&data,sizeof(int8_t));
		return *this;
	}
	inline DataStream& operator<<(bool data) {
		device.write(&data,sizeof(bool));
		return *this;
	}
	inline DataStream& operator>>(bool& data) {
		device.read(&data,sizeof(bool));
		return *this;
	}
	inline DataStream& operator<<(uint16_t data) {
		Util::convert_endian<Util::Endian::Native,io_endianness>(data);
		device.write(&data,sizeof(uint16_t));
		return *this;
	}
	inline DataStream& operator>>(uint16_t& data) {
		device.read(&data,sizeof(uint16_t));
		Util::convert_endian<io_endianness,Util::Endian::Native>(data);
		return *this;
	}
	inline DataStream& operator<<(int16_t data) {
		Util::convert_endian<Util::Endian::Native,io_endianness>(data);
		device.write(&data,sizeof(int16_t));
		return *this;
	}
	inline DataStream& operator>>(int16_t& data) {
		device.read(&data,sizeof(int16_t));
		Util::convert_endian<io_endianness,Util::Endian::Native>(data);
		return *this;
	}
	inline DataStream& operator<<(half_float::half data) {
		Util::convert_endian<Util::Endian::Native,io_endianness>(data);
		device.write(&data,sizeof(half_float::half));
		return *this;
	}
	inline DataStream& operator>>(half_float::half& data) {
		device.read(&data,sizeof(half_float::half));
		Util::convert_endian<io_endianness,Util::Endian::Native>(data);
		return *this;
	}
	inline DataStream& operator<<(uint32_t data) {
		Util::convert_endian<Util::Endian::Native,io_endianness>(data);
		device.write(&data,sizeof(uint32_t));
		return *this;
	}
	inline DataStream& operator>>(uint32_t& data) {
		device.read(&data,sizeof(uint32_t));
		Util::convert_endian<io_endianness,Util::Endian::Native>(data);
		return *this;
	}
	inline DataStream& operator<<(int32_t data) {
		Util::convert_endian<Util::Endian::Native,io_endianness>(data);
		device.write(&data,sizeof(int32_t));
		return *this;
	}
	inline DataStream& operator>>(int32_t& data) {
		device.read(&data,sizeof(int32_t));
		Util::convert_endian<io_endianness,Util::Endian::Native>(data);
		return *this;
	}
	inline DataStream& operator<<(float data) {
		Util::convert_endian<Util::Endian::Native,io_endianness>(data);
		device.write(&data,sizeof(float));
		return *this;
	}
	inline DataStream& operator>>(float& data) {
		device.read(&data,sizeof(float));
		Util::convert_endian<io_endianness,Util::Endian::Native>(data);
		return *this;
	}
	inline DataStream& operator<<(uint64_t data) {
		Util::convert_endian<Util::Endian::Native,io_endianness>(data);
		device.write(&data,sizeof(uint64_t));
		return *this;
	}
	inline DataStream& operator>>(uint64_t& data) {
		device.read(&data,sizeof(uint64_t));
		Util::convert_endian<io_endianness,Util::Endian::Native>(data);
		return *this;
	}
	inline DataStream& operator<<(int64_t data) {
		Util::convert_endian<Util::Endian::Native,io_endianness>(data);
		device.write(&data,sizeof(int64_t));
		return *this;
	}
	inline DataStream& operator>>(int64_t& data) {
		device.read(&data,sizeof(int64_t));
		Util::convert_endian<io_endianness,Util::Endian::Native>(data);
		return *this;
	}
	inline DataStream& operator<<(double data) {
		Util::convert_endian<Util::Endian::Native,io_endianness>(data);
		device.write(&data,sizeof(double));
		return *this;
	}
	inline DataStream& operator>>(double& data) {
		device.read(&data,sizeof(double));
		Util::convert_endian<io_endianness,Util::Endian::Native>(data);
		return *this;
	}
	template <typename T, class Traits = std::char_traits<T>, class Allocator = std::allocator<T>> inline DataStream& operator<<(const std::basic_string<T,Traits,Allocator>& data) {
		*this << uint32_t(data.size());
		if constexpr(sizeof(T) == 1) device.write(data.data(),data.size());
		else {
			for(const auto& it : data) *this << it;
		}
		return *this;
	}
	template <typename T, class Traits = std::char_traits<T>, class Allocator = std::allocator<T>> inline DataStream& operator>>(std::basic_string<T,Traits,Allocator>& data) {
		uint32_t len;
		*this >> len;
		data.resize(len,0);
		if constexpr(sizeof(T) == 1) device.read(data.data(),len);
		else {
			for(auto& it : data) *this >> it;
		}
		return *this;
	}
	template <typename T, class Allocator = std::allocator<T>> inline DataStream& operator<<(const std::vector<T,Allocator>& data) {
		*this << uint32_t(data.size());
		if constexpr(sizeof(T) == 1) device.write(data.data(),data.size());
		else {
			for(const auto& it : data) *this << it;
		}
		return *this;
	}
	template <typename T, class Allocator = std::allocator<T>> inline DataStream& operator>>(std::vector<T,Allocator>& data) {
		uint32_t len;
		*this >> len;
		data.resize(len,0);
		if constexpr(sizeof(T) == 1) device.read(data.data(),len);
		else {
			for(auto& it : data) *this >> it;
		}
		return *this;
	}
	template <typename T, size_t size> inline DataStream& operator<<(const std::array<T,size>& data) {
		if constexpr(sizeof(T) == 1) device.write(data.data(),data.size());
		else {
			for(const auto& it : data) *this << it;
		}
		return *this;
	}
	template <typename T, size_t size> inline DataStream& operator>>(std::array<T,size>& data) {
		if constexpr(sizeof(T) == 1) device.read(data.data(),data.size());
		else {
			for(auto& it : data) *this >> it;
		}
		return *this;
	}
#if __cplusplus > 201703L
	template <class T,std::size_t Extent = std::dynamic_extent> inline DataStream& operator<<(const std::span<T,Extent>& data) {
		if constexpr(sizeof(T) == 1) device.write(data.data(),data.size());
		else {
			for(const auto& it : data) *this << it;
		}
		return *this;
	}
	template <class T,std::size_t Extent = std::dynamic_extent> inline DataStream& operator>>(std::span<T,Extent>& data) {
		if constexpr(sizeof(T) == 1) device.read(data.data(),data.size());
		else {
			for(auto& it : data) *this >> it;
		}
		return *this;
	}
#endif
	template <typename T, class Allocator = std::allocator<T>> inline DataStream& operator<<(const std::list<T,Allocator>& data) {
		*this << uint32_t(data.size());
		for(const auto& it : data) *this << it;
		return *this;
	}
	template <typename T, class Allocator = std::allocator<T>> inline DataStream& operator>>(std::list<T,Allocator>& data) {
		uint32_t len;
		*this >> len;
		data.resize(len,0);
		for(auto& it : data) *this >> it;
		return *this;
	}
	template <typename T, class Allocator = std::allocator<T>> inline DataStream& operator<<(const std::forward_list<T,Allocator>& data) {
		*this << uint32_t(data.size());
		for(const auto& it : data) *this << it;
		return *this;
	}
	template <typename T, class Allocator = std::allocator<T>> inline DataStream& operator>>(std::forward_list<T,Allocator>& data) {
		uint32_t len;
		*this >> len;
		data.resize(len,0);
		for(auto& it : data) *this >> it;
		return *this;
	}
	template <typename T, class Allocator = std::allocator<T>> inline DataStream& operator<<(const std::deque<T,Allocator>& data) {
		*this << uint32_t(data.size());
		for(const auto& it : data) *this << it;
		return *this;
	}
	template <typename T, class Allocator = std::allocator<T>> inline DataStream& operator>>(std::deque<T,Allocator>& data) {
		uint32_t len;
		*this >> len;
		data.resize(len,0);
		for(auto& it : data) *this >> it;
		return *this;
	}
	template <class Key,
			 class Compare = std::less<Key>,
			 class Allocator = std::allocator<Key>> inline DataStream& operator<<(const std::set<Key,Compare,Allocator>& data) {
		*this << uint32_t(data.size());
		for(const auto& it : data) *this << it;
		return *this;
	}
	template <class Key,
			 class Compare = std::less<Key>,
			 class Allocator = std::allocator<Key>> inline DataStream& operator>>(std::set<Key,Compare,Allocator>& data) {
		uint32_t len;
		*this >> len;
		for(uint32_t i = 0; i < len;++i) {
			Key k;
			*this >> k;
			data.insert(std::move(k));
		}
		return *this;
	}
	template <class Key,
			 class Hash = std::hash<Key>,
			 class Compare = std::less<Key>,
			 class Allocator = std::allocator<Key>> inline DataStream& operator<<(const std::unordered_set<Key,Hash,Compare,Allocator>& data) {
		*this << uint32_t(data.size());
		for(const auto& it : data) *this << it;
		return *this;
	}
	template <class Key,
			 class Hash = std::hash<Key>,
			 class Compare = std::less<Key>,
			 class Allocator = std::allocator<Key>> inline DataStream& operator>>(std::unordered_set<Key,Hash,Compare,Allocator>& data) {
		uint32_t len;
		*this >> len;
		for(uint32_t i = 0; i < len;++i) {
			Key k;
			*this >> k;
			data.insert(std::move(k));
		}
		return *this;
	}
	template <class Key,
			 class T,
			 class Compare = std::less<Key>,
			 class Allocator = std::allocator<std::pair<const Key, T>>> inline DataStream& operator<<(const std::map<Key,T,Compare,Allocator>& data) {
		*this << uint32_t(data.size());
		for(auto it = std::begin(data); it != std::end(data); ++it) {
			*this << it->first << it->second;
		}
		return *this;
	}
	template <class Key,
			 class T,
			 class Compare = std::less<Key>,
			 class Allocator = std::allocator<std::pair<const Key, T>>> inline DataStream& operator>>(std::map<Key,T,Compare,Allocator>& data) {
		uint32_t len;
		*this >> len;
		for(uint32_t i = 0; i < len;++i) {
			Key k;
			T t;
			*this >> k >> t;
			data.emplace(std::move(k),std::move(t));
		}
		return *this;
	}
	template <class Key,
			 class T,
			 class Hash = std::hash<Key>,
			 class KeyEqual = std::equal_to<Key>,
			 class Allocator = std::allocator< std::pair<const Key, T>>> inline DataStream& operator<<(const std::unordered_map<Key,T,Hash,KeyEqual,Allocator>& data) {
		*this << uint32_t(data.size());
		for(auto it = std::begin(data); it != std::end(data); ++it) {
			*this << it->first << it->second;
		}
		return *this;
	}
	template <class Key,
			 class T,
			 class Hash = std::hash<Key>,
			 class KeyEqual = std::equal_to<Key>,
			 class Allocator = std::allocator< std::pair<const Key, T>>> inline DataStream& operator>>(std::unordered_map<Key,T,Hash,KeyEqual,Allocator>& data) {
		uint32_t len;
		*this >> len;
		for(uint32_t i = 0; i < len;++i) {
			Key k;
			T t;
			*this >> k >> t;
			data.emplace(std::move(k),std::move(t));
		}
		return *this;
	}
	// Container adapters cannot be serialized, because the container is a protected member.
	/*template<class T,class Container = std::deque<T>> inline DataStream& operator<<(const std::stack<T,Container>& data) {
		*this << data.c;
	}
	template<class T,class Container = std::deque<T>> inline DataStream& operator>>(const std::stack<T,Container>& data) {
		*this >> data.c;
	}
	template<class T,class Container = std::deque<T>> inline DataStream& operator<<(const std::queue<T,Container>& data) {
		*this << data.c;
	}
	template<class T,class Container = std::deque<T>> inline DataStream& operator>>(const std::queue<T,Container>& data) {
		*this >> data.c;
	}
	template<class T,class Container = std::deque<T>,
			 class Compare = std::less<typename Container::value_type>> inline DataStream& operator<<(const std::priority_queue<T,Container,Compare>& data) {
		*this << data.c;
	}
	template<class T,class Container = std::deque<T>,
			 class Compare = std::less<typename Container::value_type>> inline DataStream& operator>>(const std::priority_queue<T,Container,Compare>& data) {
		*this >> data.c;
	}*/
};

typedef DataStream<Util::Endian::Little> DataStreamLE;
typedef DataStream<Util::Endian::Big> DataStreamBE;
typedef DataStream<Util::Endian::Native> DataStreamNE;

}
}

#endif // MHDATASTREAM_HPP
