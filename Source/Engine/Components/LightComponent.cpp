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

		program.SetUniform(name + ".color", color);
		program.SetUniform(name + ".position", position);
	}
	void LightComponent::Read(const serial_data_t& value)
	{
		SERIAL_READ(value, color);
	}
	void LightComponent::UpdateGUI()
	{
		ImGui::ColorEdit3("Light Color", glm::value_ptr(color));
	}
}