#pragma once
#include "Engine/Entities/Entity.h"

class OasisWater :
    public Entity
{
public:
	OasisWater(XMFLOAT4 Position) : Entity(Position)
	{
		LoadModel("Oasis_Water.obj");
		GetModel()->SetTextureName("oasis_Water");
	}
};

