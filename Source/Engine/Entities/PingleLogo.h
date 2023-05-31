#pragma once
#include "Entity.h"

//The logo belongs to Pingle Game Studio (https://pinglestudio.com/), a company that specializes in game development. 
//The 3D implementation of Pingle Game Studio Logo was made by Edspin (Edward Nmerukini|https://twitter.com/Edspin3), if you want to steal it - leave a credit :)

class PingleLogo : public Entity
{
public:
	PingleLogo(XMFLOAT4 Position);

	bool CanUpdate() const override {return true;}

	void Update() override;
};