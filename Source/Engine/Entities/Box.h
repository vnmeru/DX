#pragma once
#include <DirectXMath.h>

#include "Entity.h"

class Box : public Entity
{
public:
	bool CanUpdate() const override {return false;}

	Box(XMFLOAT4 pos, bool bCustomColor = false, float r = 0.f, float g = 0.f, float b = 0.f);;
};
