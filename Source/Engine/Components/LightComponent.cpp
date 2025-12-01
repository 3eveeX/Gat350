#include "LightComponent.h"

namespace neu {
	FACTORY_REGISTER(LightComponent)
	void LightComponent::Update(float dt)
	{
		//
	}
	void LightComponent::SetProgram(Program& program, const std::string& name, const glm::mat4& view)
	{
		glm::vec3 position = view * glm::vec4(owner->transform.position, 1 );
		glm::vec3 direction = glm::normalize(glm::mat3(view) * owner->transform.Forward());

		program.SetUniform(name + ".color", color);
		program.SetUniform(name + ".intensity", intensity);
		program.SetUniform(name + ".range", range);
		program.SetUniform(name + ".position", position);
		program.SetUniform(name + ".direction", direction);
		program.SetUniform(name + ".type", static_cast<int>(type));
		program.SetUniform(name + ".outerCutoff", glm::radians(outerCutoff));
		program.SetUniform(name + ".innerCutoff", glm::radians(innerCutoff));
		program.SetUniform(name + ".shadowCaster", shadowCaster);
	}
	void LightComponent::Read(const serial_data_t& value)
	{
		std::string type;
		SERIAL_READ_NAME(value, "lightType", type);
		if(equalsIgnoreCase(type, "point"))
			this->type = LightType::Point;
		else if(equalsIgnoreCase(type, "directional"))
			this->type = LightType::Directional;
		else if(equalsIgnoreCase(type, "spot"))
			this->type = LightType::Spot;

		SERIAL_READ(value, color);
		SERIAL_READ(value, intensity);
		SERIAL_READ(value, range);
		SERIAL_READ(value, outerCutoff);
		SERIAL_READ(value, innerCutoff);

		SERIAL_READ(value, shadowCaster);
	}
	void LightComponent::UpdateGUI()
	{
		const char* types[] = { "Point", "Directional", "Spot" };
		ImGui::Combo("Light Type", (int*)&type, types, IM_ARRAYSIZE(types));

		ImGui::ColorEdit3("Light Color", glm::value_ptr(color));
		ImGui::DragFloat("Light Intensity", &intensity, 0.1f, 0.0f);
		if (type != LightType::Directional) {
			ImGui::DragFloat("Light Range", &range, 0.1f, 0.0f);
		}
		if (type == LightType::Spot) {
			ImGui::DragFloat("Outer Cutoff", &outerCutoff, 0.1f, 0.0f);
			ImGui::DragFloat("Inner Cutoff", &innerCutoff, 0.1f, 0.0f, outerCutoff);
		}

		ImGui::Checkbox("Shadow Caster", &shadowCaster);
	}
}