#pragma once
#include "Engine/Entities/Entity.h"

class OasisSand :
    public Entity
{
public:
	OasisSand(XMFLOAT4 Position) : Entity(Position)
	{
		LoadModel("Oasis_Sand.obj");
		//	SetRotation(90.f, 90.f, 0.f);
		GetModel()->SetTextureName("oasis_Sand");
	}
};

