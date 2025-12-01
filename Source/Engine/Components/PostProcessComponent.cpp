#include "PostProcessComponent.h"
#include <Engine.h>

namespace neu {
	FACTORY_REGISTER(PostProcessComponent)

	void PostProcessComponent::Update(float dt)	{
	}

	void PostProcessComponent::Apply(Program& program) {
		program.SetUniform("u_parameters", (uint32_t)parameters);
		program.SetUniform("u_colorTint", ColorTint);
		program.SetUniform("u_time", GetEngine().GetTime().GetDeltaTime());
		program.SetUniform("u_blend", blend);
	}

	void PostProcessComponent::Read(const serial_data_t& value) {
		Object::Read(value);
	}

	void PostProcessComponent::UpdateGUI() {
		uint32_t iparameters = (uint32_t)parameters;

		bool parameter = iparameters & (uint32_t)Parameters::Grayscale;
		if (ImGui::Checkbox("Gray Scale", &parameter)) {
			if (parameter) iparameters |= (uint32_t)Parameters::Grayscale;
			else iparameters &= ~(uint32_t)Parameters::Grayscale;
		}

		parameter = iparameters & (uint32_t)Parameters::ColorTint;
		if (ImGui::Checkbox("Color Tint", &parameter)) {
			if (parameter) iparameters |= (uint32_t)Parameters::ColorTint;
			else iparameters &= ~(uint32_t)Parameters::ColorTint;
		}

		parameter = iparameters & (uint32_t)Parameters::ScanLine;
		if (ImGui::Checkbox("Scan Line", &parameter)) {
			if (parameter) iparameters |= (uint32_t)Parameters::ScanLine;
			else iparameters &= ~(uint32_t)Parameters::ScanLine;
		}

		parameter = iparameters & (uint32_t)Parameters::Grain;
		if (ImGui::Checkbox("Grain", &parameter)) {
			if (parameter) iparameters |= (uint32_t)Parameters::Grain;
			else iparameters &= ~(uint32_t)Parameters::Grain;
		}

		parameter = iparameters & (uint32_t)Parameters::Invert;
		if (ImGui::Checkbox("Invert", &parameter)) {
			if (parameter) iparameters |= (uint32_t)Parameters::Invert;
			else iparameters &= ~(uint32_t)Parameters::Invert;
		}
		parameter = iparameters & (uint32_t)Parameters::Outline;
		if (ImGui::Checkbox("Outline", &parameter)) {
			if (parameter) iparameters |= (uint32_t)Parameters::Outline;
			else iparameters &= ~(uint32_t)Parameters::Outline;
		}

			parameters = (Parameters)iparameters;

		ImGui::ColorEdit3("Color Tint", glm::value_ptr(ColorTint));
		ImGui::SliderFloat("Blend", &blend, 0.0f, 1.0f);
	}
}
