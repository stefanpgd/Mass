#include "Framework/Flux.h"
#include "Framework/Renderer.h"
#include "Framework/Editor.h"
#include "Framework/Input.h"
#include "Utilities/Logger.h"

#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#include <cassert>
#include <chrono>

// ImGui //
#include <imgui.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>

#include "Framework/ParticleSimulations/SimpleNBodySimulation.h"
#include "Framework/ParticleSimulations/NBodySimulation.h"
#include "Framework/ParticleSimulations/PhysarumSimulation.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

NBodySimulation* nbodySimulation;
SimpleNBodySimulation* simpleNBodySimulation;

// TODO: Re-add resizing, which includes resizing simulations

Flux::Flux()
{
	RegisterWindowClass();

	renderer = new Renderer(applicationName, 1024, 1024);
	editor = new Editor();

	nbodySimulation = new NBodySimulation(32768);
	simpleNBodySimulation = new SimpleNBodySimulation(63488);
	activeSimulation = new PhysarumSimulation(2000000);

	renderer->SetParticleSimulation(activeSimulation);
}

void Flux::Run()
{
	std::chrono::high_resolution_clock* clock = new std::chrono::high_resolution_clock();
	auto t0 = std::chrono::time_point_cast<std::chrono::milliseconds>((clock->now())).time_since_epoch();;
	float deltaTime = 1.0f;

	MSG msg = {};
	while(runApplication && msg.message != WM_QUIT)
	{
		// DeltaTime //
		auto t1 = std::chrono::time_point_cast<std::chrono::milliseconds>((clock->now())).time_since_epoch();
		deltaTime = (t1 - t0).count() * .001;
		t0 = t1;

		// Window's Callback //
		if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		// Engine Loop //
		Start();
		Update(deltaTime);
		Render();
	}

	// Destroy systems //
}

void Flux::Start()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Flux::Update(float deltaTime)
{
	Input::Update();

	if (Input::GetKeyDown(KeyCode::G))
	{
		// TODO, probably do through some editor window soon-ish 
		// when we've 3 or more simulations
		if (activeSimulation == nbodySimulation)
		{
			activeSimulation = simpleNBodySimulation;
		}
		else
		{
			activeSimulation = nbodySimulation;
		}

		renderer->SetParticleSimulation(activeSimulation);
	}

	editor->Update(deltaTime);
	activeSimulation->Update(deltaTime);

	if(Input::GetKeyDown(KeyCode::Escape))
	{
		runApplication = false;
	}
}

void Flux::Render()
{
	ImGui::Render();
	renderer->Render();
}

void Flux::RegisterWindowClass()
{
	WNDCLASSEXW windowClassDescription = {};
	HINSTANCE hInstance = GetModuleHandle(NULL);

	windowClassDescription.cbSize = sizeof(WNDCLASSEX);
	windowClassDescription.style = CS_HREDRAW | CS_VREDRAW;
	windowClassDescription.lpfnWndProc = &WindowsCallback;
	windowClassDescription.cbClsExtra = 0;
	windowClassDescription.cbWndExtra = 0;
	windowClassDescription.hInstance = hInstance;
	windowClassDescription.hIcon = ::LoadIcon(hInstance, NULL);
	windowClassDescription.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	windowClassDescription.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	windowClassDescription.lpszMenuName = NULL;
	windowClassDescription.lpszClassName = applicationName.c_str();
	windowClassDescription.hIconSm = ::LoadIcon(hInstance, NULL);

	static ATOM atom = ::RegisterClassExW(&windowClassDescription);
	assert(atom > 0);
}

LRESULT Flux::WindowsCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
	}

	// ImGui Windows callback //
	ImGui_ImplWin32_WndProcHandler(hwnd, message, wParam, lParam);

	return ::DefWindowProcW(hwnd, message, wParam, lParam);
}