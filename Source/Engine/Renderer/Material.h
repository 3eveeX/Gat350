#pragma once
#include"Resources/Resource.h"
#include "GUI/GUI.h"
#include <glm/glm.hpp>

class Program;
class Texture;

namespace neu {
	class Material : public Resource, GUI
	{
	public:
		enum class Parameters : uint32_t
		{
			None        = 0,
			BaseMap     = (1 << 0),
			SpecularMap = (1 << 1),
			EmissiveMap = (1 << 2),
			NormalMap   = (1 << 3),
			CubeMap		= (1 << 4),
		};

	public:
		Material() = default;
		~Material() = default;

		bool Load(const std::string& filename);
		void Bind();

		void UpdateGUI() override;

	public:
		float shininess{ 2 };
		glm::vec2 tiling{ 1, 1 };
		glm::vec2 offset{ 0, 0 };


		res_t<Texture> baseMap;
		glm::vec3 baseColor{ 1, 1, 1 };
		res_t<Texture> specularMap;
		res_t<Texture> emissiveMap;
		glm::vec3 emissiveColour{ 0, 0, 0 };
		res_t<Texture> normalMap;
		res_t<Cubemap> cubeMap;
		bool useNormalMap{ false };
		res_t<Program> program;

		float ior{ 1.0f }; // Index of Refraction for materials like glass

		Parameters parameters{ Parameters::None };
	};
}