#pragma once
#include "../Util/util.h"

enum Shape
{
	S_EMPTY,
	S_RECT
};

struct ShapeInfo
{
	Shape shape{Shape::S_EMPTY};
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
};

class ShapeDict
{
public:
	static ShapeInfo GetShapeInfo(Shape shape);
};