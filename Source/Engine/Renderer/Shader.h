#pragma once

namespace neu
{
	class Shader : public Resource {
	public:
		~Shader();

		bool Load(const std::string& filename, GLuint shaderType);

		void UpdateGUI() override {}

	public:
		GLuint m_shader = 0;
	};
}