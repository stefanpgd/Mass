#include "Framework/ParticleSimulations/LifeSimulation.h"
#include "Framework/ParticleSimulation.h"

#include "Graphics/Texture.h"
#include "Graphics/DXAccess.h"

#include "Graphics/RenderStages/ClearScreenStage.h"
#include "Graphics/RenderStages/LifeParticleStage.h"
#include "Graphics/RenderStages/ScreenStage.h"

#include <imgui.h>

LifeSimulation::LifeSimulation(unsigned int particleCount)
	: ParticleSimulation(particleCount)
{
	settings.particleCount = particleCount;

	unsigned int* textureBuffer = new unsigned int[1024 * 1024];
	for(int i = 0; i < 1024 * 1024; i++)
	{
		textureBuffer[i] = 0;
	}

	renderBuffer = new Texture(textureBuffer, 1024, 1024, DXGI_FORMAT_R8G8B8A8_UNORM, sizeof(unsigned int));

	clearScreenStage = new ClearScreenStage(renderBuffer);
	screenStage = new ScreenStage("Source/Shaders/Life/screen.pixel.hlsl", renderBuffer);
	particleStage = new LifeParticleStage(renderBuffer, &settings);

	// TODO: Probably instead of doing glm::vec4, instead do floats on CPU
	// and do some extra constant bind calls on GPU
	settings.attractionMatrix[0][0].x = 0.8;
	settings.attractionMatrix[0][1].x = -0.6;
	settings.attractionMatrix[1][0].x = 0.9;
	settings.attractionMatrix[1][1].x = 0.05;
}

void LifeSimulation::Update(float deltaTime)
{
	ImGui::ShowDemoWindow();

	settings.deltaTime = deltaTime;

	ImGui::Begin("Particle Life Simulation Settings");
	ImGui::DragFloat("Particle Scan Distance", &settings.maxDistance, 0.01f, 0.0f, 50.0f);
	ImGui::DragFloat("Particle Friction", &settings.friction, 0.005f, 0.0f, 10.0f);

	ImGui::SeparatorText("Cell Attractions");

	const ImVec4 colors[3]{
		ImVec4(0.0, 0.0, 0.0, 1.0),
		ImVec4(1.0, 0.0, 0.0, 1.0),
		ImVec4(0.0, 1.0, 0.0, 1.0),
	};

	for(int x = 0; x < 3; x++)
	{
		ImGui::PushID(x);

		if(x > 0)
		{
			ImGui::SameLine();
		}

		ImGui::PushStyleColor(ImGuiCol_Header, colors[x]);
		if(ImGui::Selectable("###", true, ImGuiSelectableFlags_Disabled, ImVec2(50, 50)))
		{

		}
		ImGui::PopStyleColor();

		ImGui::PopID();
	}

	static float t = 0.0f;

	for(int y = 0; y < 2; y++)
	{
		for(int x = 0; x < 3; x++)
		{
			ImGui::PushID(y * 2 + x);

			if(x == 0)
			{
				ImGui::PushStyleColor(ImGuiCol_Header, colors[y + 1]);
				ImGui::Selectable("###", true, ImGuiSelectableFlags_Disabled, ImVec2(50, 50));
				ImGui::PopStyleColor();
			}
			else
			{
				float* value = &settings.attractionMatrix[y][x - 1].x;

				ImGui::SameLine();

				glm::vec3 on = glm::vec3(0.2f, 1.0f, 0.2);
				glm::vec3 off = glm::vec3(1.0f, 0.2f, 0.2f);

				float a = (*value * 0.5) + 0.5;
				glm::vec3 color = off * (1.0f - a) + on * a;

				ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(color.x, color.y, color.z, 0.5f));
				ImGui::VSliderFloat("", ImVec2(51, 51), value, -1.0f, 1.0f);
				ImGui::PopStyleColor();
			}

			ImGui::PopID();
		}
	}


	ImGui::End();
}

void LifeSimulation::Render(ComPtr<ID3D12GraphicsCommandList2> commandList)
{
	clearScreenStage->RecordStage(commandList);
	particleStage->RecordStage(commandList);
	screenStage->RecordStage(commandList);
}