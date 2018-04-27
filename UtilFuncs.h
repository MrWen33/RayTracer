#include<iostream>
inline double clamp(double n)
{
	if (n > 1)return 1;
	if (n < 0)return 0;
	return n;
}

inline double rand01()
{
	return (double)rand() / (double)RAND_MAX;
}

//inline int toInt(double n)
//{
//	//��rgbֵ��0-1תΪ0-255
//	return 255 * clamp(n);
//}

inline int toInt(double x) { return int(pow(clamp(x), 1 / 2.2f) * 255 + .5); }