#pragma once
#include<iostream>
#include<string>
#include<algorithm>
#include<new>
#include<cstdlib>
#include<type_traits>
#include<utility>
#include<functional>
_PANAGIOTIS_BEGIN
#if __cplusplus > 202002L
template<typename _Ty>
class Array2D final {
private:
	_Ty* Array;
	std::size_t x, y;
public:
	Array2D()noexcept :Array{ nullptr }, x{ 0 }, y{ 0 } {}
	Array2D(const std::size_t dimx, const std::size_t dimy) : x{ dimx }, y{ dimy }
	{
		static_assert(std::is_default_constructible_v<_Ty>);
		//the type must be default constructible 
		Array = new _Ty[x * y]{};

	}
	Array2D(_Ty* p,const std::size_t dimx,const std::size_t dimy) {
		Array = p;
		x = dimx;
		y = dimy;

	}
	Array2D(const std::size_t dimx, const std::size_t dimy,
		const _Ty& default_value) :x{ dimx }, y{ dimy }
	{
		static_assert(std::is_default_constructible_v<_Ty>);
		Array = new _Ty[x * y]{};
		std::fill_n(Array, x * y, default_value);

	}
	Array2D(const Array2D& other) :x{ other.x }, y{ other.y }, Array{ nullptr } {
		if (this != &other) {
			if (other.size() != this->size()) {
				static_assert(std::is_default_constructible_v<_Ty>);
				//the type must be default constructible 
				Array = new _Ty[other.x * other.y]{};


			}
			std::copy_n(other.Array, other.x * other.y, Array);
		}

	}
	Array2D(Array2D&& other)noexcept :x{ 0 }, y{ 0 }, Array{ nullptr }
	{
		if (this != &other) {
			std::swap(Array, other.Array);
			std::swap(x, other.x);
			std::swap(y, other.y);
		}
	}
	std::size_t size()const noexcept {
		return x * y;
	}
	bool empty()const noexcept {
		return Array == nullptr;
	}
	void clear()noexcept {
		static_assert(std::is_nothrow_destructible_v<_Ty>);
		//doesnt make sence for the type to not be destructible
		if (!empty()) {
			delete[] Array;
			Array = nullptr;
			x = y = 0;

		}
	}
	bool Allocate(const std::size_t dimx, const std::size_t dimy) {
		if (dimx == 0 || dimy == 0)return false;
		if(Array!=nullptr && (dimx*dimy)==(x*y))return false;//no need to allocate memory we already have the memory 
		static_assert(std::is_default_constructible_v<_Ty>);
		//the type must be default constructible 
		this->clear();
		Array = new(std::nothrow) _Ty[dimx * dimy]{};
		if (Array != nullptr) {
			x = dimx;
			y = dimy;
			return true;
		}
		else {

			x = y = 0;
			return false;
		}
	}
	bool Allocate(const std::size_t dimx, const std::size_t dimy,
		const _Ty& default_value) {
		if (dimx == 0 || dimy == 0)return false;
		if(Array!=nullptr && (dimx*dimy)==(x*y))
		{
		   std::fill_n(Array, dimx * dimy, default_value);
	           return false;
		}//no need to allocate memory we already have the memory 
		static_assert(std::is_default_constructible_v<_Ty>);
		//the type must be constructible withe default value
		this->clear();
		Array = new(std::nothrow) _Ty[dimx * dimy]{};
		if (Array != nullptr) {
			std::fill_n(Array, dimx * dimy, default_value);
			x = dimx;
			y = dimy;
			return true;
		}
		else {
			x = y = 0;
			return false;
		}
	}

	void initialize(const _Ty& default_value) {
		if (!empty())std::fill_n(Array, x * y, default_value);
	}

	std::size_t get_dimx()const noexcept {
		return x;
	}
	std::size_t get_dimy()const noexcept {
		return y;
	}
	bool operator==(const Array2D& other)const noexcept {
		if (x != other.x || y != other.y)return false;
		for (std::size_t i = 0; i < size(); i++) {
			if (Array[i] != other.Array[i])return false;
		}
		return true;
	}
	template<typename _comp>
	bool compare(const Array2D& other, _comp comp) {
		if (x != other.x || y != other.y)return false;
		for (std::size_t i = 0; i < size(); i++) {
			if (!std::invoke(comp, Array[i], other.Array[i]))return false;
		}
		return true;
	}
	bool operator !=(const Array2D& other)const noexcept {
		return !((*this) == other);
	}
	Array2D& operator =(const Array2D& other)& {
		if (this != &other) {
			if (x != other.x || y != other.y) {
				this->clear();
				if (!Allocate(other.x, other.y)) {
					throw std::bad_alloc{};
				}
				x = other.x;
				y = other.y;
			}


			std::copy_n(other.Array, x * y, Array);

		}
		return *this;
	}
	Array2D& operator = (Array2D&& other) & noexcept {
		if (this != &other) {
			this->clear();
			std::swap(Array, other.Array);
			std::swap(x, other.x);
			std::swap(y, other.y);

		}
		return *this;
	}
	_Ty& operator ()(const std::size_t dimx, const std::size_t dimy) & noexcept {
		return Array[x * dimy + dimx];
	}
	const _Ty& operator ()(const std::size_t dimx, const std::size_t dimy) const& noexcept {
		return Array[x * dimy + dimx];
	}
	_Ty&& operator ()(const std::size_t dimx, const std::size_t dimy) && noexcept {
		return Array[x * dimy + dimx];
	}
	const _Ty&& operator ()(const std::size_t dimx, const std::size_t dimy) const&& noexcept {
		return Array[x * dimy + dimx];
	}
	void set(const std::size_t dimx, const std::size_t dimy, const _Ty& value) {
		Array[x * dimy + dimx] = value;
	}
	_Ty& get(const std::size_t dimx, const std::size_t dimy) & noexcept {
		return Array[x * dimy + dimx];
	}
	_Ty&& get(const std::size_t dimx, const std::size_t dimy) && noexcept {
		return std::move(Array[x * dimy + dimx]);
	}
	const _Ty& get(const std::size_t dimx, const std::size_t dimy) const& noexcept {
		return Array[x * dimy + dimx];
	}
	const _Ty&& get(const std::size_t dimx, const std::size_t dimy) const&& noexcept {
		return std::move(Array[x * dimy + dimx]);
	}
	~Array2D()noexcept {
		this->clear();
	}
	Array2D<_Ty> ExtractSubArray(const size_t MinX,const size_t MinY,const size_t MaxX,
		const size_t MaxY) const {//return by value use move semantics 
		if (MaxX < MinX|| x<MaxX) { throw OUT_OF_BOUNDS_{"out of bounds"}; }
		if (MaxY < MinY ||y<MaxY) { throw OUT_OF_BOUNDS_{ "out of bounds" }; }

		std::size_t dimx = MaxX - MinX;
		std::size_t dimy = MaxY - MinY;
		if (dimx == 0 || dimy == 0)return Array2D<_Ty>{};
		_Ty* p = new _Ty[dimx * dimy]{};
		std::size_t count = 0;
		for (std::size_t i = MinY; i < MaxY; i++) {
			for (std::size_t  j= MinX; j < MaxX; j++) {
				
				p[count] = Array[x * i + j];
				count++;
			}
		}
		
		return Array2D<_Ty>{p,dimx,dimy};


	}
	void show()const //use this func only to print elemets that support operator <<
	{
		for (std::size_t i = 0; i < size(); i++) {
			std::cout << Array[i] << '\n';
		}
	}

};
#endif
_PANAGIOTIS_END
