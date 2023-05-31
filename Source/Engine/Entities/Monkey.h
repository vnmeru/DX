#pragma once
#include "Entity.h"

class Monkey :public Entity
{
public:

	Monkey(XMFLOAT4 Position) : Entity(Position)
	{
		if(LoadModel("Monkey.obj", true, { 0.f,0.4f,1.f,1.f }))
		{
			GetModel()->SetRenderMode("wireframe");
		}
		
	}
	bool CanUpdate() const override { return true; }
	void Update() override
	{
		Rotate(0.f, 1.f, 0.f);
	}
};

