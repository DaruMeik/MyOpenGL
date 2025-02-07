#pragma once
#include "../Util/util.h"

struct ShapeInfo
{
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
};

class ShapeDict
{
public:
	enum Shape
	{
		rect
	};
	static ShapeInfo GetShapeInfo(Shape shape);
};