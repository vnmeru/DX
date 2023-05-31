#pragma once
#include "Engine/Entities/Entity.h"

class OasisDirt :public Entity
{
public:
	OasisDirt(XMFLOAT4 Position) : Entity(Position)
	{
		LoadModel("Oasis_Dirt.obj");
	//	SetRotation(90.f, 90.f, 0.f);
		GetModel()->SetTextureName("oasis_Dirt");
	}
};