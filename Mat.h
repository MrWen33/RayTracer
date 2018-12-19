#pragma once

#include"Vec.h"
class Mat33 {
private:
	double data[3][3];
public:
	Mat33()//初始化单位矩阵
	{
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
				if (i == j)data[i][j] = 1;
				else data[i][j] = 0;
	}

	Mat33(const vec3f& v1,const vec3f& v2,const vec3f& v3)
	{
		for (int i = 0; i < 3; ++i)
		{
			data[i][0] = v1[i];
			data[i][1] = v2[i];
			data[i][2] = v3[i];
		}
	}

	double* operator[](int a)
	{
		return data[3 * a];
	}

	double det()
	{
		double add1 = data[0][0] * data[1][1] * data[2][2];
		double add2 = data[0][1] * data[1][2] * data[2][0];
		double add3 = data[0][2] * data[1][0] * data[2][1];
		double del1 = data[2][0] * data[1][1] * data[0][2];
		double del2 = data[0][0] * data[1][2] * data[2][1];
		double del3 = data[0][1] * data[1][0] * data[2][2];
		return add1 + add2 + add3 - del1 - del2 - del3;
	}

};