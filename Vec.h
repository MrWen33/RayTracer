#pragma once
#include<iostream>
#include<cassert>
#include<cmath>
#define PI 3.1415926
#define DEBUGPRINT(x) std::cout<<x<<std::endl

template <class T>
class vec3 {
public:
	T x, y, z;

	vec3(T x,T y,T z):x(x),y(y),z(z){}
	vec3() :x(0), y(0), z(0) {}
	vec3(T num) :x(num), y(num), z(num) {}
	vec3(const vec3<T>& v) {
		x = v.x; y = v.y; z = v.z;
	}

	T operator[](int ind) const {
		assert(ind >= 0 && ind <= 2);
		if (ind == 0) return x;
		else if (ind == 1)return y;
		else return z;
	}

	vec3<T> operator+(const vec3& v) const
	{
		return vec3<T>(x + v.x, y + v.y, z + v.z);
	}
	
	vec3<T> operator-(const vec3& v)const
	{
		return vec3<T>(x - v.x, y - v.y, z - v.z);
	}

	vec3<T> operator*(double n) const
	{
		return vec3<T>(x*n, y*n, z*n);
	}

	template<class P>
	vec3<T> operator/(P n) const
	{
		P invn = 1 / n;
		return vec3<T>(x*invn, y*invn, z*invn);
	}

	T dot(const vec3& v)const
	{
		return x*v.x + y*v.y + z*v.z;
	}

	vec3<T> cross(const vec3& v)const
	{
		return vec3<T>(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
	}

	double len() const
	{
		return sqrt((double)(x*x + y*y + z*z));
	}

	vec3<T> normalized() const
	{
		double length = len();
		return (*this) / length;
	}

	vec3<T> mult(const vec3<T> v) const
	{
		return vec3<T>(x*v.x, y*v.y, z*v.z);
	}

};

template <class T>
std::ostream& operator<<(std::ostream& out,const vec3<T>& v)
{
	out << "(" << v.x << "," << v.y << "," << v.z << ")";
	return out;
}

typedef vec3<int> vec3d;
typedef vec3<double> vec3f;