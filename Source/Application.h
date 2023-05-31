#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <vcruntime_typeinfo.h>
#include <vector>

#include "Engine/Utils/Logger.h"
#include "ImGUI/imgui_impl_win32.h"

struct Entity;
using namespace DirectX; // we will be using the directxmath library


// this is the structure of our constant buffer.
struct ConstantBufferPerObject {
	XMFLOAT4X4 wvpMat;
};
constexpr size_t ConstantBufferPerObjectAlignedSize = (sizeof(ConstantBufferPerObject) + 255) & ~255;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern IMGUI_IMPL_API LRESULT GameWndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define ADD_ENTITY(a) AddEntity(a, typeid(a).name())

class EntityManager
{
public:
	static EntityManager& GetInstance()
	{
		static EntityManager _instance;
		return _instance;
	}
	void AddEntity(Entity* pEntity,const char* type)
	{
		logger.Log(DEBUG,"Adding entity #%i (%s)",Entities.size(),type);
		Entities.emplace_back(pEntity);
	}
	std::vector<Entity*> GetEntites() {return Entities;}
	
private:
	std::vector<Entity*> Entities;
};
class Window
{
public:
	static LRESULT CALLBACK WndProc(HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam);

	static Window& GetInstance();
	void Resize();
	bool InitializeWindow(HINSTANCE hInstance, int ShowWnd, bool Fullscreen);
	int Height = 720;
	int Width = 1280;

	bool FullScreen = false;
	HWND hwnd = NULL;
};
class Application
{
public:
	static Application& GetInstance()
	{
		static Application _instance;
		return _instance;
	}
	bool Running = true;
	void LoadContent();
};

class ImGUIContext
{
public:
	static ImGUIContext& GetInstance();
	void InitIMGUIDX();

	void InitIMGUI();
	void RenderIMGUI();
};