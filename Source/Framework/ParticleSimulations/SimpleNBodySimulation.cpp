#include "Framework/ParticleSimulations/SimpleNBodySimulation.h"
#include "Framework/Input.h"

#include "Graphics/Texture.h"
#include "Graphics/DXAccess.h"

// Render Stages //
#include "Graphics/RenderStages/NBodyClearStage.h"
#include "Graphics/RenderStages/SimpleNBodyComputeStage.h"
#include "Graphics/RenderStages/PresentScreenStage.h"

#include <imgui.h>

// TODO: Be able to pass different sizes instead of hard-coding it
SimpleNBodySimulation::SimpleNBodySimulation(int particleCount) : ParticleSimulation(particleCount)
{
	simulationName = "Simple N-Body";
	settings.particleCount = particleCount;

	// Variables relevant to the render stages & compute pipeline(s) //
	unsigned int* textureBuffer = new unsigned int[1024 * 1024];
	renderBuffer = new Texture(textureBuffer, 1024, 1024, DXGI_FORMAT_R8G8B8A8_UNORM, sizeof(unsigned int));

	clearBufferStage = new NBodyClearStage(renderBuffer, &settings.trailStrength, &settings.trailCutoffOpacity);
	nBodyComputeStage = new SimpleNBodyComputeStage(renderBuffer, &settings);
	screenStage = new PresentScreenStage("Source/Shaders/NBody/screen.pixel.hlsl", renderBuffer);
}

void SimpleNBodySimulation::Update(float deltaTime)
{
	settings.deltaTime = deltaTime;
	settings.positionX = ImGui::GetIO().MousePos.x;
	settings.positionY = ImGui::GetIO().MousePos.y;

	if(Input::GetKeyDown(KeyCode::F))
	{
		settings.G *= -1;
	}

	ImGui::Begin("Simplified N-Body Settings");
	ImGui::Text("X:%i , Y:%i", int(settings.positionX), int(settings.positionY));
	ImGui::DragFloat("G", &settings.G, 0.01f, 0.01f, 10.0f);
	ImGui::DragFloat("Max Velocity", &settings.maxVelocity, 0.1f, 0.01f, 500.0f);
	ImGui::DragFloat("Mouse Mass", &settings.mouseMass, 10.0f);
	ImGui::DragFloat("Simulation Speed", &settings.simulationSpeed, 0.001f, 0.00f, 1.0f);

	ImGui::DragFloat("Trail Strength", &settings.trailStrength, 0.001f, 0.0f, 1.0f);
	ImGui::DragFloat("Trail Cutoff Opacity", &settings.trailCutoffOpacity, 0.001f, 0.0f, 1.0f);

	ImGui::Separator();
	ImGui::Text("Particle Count: %i", particleCount);
	ImGui::Text("To Flip Gravity - Press F");

	ImGui::End();
}

void SimpleNBodySimulation::Render(ComPtr<ID3D12GraphicsCommandList2> commandList)
{
	// Execute rendering passes //
	clearBufferStage->RecordStage(commandList);
	nBodyComputeStage->RecordStage(commandList);
	screenStage->RecordStage(commandList);
}