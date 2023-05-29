#include "RenderLightMap.h"

namespace util
{
	RenderLightMap::RenderLightMap()
	{
	}

	RenderLightMap::~RenderLightMap()
	{
	}

	void RenderLightMap::Prepare(rsrc::Material* material, const glm::mat4& model)
	{
		rsrc::Program* light_map_prog = util::ResourcesManager::GetProgramsManager()->GetLightMapProg();

		int base_color_unit = 0;

		if (mProgram != light_map_prog)
		{
			// must set once
			light_map_prog->SetUniform(rsrc::Program::UNIFORM_BASE_COLOR_MAP, base_color_unit);
			mProgram = light_map_prog;
		}

		mProgram->SetUniform(rsrc::Program::UNIFORM_MODEL, model);

		if (material)
		{
			if (material->GetAlphaMode() == rsrc::Material::AlphaMode::BLEND)
			{
				gl::Pipeline::EnableBlending();
			}
			else if (material->GetAlphaMode() == rsrc::Material::AlphaMode::OPAQUE)
			{
				gl::Pipeline::DisableBlending();
			}

			mProgram->SetUniform(rsrc::Program::UNIFORM_BASE_COLOR_FACTOR, material->GetBaseColorFactor());

			gl::Texture2D* baseColorMap = material->GetTextureByMap(rsrc::Material::MapType::BASE_COLOR);

			light_map_prog->SetUniform(rsrc::Program::UNIFORM_BASE_COLOR_MAP_SET, material->GetTextureCoordSets(rsrc::Material::MapType::BASE_COLOR));
			if (baseColorMap)
				baseColorMap->Activate(base_color_unit);
		}
	}
}