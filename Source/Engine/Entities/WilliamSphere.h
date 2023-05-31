#pragma once
#include "Entity.h"

//This entity is a meme to test UV mapping + textures. Pingle, please don't laugh too much :D
class WilliamSphere : public Entity
{
public:
	bool CanUpdate() const override { return true; }

	WilliamSphere(XMFLOAT4 pos, bool bCustomColor = false);
	
	void Update() override;
};
