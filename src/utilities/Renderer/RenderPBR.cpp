#include "RenderPBR.h"

namespace util
{
	RenderPBR::RenderPBR()
	{
	}

	RenderPBR::~RenderPBR()
	{
	}

	void RenderPBR::Prepare(rsrc::Material* material, const glm::mat4& model)
	{
		rsrc::Program* pbr_prog = util::ResourcesManager::GetProgramsManager()->GetPBRProg();

		pbr_prog->Use();
		int base_color_unit = 0;
		int metallic_roughness_unit = 1;
		int normal_unit = 2;
		int emissive_unit = 3;

		if (mProgram != pbr_prog)
		{
			// must set once
			pbr_prog->SetUniform(rsrc::Program::UNIFORM_BASE_COLOR_MAP, base_color_unit);
			pbr_prog->SetUniform(rsrc::Program::UNIFORM_METALLIC_ROUGHNESS_MAP, metallic_roughness_unit);
			pbr_prog->SetUniform(rsrc::Program::UNIFORM_NORMAL_MAP, normal_unit);
			pbr_prog->SetUniform(rsrc::Program::UNIFORM_EMISSIVE_MAP, emissive_unit);
			mProgram = pbr_prog;
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

			pbr_prog->SetUniform(rsrc::Program::UNIFORM_BASE_COLOR_FACTOR, material->GetBaseColorFactor());
			pbr_prog->SetUniform(rsrc::Program::UNIFORM_METALNESS_FACTOR, material->GetMetallicFactor());
			pbr_prog->SetUniform(rsrc::Program::UNIFORM_ROUGHNESS_FACTOR, material->GetRoughnessFactor());
			pbr_prog->SetUniform(rsrc::Program::UNIFORM_EMISSIVE_FACTOR, material->GetEmissiveFactor());

			gl::Texture2D* baseColorMap = material->GetTextureByMap(rsrc::Material::MapType::BASE_COLOR);
			gl::Texture2D* metallicRoughnessMap = material->GetTextureByMap(rsrc::Material::MapType::METALLIC_ROUGHNESS);
			gl::Texture2D* normalMap = material->GetTextureByMap(rsrc::Material::MapType::NORMAL);
			gl::Texture2D* emissiveMap = material->GetTextureByMap(rsrc::Material::MapType::EMISSIVE);

			pbr_prog->SetUniform(rsrc::Program::UNIFORM_BASE_COLOR_MAP_SET, material->GetTextureCoordSets(rsrc::Material::MapType::BASE_COLOR));
			if (baseColorMap)
				baseColorMap->Activate(base_color_unit);

			pbr_prog->SetUniform(rsrc::Program::UNIFORM_METALLIC_ROUGHNESS_MAP_SET, material->GetTextureCoordSets(rsrc::Material::MapType::METALLIC_ROUGHNESS));
			if (metallicRoughnessMap)
				metallicRoughnessMap->Activate(metallic_roughness_unit);

			pbr_prog->SetUniform(rsrc::Program::UNIFORM_NORMAL_MAP_SET, material->GetTextureCoordSets(rsrc::Material::MapType::NORMAL));
			if (normalMap)
				normalMap->Activate(normal_unit);

			pbr_prog->SetUniform(rsrc::Program::UNIFORM_EMISSIVE_MAP_SET, material->GetTextureCoordSets(rsrc::Material::MapType::EMISSIVE));
			if (emissiveMap)
				emissiveMap->Activate(emissive_unit);
		}
	}
}