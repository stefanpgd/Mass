#include "Framework/Editor.h"
#include "Framework/Flux.h"
#include "Framework/ParticleSimulation.h"

#include <imgui.h>
#include <string>

Editor::Editor(Flux* application, const std::vector<ParticleSimulation*>& simulations) 
	: application(application), simulations(simulations)
{
	ImGuiStyleSettings();
}

void Editor::Update(float deltaTime)
{
	// Record FPS //
	frameCount++;
	int index = frameCount % averageFPS.size();
	averageFPS[index] = int(1.0f / deltaTime);

	if(ImGui::BeginMainMenuBar())
	{
		if(ImGui::BeginMenu("Simulation Menu"))
		{

			for(int i = 0; i < simulations.size(); i++)
			{
				if(ImGui::MenuItem(simulations[i]->simulationName.c_str()))
				{
					application->SetParticleSimulation(simulations[i]);
				}
			}

			ImGui::EndMenu();
		}

		ImGui::Separator();

		ImGui::PushFont(boldFont);
		ImGui::Text("Active Simulation: ");
		ImGui::PopFont();
		ImGui::Text(application->activeSimulation->simulationName.c_str());
		
		ImGui::Separator();

		ImGui::PushFont(boldFont);
		ImGui::Text("Particle Count: ");
		ImGui::PopFont();
		ImGui::Text("%i", application->activeSimulation->particleCount);
		
		ImGui::Separator();

		// FPS // 
		ImGui::PushFont(boldFont);
		ImGui::Text("Average FPS:");
		ImGui::PopFont();

		int sum = 0;
		for(int fps : averageFPS)
		{
			sum += fps;
		}
		sum /= 60;

		std::string fps = std::to_string(sum);
		ImGui::Text(fps.c_str());
		ImGui::Separator();

		ImGui::EndMainMenuBar();
	}
}

void Editor::ImGuiStyleSettings()
{
	ImGuiIO& io = ImGui::GetIO();

	// Fonts //	
	io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto-Regular.ttf", 13.f);
	io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto-Bold.ttf", 13.f);

	baseFont = ImGui::GetFont();
	boldFont = io.Fonts->Fonts[1];

	// Style //
	ImGuiStyle& style = ImGui::GetStyle();
	style.ScrollbarRounding = 2;
	style.ScrollbarSize = 12;
	style.WindowRounding = 3;
	style.WindowBorderSize = 0.0f;
	style.WindowTitleAlign = ImVec2(0.0, 0.5f);
	style.WindowPadding = ImVec2(5, 1);
	style.ItemSpacing = ImVec2(12, 5);
	style.FrameBorderSize = 0.5f;
	style.FrameRounding = 3;
	style.GrabMinSize = 5;

	// Color Wheel //
	ImGui::SetColorEditOptions(ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR |
		ImGuiColorEditFlags_PickerHueBar);

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(0.761, 0.761, 0.761, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.118f, 0.118f, 0.118f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.21f, 0.21f, 0.21f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.85f, 0.48f, 0.21f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.85f, 0.48f, 0.21f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.96f, 0.72f, 0.55f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}
