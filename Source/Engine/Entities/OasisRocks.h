#pragma once
#include "Engine/Entities/Entity.h"

class OasisRocks :
    public Entity
{
public:
	OasisRocks(XMFLOAT4 Position) : Entity(Position)
	{
		LoadModel("Oasis_Rocks.obj");
		//	SetRotation(90.f, 90.f, 0.f);
		GetModel()->SetTextureName("oasis_Rock");
	}
};

