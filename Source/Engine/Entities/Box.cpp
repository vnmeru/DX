#include "Box.h"

Box::Box(XMFLOAT4 pos, bool bCustomColor, float r, float g, float b): Entity(pos)
{
	XMFLOAT4 Color = { 0.f, 1.f, 0.f, 1.f };

	if(bCustomColor)
	{
		Color = { r,g, b, 1.f };
	}

	LoadModel("cube.obj", true, Color);
}
