#ifndef MHINTEGRALITERATOR_HPP
#define MHINTEGRALITERATOR_HPP
namespace MH33 {
namespace Util {

template <typename T> struct IntegralIterator {
	typedef T dataType;
	dataType var;
	explicit IntegralIterator(dataType nvar=0) : var(nvar) {

	}
	IntegralIterator(const IntegralIterator& cpy) : var(cpy.var) {

	}
	// Overloaded operators - assignment
	inline IntegralIterator& operator=(const IntegralIterator& other) {
		this->var = other.var;
		return *this;
	}
	inline IntegralIterator& operator+=(const IntegralIterator& other) {
		this->var += other.var;
		return *this;
	}
	inline IntegralIterator& operator-=(const IntegralIterator& other) {
		this->var -= other.var;
		return *this;
	}
	inline IntegralIterator& operator*=(const IntegralIterator& other) {
		this->var *= other.var;
		return *this;
	}
	inline IntegralIterator& operator/=(const IntegralIterator& other) {
		this->var /= other.var;
		return *this;
	}
	inline IntegralIterator& operator%=(const IntegralIterator& other) {
		this->var %= other.var;
		return *this;
	}
	// Overloaded operators - new data
	inline IntegralIterator operator+(const IntegralIterator& other) const {
		IntegralIterator a(*this);
		a += other.var;
		return a;
	}
	inline IntegralIterator operator-(const IntegralIterator& other) const {
		IntegralIterator a(*this);
		a -= other.var;
		return a;
	}
	inline IntegralIterator operator*(const IntegralIterator& other) const {
		IntegralIterator a(*this);
		a *= other.var;
		return a;
	}
	inline IntegralIterator operator/(const IntegralIterator& other) const {
		IntegralIterator a(*this);
		a /= other.var;
		return a;
	}
	inline IntegralIterator operator%(const IntegralIterator& other) const {
		IntegralIterator a(*this);
		a %= other.var;
		return a;
	}
	// Overloaded operators - Comparisons
	inline bool operator==(const IntegralIterator& other) {
		return this->var == other.var;
	}
	inline bool operator!=(const IntegralIterator& other) {
		return this->var != other.var;
	}
	inline bool operator<(const IntegralIterator& other) {
		return this->var < other.var;
	}
	inline bool operator<=(const IntegralIterator& other) {
		return this->var <= other.var;
	}
	inline bool operator>(const IntegralIterator& other) {
		return this->var > other.var;
	}
	inline bool operator>=(const IntegralIterator& other) {
		return this->var >= other.var;
	}
	// Iterator-like functionality
	inline IntegralIterator& operator++() { ++var; return *this;}
	inline IntegralIterator& operator--() { --var; return *this;}
	inline IntegralIterator operator++(int) { IntegralIterator tmp(*this); ++var; return tmp;}
	inline IntegralIterator operator--(int) { IntegralIterator tmp(*this); --var; return tmp;}
	inline dataType& operator*() { return var; }
	inline const dataType& operator*() const { return var; }
};
template <typename T> struct IntegralIterable {
	typedef T dataType;
	dataType var;
	typedef IntegralIterator<T> iterator;
	explicit IntegralIterable(dataType nvar=0) : var(nvar) {

	}
	IntegralIterable(const IntegralIterable& cpy) : var(cpy.var) {

	}
	// Overloaded operators - assignment
	inline IntegralIterable& operator=(const IntegralIterable& other) {
		this->var = other.var;
		return *this;
	}
	inline IntegralIterable& operator+=(const IntegralIterable& other) {
		this->var += other.var;
		return *this;
	}
	inline IntegralIterable& operator-=(const IntegralIterable& other) {
		this->var -= other.var;
		return *this;
	}
	inline IntegralIterable& operator*=(const IntegralIterable& other) {
		this->var *= other.var;
		return *this;
	}
	inline IntegralIterable& operator/=(const IntegralIterable& other) {
		this->var /= other.var;
		return *this;
	}
	inline IntegralIterable& operator%=(const IntegralIterable& other) {
		this->var %= other.var;
		return *this;
	}
	// Overloaded operators - new data
	inline IntegralIterable operator+(const IntegralIterable& other) const {
		IntegralIterable a(*this);
		a += other.var;
		return a;
	}
	inline IntegralIterable operator-(const IntegralIterable& other) const {
		IntegralIterable a(*this);
		a -= other.var;
		return a;
	}
	inline IntegralIterable operator*(const IntegralIterable& other) const {
		IntegralIterable a(*this);
		a *= other.var;
		return a;
	}
	inline IntegralIterable operator/(const IntegralIterable& other) const {
		IntegralIterable a(*this);
		a /= other.var;
		return a;
	}
	inline IntegralIterable operator%(const IntegralIterable& other) const {
		IntegralIterable a(*this);
		a %= other.var;
		return a;
	}
	// Overloaded operators - Comparisons
	inline bool operator==(const IntegralIterable& other) {
		return this->var == other.var;
	}
	inline bool operator!=(const IntegralIterable& other) {
		return this->var != other.var;
	}
	inline bool operator<(const IntegralIterable& other) {
		return this->var < other.var;
	}
	inline bool operator<=(const IntegralIterable& other) {
		return this->var <= other.var;
	}
	inline bool operator>(const IntegralIterable& other) {
		return this->var > other.var;
	}
	inline bool operator>=(const IntegralIterable& other) {
		return this->var >= other.var;
	}
	// Iterators
	inline iterator begin() const { return iterator(0); }
	inline iterator end() const { return iterator(var); }
};

}
}
#endif // MHINTEGRALITERATOR_HPP
