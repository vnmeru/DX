#pragma once
#include "Engine/Entities/Camera.h"
#include "Engine/Utils/ColorHelper.h"

class Game
{
public:
	Game() {};
	static Game& GetInstance()
	{
		static Game _instance;
		return _instance;
	}
	Camera Camera;
	ColorHelper::HSV BGColorHSV = {0.f,0.f,1.f};
};
