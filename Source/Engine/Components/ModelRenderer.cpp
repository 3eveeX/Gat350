#include "EnginePCH.h"
#include "ModelRenderer.h"

namespace neu
{


	FACTORY_REGISTER(ModelRenderer);

	void neu::ModelRenderer::Update(float dt)
	{
	}

	void neu::ModelRenderer::Draw(Renderer& renderer)
	{
		material->Bind();
		material->program->SetUniform("u_model", owner->transform.GetMatrix());

		glDepthMask(enableDepth);
		glCullFace(cullFace);

		model->Draw(GL_TRIANGLES);
	}

	void neu::ModelRenderer::Read(const serial_data_t& value)
	{
		Object::Read(value);

		std::string modelName;
		SERIAL_READ_NAME(value, "model", modelName);

		model = Resources().Get<neu::Model>(modelName);


		std::string materialName;
		SERIAL_READ_NAME(value, "material", materialName);

		material = Resources().Get<neu::Material>(materialName);

		SERIAL_READ(value, enableDepth);

		std::string cullFaceName;
		SERIAL_READ_NAME(value, "cullFace", cullFaceName);
		if (equalsIgnoreCase(cullFaceName, "NONE")) {
			cullFace = GL_NONE;
		}
		else if (equalsIgnoreCase(cullFaceName, "FRONT")) {
			cullFace = GL_FRONT;
		}
		else if (equalsIgnoreCase(cullFaceName, "BACK")) {
			cullFace = GL_BACK;
		}
		else if (equalsIgnoreCase(cullFaceName, "FRONT_AND_BACK")) {
			cullFace = GL_FRONT_AND_BACK;
		}
	}
	void ModelRenderer::UpdateGUI()
	{
		if (model) {
			ImGui::Text("Model: %s", model->name.c_str());
			Editor::GetDialogResource<Model>(model, "ModelDialog", "Open Model", "Model (*.obj; *.fbx){.obj,.fbx},.*");
		}
		if (material) {
			ImGui::Text("Material: %s", material->name.c_str());
			Editor::GetDialogResource<Material>(material, "MaterialDialog", "Open Material", "Material (*.mat){.mat},.*");

			material->UpdateGUI();
		}
	}
}	