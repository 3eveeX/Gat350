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
		if (material) {
			material->Bind();
			material->program->SetUniform("u_model", owner->transform.GetMatrix());
		}
		glDepthMask(enableDepth);
		glCullFace(cullFace);
		if (model) {
			model->Draw(GL_TRIANGLES);
		}
	}

	void ModelRenderer::DrawShadow(Renderer& renderer, Program& program)
	{
		program.SetUniform("u_model", owner->transform.GetMatrix());

		glDepthMask(enableDepth);
		glCullFace(cullFace); 

		if (model) {
			model->Draw(GL_TRIANGLES);
		}
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
		std::string text;

		text = (model) ? model->name : "ModelRenderer";
		ImGui::Text("Model: %s", text.c_str());
		Editor::GetDialogResource<Model>(model, "ModelDialog", "Open Model", "Model (*.obj; *.fbx; *.glb){.obj,.fbx,.glb},.*");
		
		text = (material) ? material->name : "MaterialRenderer";
		ImGui::Text("Material: %s", text.c_str());
		Editor::GetDialogResource<Material>(material, "MaterialDialog", "Open Material", "Material (*.mat){.mat},.*");

		if (material) {
			material->UpdateGUI();
		}
	}
}	