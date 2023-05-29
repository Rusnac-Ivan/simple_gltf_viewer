#include "Rendering.h"
#include <resources/Material.h>
#include <resources/Program.h>
#include <opengl/Pipeline.h>
#include <utilities/Resources/ResourcesManager.h>
#include <core/Application.h>
#include <opengl/Render.h>

namespace util
{
    rsrc::Program *Rendering::mPBRProg = nullptr;
    rsrc::Program *Rendering::mLightMapProg = nullptr;
    rsrc::Program* Rendering::mWParallaxProg = nullptr;

    void Rendering::PrepareLightMap(rsrc::Material *material, const glm::mat4 &model)
    {
        rsrc::Program *light_map_prog = util::ResourcesManager::GetProgramsManager()->GetLightMapProg();
        light_map_prog->Use();

        int base_color_unit = 0;

        if (mLightMapProg != light_map_prog)
        {
            // must set once for program
            light_map_prog->SetUniform(rsrc::Program::UNIFORM_BASE_COLOR_MAP, base_color_unit);
            mLightMapProg = light_map_prog;
        }

        mLightMapProg->SetUniform(rsrc::Program::UNIFORM_MODEL, model);

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

            mLightMapProg->SetUniform(rsrc::Program::UNIFORM_BASE_COLOR_FACTOR, material->GetBaseColorFactor());

            gl::Texture2D *baseColorMap = material->GetTextureByMap(rsrc::Material::MapType::BASE_COLOR);

            light_map_prog->SetUniform(rsrc::Program::UNIFORM_BASE_COLOR_MAP_SET, material->GetTextureCoordSets(rsrc::Material::MapType::BASE_COLOR));
            if (baseColorMap)
                baseColorMap->Activate(base_color_unit);
        }
    }

    void Rendering::PrepareWParallax(rsrc::Material* material, const glm::mat4& model)
    {
        rsrc::Program* w_parallax__prog = util::ResourcesManager::GetProgramsManager()->GetWParallaxProg();
        w_parallax__prog->Use();

        int base_color_unit = 0;

        if (mWParallaxProg != w_parallax__prog)
        {
            // must set once for program
            w_parallax__prog->SetUniform(rsrc::Program::UNIFORM_BASE_COLOR_MAP, base_color_unit);
            mWParallaxProg = w_parallax__prog;
        }

        mWParallaxProg->SetUniform(rsrc::Program::UNIFORM_MODEL, model);

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
            //gl::Pipeline::DisableBlending();
            gl::Pipeline::EnableBlending();


            mWParallaxProg->SetUniform(rsrc::Program::UNIFORM_BASE_COLOR_FACTOR, material->GetBaseColorFactor());

            gl::Texture2D* baseColorMap = material->GetTextureByMap(rsrc::Material::MapType::BASE_COLOR);

            mWParallaxProg->SetUniform(rsrc::Program::UNIFORM_BASE_COLOR_MAP_SET, material->GetTextureCoordSets(rsrc::Material::MapType::BASE_COLOR));
            if (baseColorMap)
                baseColorMap->Activate(base_color_unit);
        }
    }

    void Rendering::PreparePBR(rsrc::Material *material, const glm::mat4 &model)
    {
        rsrc::Program *pbr_prog = util::ResourcesManager::GetProgramsManager()->GetPBRProg();
        pbr_prog->Use();

        int base_color_unit = 0;
        int metallic_roughness_unit = 1;
        int normal_unit = 2;
        int emissive_unit = 3;
        int cubemap_unit = 4;
        //int back_normals_map_unit = 5;

        if (mPBRProg != pbr_prog)
        {
            // must set once for program
            pbr_prog->SetUniform(rsrc::Program::UNIFORM_BASE_COLOR_MAP, base_color_unit);
            pbr_prog->SetUniform(rsrc::Program::UNIFORM_METALLIC_ROUGHNESS_MAP, metallic_roughness_unit);
            pbr_prog->SetUniform(rsrc::Program::UNIFORM_NORMAL_MAP, normal_unit);
            pbr_prog->SetUniform(rsrc::Program::UNIFORM_EMISSIVE_MAP, emissive_unit);
            pbr_prog->SetUniform(rsrc::Program::UNIFORM_CUBEMAP, cubemap_unit);
            //pbr_prog->SetUniform(rsrc::Program::UNIFORM_BACK_NORMALS_MAP, back_normals_map_unit);
            mPBRProg = pbr_prog;
        }

        mPBRProg->SetUniform(rsrc::Program::UNIFORM_MODEL, model);

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

            gl::Texture2D *baseColorMap = material->GetTextureByMap(rsrc::Material::MapType::BASE_COLOR);
            gl::Texture2D *metallicRoughnessMap = material->GetTextureByMap(rsrc::Material::MapType::METALLIC_ROUGHNESS);
            gl::Texture2D *normalMap = material->GetTextureByMap(rsrc::Material::MapType::NORMAL);
            gl::Texture2D *emissiveMap = material->GetTextureByMap(rsrc::Material::MapType::EMISSIVE);

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

            util::ResourcesManager::GetCubeMapsManager()->GetCubeMap(util::CubeMapsManager::BASE)->GetGLCubeMap()->Activate(cubemap_unit);
            
            //material->GetBackNormalsTex()->Activate(back_normals_map_unit);
        }
    }



}