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
	}
	void ModelRenderer::UpdateGUI()
	{
	}
}	