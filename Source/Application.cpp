#include "Application.h"

#include "Game.h"
#include "Engine/Entities/OasisDirt.h"
#include "Engine/Entities/OasisPalmAndGrass.h"
#include "Engine/Entities/OasisRocks.h"
#include "Engine/Entities/OasisSand.h"
#include "Engine/Entities/OasisWater.h"
#include "Engine/Entities/Box.h"
#include "Engine/Entities/Monkey.h"
#include "Engine/Entities/PingleLogo.h"
#include "Engine/Entities/WilliamSphere.h"
#include "Engine/Renderer/DirectX12.h"
#include "Engine/Utils/ColorHelper.h"
#include "Engine/Utils/Logger.h"
#include "ImGUI/imgui_impl_dx12.h"


LRESULT Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
	{
		return true;
	}
	GameWndProcHandler(hwnd, msg, wParam, lParam);
	static bool bCatch = false;
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) {
			if (MessageBox(0, L"Are you sure you want to exit?",
			               L"Really?", MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				//Running = false;
				DestroyWindow(hwnd);
			}
		}
		if (wParam == VK_F1) {
			bCatch = !bCatch;
		}
		return 0;
	

	case WM_DESTROY: // x button on top right corner of window was pressed
		//Running = false;
		PostQuitMessage(0);
		return 0;

	case WM_SIZE:
		Window::GetInstance().Resize();
		return 0;
		
	}
	return DefWindowProc(hwnd,
	                     msg,
	                     wParam,
	                     lParam);
}

Window& Window::GetInstance()
{
	static Window _instance;
	return _instance;
}

void Window::Resize()
{
	auto& DX = DirectX12::GetInstance();
	RECT clientRect = {};
	::GetClientRect(hwnd, &clientRect);
		
	int width = clientRect.right - clientRect.left;
	int height = clientRect.bottom - clientRect.top;
	DX.Resize(max(1, width), max(1, height));
}

bool Window::InitializeWindow(HINSTANCE hInstance, int ShowWnd, bool Fullscreen)
{
	//TODO: IMPLEMENT FULLSCREEN
	//if (fullscreen)
	//{
	//	HMONITOR hmon = MonitorFromWindow(DX.hwnd,
	//		MONITOR_DEFAULTTONEAREST);
	//	MONITORINFO mi = { sizeof(mi) };
	//	GetMonitorInfo(hmon, &mi);
	//
	//	Width = mi.rcMonitor.right - mi.rcMonitor.left;
	//	Height = mi.rcMonitor.bottom - mi.rcMonitor.top;
	//}

	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"Cringine";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Error registering class",
		           L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	Window::GetInstance().hwnd = CreateWindowEx(NULL,
	                                            L"Cringine",
	                                            L"Cringine",
	                                            WS_OVERLAPPEDWINDOW,
	                                            CW_USEDEFAULT, CW_USEDEFAULT,
	                                            Window::GetInstance().Width, Window::GetInstance().Height,
	                                            NULL,
	                                            NULL,
	                                            hInstance,
	                                            NULL);

	if (!Window::GetInstance().hwnd)
	{
		MessageBox(NULL, L"Error creating window",
		           L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	if (Fullscreen)
	{
		SetWindowLong(Window::GetInstance().hwnd, GWL_STYLE, 0);
	}

	ShowWindow(Window::GetInstance().hwnd, ShowWnd);
	UpdateWindow(Window::GetInstance().hwnd);

	return true;
}

void Application::LoadContent()
{
	logger.Log(INFO,"Loading Content...");
	EntityManager& EManager = EntityManager::GetInstance();
	EManager.ADD_ENTITY(&Game::GetInstance().Camera );

	EManager.ADD_ENTITY(new Box(XMFLOAT4(-1.f, -10.0f, 20.0f, 0.0f)));

	//Lines
	{
		//X
		auto* linex = new Box(XMFLOAT4(0.f, 0.f, 0.0f, 0.0f), true, 1.f, 0.f, 0.f);
		linex->m_Transform.Scale = XMMatrixScaling(999.f, 0.01f, 0.01f);
		EManager.ADD_ENTITY(linex);
		//Y
		auto* liney = new Box(XMFLOAT4(0.f, 0.f, 0.0f, 0.0f), true, 0.f, 1.f, 0.f);
		liney->m_Transform.Scale = XMMatrixScaling(0.01f, 999.f, 0.01f);
		EManager.ADD_ENTITY(liney);
		//Z
		auto* linez = new Box(XMFLOAT4(0.f, 0.f, 0.0f, 0.0f), true, 0.f, 0.f, 1.f);
		linez->m_Transform.Scale = XMMatrixScaling(0.01f, 0.01f, 999.0f);
		EManager.ADD_ENTITY(linez);
	}

	EManager.ADD_ENTITY(new PingleLogo(XMFLOAT4(1.f, 10.0f, 20.0f, 0.0f)));

	EManager.ADD_ENTITY(new PingleLogo(XMFLOAT4(10.f, 10.0f, 20.0f, 0.0f)));

	EManager.ADD_ENTITY(new PingleLogo(XMFLOAT4(1.f, 10.0f, 1.0f, 0.0f)));

	EManager.ADD_ENTITY(new Monkey(XMFLOAT4(1.f, 0.f, 20.0f, 0.0f)));

	EManager.ADD_ENTITY(new OasisDirt(XMFLOAT4(1.f, -1.f, 20.0f, 0.0f)));
	EManager.ADD_ENTITY(new OasisPalmAndGrass(XMFLOAT4(1.f, -1.f, 20.0f, 0.0f)));
	EManager.ADD_ENTITY(new OasisRocks(XMFLOAT4(1.f, -1.f, 20.0f, 0.0f)));
	EManager.ADD_ENTITY(new OasisSand(XMFLOAT4(1.f, -1.f, 20.0f, 0.0f)));
	EManager.ADD_ENTITY(new OasisWater(XMFLOAT4(1.f, -1.f, 20.0f, 0.0f)));

	EManager.ADD_ENTITY(new WilliamSphere(XMFLOAT4(1.f, 5.f, 20.0f, 0.0f)));

	logger.Log(INFO,"Finished loading content...");
}

ImGUIContext& ImGUIContext::GetInstance()
{
	static ImGUIContext _instance;
	return _instance;
}

void ImGUIContext::InitIMGUIDX()
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 2;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	DirectX12::GetInstance().m_pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&DirectX12::GetInstance().m_pDescriptorHeap[DirectX12::HEAP::SRV]));
}

void ImGUIContext::InitIMGUI()
{
	const auto& DX = DirectX12::GetInstance();

	ImGui::CreateContext();
	
	ImGui_ImplWin32_Init(Window::GetInstance().hwnd);
	ImGui_ImplDX12_Init(DX.m_pDevice, DX.frameBufferCount,
		DXGI_FORMAT_R8G8B8A8_UNORM, DX.m_pDescriptorHeap[DirectX12::HEAP::SRV],
		DX.m_pDescriptorHeap[DirectX12::HEAP::SRV]->GetCPUDescriptorHandleForHeapStart(),
		DX.m_pDescriptorHeap[DirectX12::HEAP::SRV]->GetGPUDescriptorHandleForHeapStart());
}

void ImGUIContext::RenderIMGUI()
{
	const auto& DX = DirectX12::GetInstance();

	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	
	const auto& Cam = Game::GetInstance().Camera;
	{
		ImGui::Begin("Info", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text("Press F1 to toggle mouse input!");
		ImGui::End();

		ImGui::Begin("Camera",nullptr,ImGuiWindowFlags_AlwaysAutoResize );
		ImGui::Text("Camera: X=%f Y=%f Z=%f", Cam.m_Transform.Position.x,
			Cam.m_Transform.Position.y,
			Cam.m_Transform.Position.z);
		ImGui::Text("Camera: Pitch=%f Yaw=%f", Cam.camPitch,
			Cam.camYaw);
		{
			auto camx = XMVectorGetX(Cam.cameraTarget);
			auto camy = XMVectorGetY(Cam.cameraTarget);
			auto camz = XMVectorGetZ(Cam.cameraTarget);
			ImGui::Text("Camera Target: X=%f Y=%f Z=%f", camx,camy,camz);
		}
		{
			auto camx = XMVectorGetX(Cam.camForward);
			auto camy = XMVectorGetY(Cam.camForward);
			auto camz = XMVectorGetZ(Cam.camForward);
			ImGui::Text("Camera Forward: X=%f Y=%f Z=%f", camx, camy, camz);
		}
		if (ImGui::InputFloat("FOV", &Game::GetInstance().Camera.m_FOV)) {
			Game::GetInstance().Camera.SetFOV(Game::GetInstance().Camera.m_FOV);
		}
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	
		ColorHelper::RGB RGB = ColorHelper::HSV2RGB(Game::GetInstance().BGColorHSV);
		ImGui::ColorPicker3("Clear",(float*)&RGB);

		Game::GetInstance().BGColorHSV = ColorHelper::RGB2HSV(RGB);

		ImGui::End();
	}

	// Rendering
	ImGui::Render();
	
	DX.m_pCommandList->SetDescriptorHeaps(1, &DX.m_pDescriptorHeap[DirectX12::HEAP::SRV]);

	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DX.m_pCommandList);
}
