#pragma once

namespace neu {
	class LightComponent : public Component
	{
	public:
		enum LightType
		{
			Point,
			Directional,
			Spot
		};
	public:
		CLASS_PROTOTYPE(LightComponent)

		void Update(float dt) override;
		void SetProgram(Program& program, const std::string& name, const glm::mat4& view);

		void Read(const serial_data_t& value) override;
		void UpdateGUI() override;

	public:
		glm::vec3 color{ 1, 1, 1 };
		float intensity{ 1.0f };
		float range{ 20.0f };
		LightType type{ LightType::Point };
		float outerCutoff{ 15.0f }; // for spot light
		float innerCutoff{ 12.5f }; // for spot light
	};
}