#include "shape.h"

ShapeInfo ShapeDict::GetShapeInfo(Shape shape)
{
	ShapeInfo shapeInfo;
	shapeInfo.shape = shape;
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	switch (shape)
	{
	case Shape::S_EMPTY:
		break;
	case Shape::S_RECT:
		vertices = std::vector<float>{
			-0.5f, -0.5f, 0.0f, 0.0f,
			+0.5f, -0.5f, 1.0f, 0.0f,
			+0.5f, +0.5f, 1.0f, 1.0f,
			-0.5f, +0.5f, 0.0f, 1.0f
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
