#include <chrono>
#include <vector>
#include <windowsx.h>

#include "Application.h"
#include "Game.h"
#include "stdafx.h"
#include "Engine/Input/InputSystem.h"
#include "Engine/Renderer/DirectX12.h"
#include "ImGUI/imgui.h"

struct Entity;

void Update();
void MainLoop();

int WINAPI WinMain(HINSTANCE hInstance,    //Main windows function
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)

{
	Window::GetInstance().InitializeWindow(hInstance, nShowCmd, false /* TODO: FullScreen!!! */);
	// create the window
	DirectX12::GetInstance().InitD3D();
	ImGUIContext::GetInstance().InitIMGUIDX();
	ImGUIContext::GetInstance().InitIMGUI();
	Application::GetInstance().LoadContent();
	// start the main loop
	MainLoop();

	// we want to wait for the gpu to finish executing the command list before we start releasing everything
	DirectX12::GetInstance().Shutdown();

	return 0;
}

void MainLoop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (Application::GetInstance().Running)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// run game code
			Update(); // update the game logic
			DirectX12::GetInstance().Render(); // execute the command queue (rendering the scene is the result of the gpu executing the command lists)
		}
	}
}


void Update()
{
	// Define a clock and track elapsed time since the last update
	static std::chrono::high_resolution_clock clock;
	static auto t0 = clock.now();
	static double elapsedSeconds = 0.0;
	// Calculate the delta time
	auto t1 = clock.now();
	auto deltaTime = t1 - t0;
	t0 = t1;

	// Update the camera
	Game::GetInstance().Camera.Update();

	// Update all entities with a valid model
	EntityManager& EManger = EntityManager::GetInstance();
	for (Entity* Ent : EManger.GetEntites())
	{
		if (Ent->IsRenderable())
		{
			Ent->UpdateRender();
		}
	}

	// Check if it's time for a game update
	if (elapsedSeconds > 0.016)
	{
		// Update input hold
		InputSystem::GetInstance().UpdateHold();

		// Update all entities that can be updated
		for (Entity* Ent : EManger.GetEntites())
		{
			if (Ent->CanUpdate())
			{
				Ent->Update();
			}
		}

		// Reset elapsed time
		elapsedSeconds = 0.0;
	}

	// Increment elapsed time with delta time
	elapsedSeconds += deltaTime.count() * 1e-9;
}

