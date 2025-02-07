#include "shape.h"

ShapeInfo ShapeDict::GetShapeInfo(Shape shape)
{
	ShapeInfo shapeInfo;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	switch (shape)
	{
	case Shape::rect:
		vertices = std::vector<float>{
			-0.5f, -0.5f,
			+0.5f, -0.5f,
			+0.5f, +0.5f,
			-0.5f, +0.5f
		};
		indices = std::vector<unsigned int>{
			0, 1, 3,
			1, 2, 3
		};
		shapeInfo.vertices = vertices;
		shapeInfo.indices = indices;
		break;
	default:
		ASSERT(false);
	}
	return shapeInfo;
}
