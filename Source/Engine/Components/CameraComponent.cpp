#include "CameraComponent.h"
#include "Engine.h"

namespace neu {
	FACTORY_REGISTER(CameraComponent)

	void CameraComponent::Update(float dt)
	{
		view = glm::lookAt(owner->transform.position, owner->transform.position + owner->transform.Forward(), owner->transform.Up());
		projection = glm::perspective(glm::radians(fov), aspect, near, far);
	}

	void CameraComponent::Clear()
	{
		glClearColor(backgroundColour.r, backgroundColour.g, backgroundColour.b, 1.0f);
		GLbitfield bits = 0;
		if (clearColourBuffer) bits |= GL_COLOR_BUFFER_BIT;	 
		if (clearDepthBuffer) bits |= GL_DEPTH_BUFFER_BIT;

		glClear(bits);
	}

	void CameraComponent::SetPerspective(float fov, float aspect, float near, float far)
	{

		this->fov = fov;
		this->aspect = aspect;
		this->near = near;
		this->far = far;
		
		//projection = glm::perspective(glm::radians(fov), aspect, near, far);
	}
	void CameraComponent::SetLookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up)
	{
		view = glm::lookAt(eye, center, up);
	}
	void CameraComponent::SetProgram(Program& program)
	{
		program.SetUniform("u_view", view);
		program.SetUniform("u_projection", projection);
	}
	void CameraComponent::Read(const serial_data_t& value)
	{
		SERIAL_READ(value, fov);
		if (!SERIAL_READ(value, aspect)) aspect = GetEngine().GetRenderer().GetWidth() / (float)GetEngine().GetRenderer().GetHeight();
		SERIAL_READ(value, near);
		SERIAL_READ(value, far);

		SERIAL_READ(value, clearColourBuffer);
		SERIAL_READ(value, clearDepthBuffer);
		SERIAL_READ(value, backgroundColour);

		std::string outputTextureName;
		SERIAL_READ_NAME(value, "outputTexture", outputTextureName);
		if (!outputTextureName.empty())
		{
			outputTexture = Resources().Get<RenderTexture>(outputTextureName);
		}
	}
	void CameraComponent::UpdateGUI()
	{
		ImGui::DragFloat("FOV", &fov, 0.1f, 10.0f, 100.0f);
		ImGui::InputFloat("Aspect", &aspect, 0.1f);
		ImGui::InputFloat("Near", &near, 0.1f);
		ImGui::InputFloat("Far", &far, 0.1f);

		ImGui::ColorEdit3("Background Colour", glm::value_ptr(backgroundColour));
		ImGui::Checkbox("Clear Colour", &clearColourBuffer);
		ImGui::Checkbox("Clear Depth", &clearDepthBuffer);
	}
}