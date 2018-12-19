#pragma once
#include<vector>
#include<string>
#include<fstream>
#include<sstream>
#include"Vec.h"
#include"Triangle.h"
#include"Material.h"

class ObjReader {
public:
	int TriNum=0;
	std::vector<Primitive*> scene;//读取的场景
	std::vector<vec3f> points;//点坐标
	std::vector<vec3f> normals;//法线
	//std::vector<vec2f> textures;//材质坐标

	ObjReader()
	{
		TriNum = 0;
		//不使用下标0
		points.push_back(vec3f());
		normals.push_back(vec3f());
	}

	ObjReader(std::string filename)
	{
		TriNum = 0;
		//不使用下标0
		points.push_back(vec3f());
		normals.push_back(vec3f());
		loadFile(filename);
	}

	~ObjReader() {
		for (Primitive* p : scene)
		{
			delete p;
		}
	}

	bool loadFile(std::string filename, const PhoneMaterial* material = NULL)
	{
		std::cout << "Loading file:" << filename << std::endl;
		std::ifstream in(filename);
		if (!in)
		{
			std::cout << "error:file not exist" << std::endl;
		}
		std::string line;
		while (std::getline(in, line))
		{
			std::istringstream linestream(line);
			std::string mode;
			linestream >> mode;
			//DEBUGPRINT(mode);
			if (mode == "v"||mode=="V")//顶点坐标数据
			{
				vec3f point;
				linestream >> point.x >> point.y >> point.z;
				points.push_back(point);
			}
			if (mode == "vt" || mode == "VT")//顶点材质坐标
			{
				//Todo:读取材质
			}
			if (mode == "vn" || mode == "VN")//顶点法线
			{
				vec3f normal;
				linestream >> normal.x >> normal.y >> normal.z;
				normals.push_back(normal);
			}
			if (mode == "f" || mode == "F")//面数据
			{
				std::string data;
				std::vector<std::string> datas;
				while (linestream >> data)
				{
					datas.push_back(data);
				}
				if (datas.size() == 3)//三角形面片
				{
					TriNum++;
					std::vector<int> pointInd(3);
					std::vector<int> normalInd(3);
					std::vector<int> textureInd(3);//暂时没用

					for (int i = 0; i < 3; ++i)
					{
						std::vector<int> parsedData = parseFaceData(datas[i]);
						pointInd[i] = parsedData[0];
						textureInd[i] = parsedData[1];
						normalInd[i] = parsedData[2];
					}
					vec3f faceNormal;
					for (int i = 0; i < 3; ++i)
					{
						faceNormal = faceNormal + normals[normalInd[i]] * 1.f / 3.f;
					}
					Triangle* triFace = new Triangle(&points[pointInd[0]], &points[pointInd[1]], &points[pointInd[2]], faceNormal,material);
					scene.push_back(triFace);
				}
				if (datas.size() == 4)//四边形面片
				{
					TriNum += 2;
					std::vector<int> pointInd(4);
					std::vector<int> normalInd(4);
					std::vector<int> textureInd(4);//暂时没用

					for (int i = 0; i < 4; ++i)
					{
						std::vector<int> parsedData = parseFaceData(datas[i]);
						pointInd[i] = parsedData[0];
						textureInd[i] = parsedData[1];
						normalInd[i] = parsedData[2];
					}

					vec3f faceNormal1, faceNormal2;
					faceNormal1 = (normals[normalInd[0]] + normals[normalInd[1]] + normals[normalInd[2]])*1.f / 3.f;
					faceNormal2= (normals[normalInd[0]] + normals[normalInd[3]] + normals[normalInd[2]])*1.f / 3.f;
					Triangle* triFace1 = new Triangle(&points[pointInd[0]], &points[pointInd[1]], &points[pointInd[2]], faceNormal1,material);
					scene.push_back(triFace1);
					Triangle* triFace2 = new Triangle(&points[pointInd[0]], &points[pointInd[2]], &points[pointInd[3]], faceNormal2,material);
					scene.push_back(triFace2);
				}
			}
		}

		DEBUGPRINT("load success");
		std::cout << "Facenum:" << TriNum << std::endl;
		in.close();
	}

	std::vector<int> parseFaceData(std::string data) {//data格式需为形如1/2/3或1//2或1/2或1等
		std::vector<std::string> sResult(3);
		std::vector<int> result(3);
		int curInd=0;
		for (int i = 0; i < data.size(); ++i)
		{
			if (data[i] == '/')curInd++;
			else
			{
				sResult[curInd] += data[i];
			}
		}
		for (int i = 0; i < 3; ++i)
		{
			std::istringstream stream(sResult[i]);
			stream >> result[i];
		}
		return result;
	}
};