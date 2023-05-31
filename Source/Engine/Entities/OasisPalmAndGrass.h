#pragma once
#include "Engine/Entities/Entity.h"

class OasisPalmAndGrass : public Entity
{
public:
	OasisPalmAndGrass(XMFLOAT4 Position) : Entity(Position)
	{
		LoadModel("Oasis_PalmAndGrass.obj");
		//	SetRotation(90.f, 90.f, 0.f);
		GetModel()->SetTextureName("oasis_PalmTree");
	}
};

