#include "Material.h"
#include <tiny_gltf.h>
#include "Image.h"
#include <opengl/Texture2D.h>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <core/Application.h>

namespace rsrc
{
	Material::Material() : 
		mTransparency(1.f),
		mMetallicFactor(0.5f),
		mRoughnessFactor(0.5f),
		mAlphaCutoff(0.5f),
		mBaseColorFactor(0.5),
		mEmissiveFactor(0.f),
		mIOR(1.f)
	{
		for (uint32_t i = 0; i < MapType::COUNT; i++)
		{
			mTexCoordSets[i] = -1;
		}
	}
	Material::~Material() {}

	Material::Material(Material &&other) noexcept : mAlphaMode(other.mAlphaMode),
													mTransparency(other.mTransparency),
													mMetallicFactor(other.mMetallicFactor),
													mRoughnessFactor(other.mRoughnessFactor),
													mAlphaCutoff(other.mAlphaCutoff),
													mBaseColorFactor(other.mBaseColorFactor),
													mEmissiveFactor(other.mEmissiveFactor),
													mExtension(other.mExtension),
													mPbrWorkflows(other.mPbrWorkflows),
													mIOR(other.mIOR),
													mName(other.mName)
	{
		for (uint32_t i = 0; i < MapType::COUNT; i++)
		{
			mAttachedMaps[i] = other.mAttachedMaps[i];
			other.mAttachedMaps[i] = nullptr;
		}
		for (uint32_t i = 0; i < MapType::COUNT; i++)
		{
			mTexCoordSets[i] = other.mTexCoordSets[i];
			other.mTexCoordSets[i] = -1;
		}
		other.mAlphaMode = AlphaMode::OPAQUE;
		other.mTransparency = 1.f;
		other.mMetallicFactor = 0.5f;
		other.mRoughnessFactor = 0.5f;
		other.mAlphaCutoff = 0.5f;
		other.mBaseColorFactor = glm::vec4(0.5f);
		other.mEmissiveFactor = glm::vec4(0.f);
		;
		other.mExtension.diffuseFactor = glm::vec4(0.5f);
		other.mExtension.glossinessFactor = 0.5f;
		other.mExtension.specularFactor = glm::vec3(0.5f);
		other.mPbrWorkflows.metallicRoughness = true;
		other.mPbrWorkflows.specularGlossiness = false;
		other.mIOR = 1.f;
		other.mName = "";
	}

	Material &Material::operator=(Material &&other) noexcept
	{
		mAlphaMode = other.mAlphaMode;
		mTransparency = other.mTransparency;
		mMetallicFactor = other.mMetallicFactor;
		mRoughnessFactor = other.mRoughnessFactor;
		mAlphaCutoff = other.mAlphaCutoff;
		mBaseColorFactor = other.mBaseColorFactor;
		mEmissiveFactor = other.mEmissiveFactor;
		for (uint32_t i = 0; i < MapType::COUNT; i++)
		{
			mAttachedMaps[i] = other.mAttachedMaps[i];
			other.mAttachedMaps[i] = nullptr;
		}
		for (uint32_t i = 0; i < MapType::COUNT; i++)
		{
			mTexCoordSets[i] = other.mTexCoordSets[i];
			other.mTexCoordSets[i] = -1;
		}
		mExtension = other.mExtension;
		mPbrWorkflows = other.mPbrWorkflows;
		mIOR = other.mIOR;
		mName = other.mName;

		other.mAlphaMode = AlphaMode::OPAQUE;
		other.mTransparency = 1.f;
		other.mMetallicFactor = 0.5f;
		other.mRoughnessFactor = 0.5f;
		other.mBaseColorFactor = glm::vec4(0.5f);
		other.mEmissiveFactor = glm::vec4(0.f);
		
		other.mExtension.diffuseFactor = glm::vec4(0.5f);
		other.mExtension.glossinessFactor = 0.5f;
		other.mExtension.specularFactor = glm::vec3(0.5f);
		other.mPbrWorkflows.metallicRoughness = true;
		other.mPbrWorkflows.specularGlossiness = false;
		other.mIOR = 1.f;
		other.mName = "";

		return *this;
	}

	/*void Material::AttachMap(const MapType type, uint8_t* data, size_t width, size_t height, size_t channels, bool isFloatingPoint)
	{
		mAttachedMaps[type].SetData(data, width, height, channels, isFloatingPoint);
	}*/
	void Material::AttachMap(const MapType type, Image *image)
	{
		mAttachedMaps[type] = image;
	}
	void Material::DetachMap(const MapType type)
	{
		mAttachedMaps[type] = nullptr;
	}

	gl::Texture2D* Material::GetBackNormalsTex() { return &mBackNormalsTex; }

	gl::Texture2D *Material::GetTextureByMap(MapType type)
	{
		Image *img = mAttachedMaps[type];
		gl::Texture2D *tex = nullptr;

		if (img)
			tex = img->GetTexture();

		return tex;
	}

	void Material::LoadFromTinyGLTF(tinygltf::Material &mat, std::vector<Image> &textures)
	{
		mName =  mat.name;
		
		if (mat.values.find("baseColorTexture") != mat.values.end())
		{
			uint32_t texture_idx = mat.values["baseColorTexture"].TextureIndex();
			mTexCoordSets[MapType::BASE_COLOR] = mat.values["baseColorTexture"].TextureTexCoord();

			AttachMap(MapType::BASE_COLOR, &(textures[texture_idx]));
		}
		if (mat.values.find("metallicRoughnessTexture") != mat.values.end())
		{
			uint32_t texture_idx = mat.values["metallicRoughnessTexture"].TextureIndex();
			mTexCoordSets[MapType::METALLIC_ROUGHNESS] = mat.values["metallicRoughnessTexture"].TextureTexCoord();

			AttachMap(MapType::METALLIC_ROUGHNESS, &(textures[texture_idx]));
		}
		if (mat.additionalValues.find("normalTexture") != mat.additionalValues.end())
		{
			uint32_t texture_idx = mat.additionalValues["normalTexture"].TextureIndex();
			mTexCoordSets[MapType::NORMAL] = mat.values["normalTexture"].TextureTexCoord();

			AttachMap(MapType::NORMAL, &(textures[texture_idx]));
		}
		if (mat.additionalValues.find("emissiveTexture") != mat.additionalValues.end())
		{
			uint32_t texture_idx = mat.additionalValues["emissiveTexture"].TextureIndex();
			mTexCoordSets[MapType::EMISSIVE] = mat.values["emissiveTexture"].TextureTexCoord();

			AttachMap(MapType::EMISSIVE, &(textures[texture_idx]));
		}
		if (mat.additionalValues.find("occlusionTexture") != mat.additionalValues.end())
		{
			uint32_t texture_idx = mat.additionalValues["occlusionTexture"].TextureIndex();
			mTexCoordSets[MapType::AMBIENT_OCCLUSION] = mat.values["occlusionTexture"].TextureTexCoord();

			AttachMap(MapType::AMBIENT_OCCLUSION, &(textures[texture_idx]));
		}

		if (mat.values.find("roughnessFactor") != mat.values.end())
		{
			mRoughnessFactor = static_cast<float>(mat.values["roughnessFactor"].Factor());
		}
		if (mat.values.find("metallicFactor") != mat.values.end())
		{
			mMetallicFactor = static_cast<float>(mat.values["metallicFactor"].Factor());
		}
		if (mat.values.find("baseColorFactor") != mat.values.end())
		{
			mBaseColorFactor = glm::make_vec4(mat.values["baseColorFactor"].ColorFactor().data());
		}

		if (mat.additionalValues.find("alphaMode") != mat.additionalValues.end())
		{
			tinygltf::Parameter param = mat.additionalValues["alphaMode"];
			if (param.string_value == "BLEND")
			{
				mAlphaMode = AlphaMode::BLEND;
			}
			if (param.string_value == "MASK")
			{
				mAlphaCutoff = 0.5f;
				mAlphaMode = AlphaMode::MASK;
			}
		}

		if (mat.additionalValues.find("alphaCutoff") != mat.additionalValues.end())
		{
			mAlphaCutoff = static_cast<float>(mat.additionalValues["alphaCutoff"].Factor());
		}
		if (mat.additionalValues.find("emissiveFactor") != mat.additionalValues.end())
		{
			mEmissiveFactor = glm::vec4(glm::make_vec3(mat.additionalValues["emissiveFactor"].ColorFactor().data()), 1.0);
		}

		// Extensions
		// @TODO: Find out if there is a nicer way of reading these properties with recent tinygltf headers
		/*if (mat.extensions.find("KHR_materials_pbrSpecularGlossiness") != mat.extensions.end()) {
			auto ext = mat.extensions.find("KHR_materials_pbrSpecularGlossiness");
			if (ext->second.Has("specularGlossinessTexture")) {
				auto index = ext->second.Get("specularGlossinessTexture").Get("index");
				material.extension.specularGlossinessTexture = &textures[index.Get<int>()];
				auto texCoordSet = ext->second.Get("specularGlossinessTexture").Get("texCoord");
				material.texCoordSets.specularGlossiness = texCoordSet.Get<int>();
				material.pbrWorkflows.specularGlossiness = true;
			}
			if (ext->second.Has("diffuseTexture")) {
				auto index = ext->second.Get("diffuseTexture").Get("index");
				material.extension.diffuseTexture = &textures[index.Get<int>()];
			}
			if (ext->second.Has("diffuseFactor")) {
				auto factor = ext->second.Get("diffuseFactor");
				for (uint32_t i = 0; i < factor.ArrayLen(); i++) {
					auto val = factor.Get(i);
					material.extension.diffuseFactor[i] = val.IsNumber() ? (float)val.Get<double>() : (float)val.Get<int>();
				}
			}
			if (ext->second.Has("specularFactor")) {
				auto factor = ext->second.Get("specularFactor");
				for (uint32_t i = 0; i < factor.ArrayLen(); i++) {
					auto val = factor.Get(i);
					material.extension.specularFactor[i] = val.IsNumber() ? (float)val.Get<double>() : (float)val.Get<int>();
				}
			}
		}*/

		if (mat.extensions.find("KHR_materials_ior") != mat.extensions.end()) {
			auto ext = mat.extensions.find("KHR_materials_ior");
			if (ext->second.Has("ior")) {
				auto val = ext->second.Get("ior");
				mIOR = val.IsNumber() ? (float)val.Get<double>() : (float)val.Get<int>();
			}
		}

		if (mat.extensions.find("KHR_materials_specular") != mat.extensions.end()) {
			auto ext = mat.extensions.find("KHR_materials_specular");
			if (ext->second.Has("specularFactor"))
			{
				auto val = ext->second.Get("specularFactor");
				float specular_factor = val.IsNumber() ? (float)val.Get<double>() : (float)val.Get<int>();
			}
			if(ext->second.Has("specularColorFactor"))
			{
				glm::vec3 specularColorFactor = glm::vec3();
				auto factor = ext->second.Get("specularColorFactor");
				for (uint32_t i = 0; i < factor.ArrayLen(); i++) {
					auto val = factor.Get(i);
					specularColorFactor[i] = val.IsNumber() ? (float)val.Get<double>() : (float)val.Get<int>();
				}
			}
			if (ext->second.Has("specularColorTexture"))
			{
				auto colorTex = ext->second.Get("specularColorTexture");
				if (colorTex.Has("index"))
				{
					auto index = colorTex.Get("index");
					int idx = index.IsNumber() ? (float)index.Get<double>() : (float)index.Get<int>();
					int a = 0;
				}
			}
		}

		if (mat.extensions.find("KHR_materials_transmission") != mat.extensions.end()) {

			auto ext = mat.extensions.find("KHR_materials_transmission");
			if (ext->second.Has("transmissionFactor"))
			{
				auto val = ext->second.Get("transmissionFactor");
				float transmission_factor = val.IsNumber() ? (float)val.Get<double>() : (float)val.Get<int>();
			}
			if (ext->second.Has("transmissionTexture"))
			{
				auto transmissionTex = ext->second.Get("transmissionTexture");
				if (transmissionTex.Has("index"))
				{
					auto index = transmissionTex.Get("index");
					int idx = index.IsNumber() ? (float)index.Get<double>() : (float)index.Get<int>();
					int a = 0;
				}
			}
		}

		if (mat.extensions.find("KHR_materials_volume") != mat.extensions.end()) {
			int a = 0;
		}
	}

	void Material::OnGUI()
	{
		if (ImGui::TreeNodeEx(std::string("Material: " + std::to_string((uint64_t)this)).c_str(), ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text(std::string(std::string("Name: ") + mName).c_str());


			if (mAlphaMode == AlphaMode::OPAQUE)
				ImGui::Text("AlphaMode : OPAQUE");
			if (mAlphaMode == AlphaMode::MASK)
				ImGui::Text("AlphaMode : MASK");
			if (mAlphaMode == AlphaMode::BLEND)
				ImGui::Text("AlphaMode : BLEND");



			char buff[64] = {};

			sprintf(buff, "BaseColor: [%.2f, %.2f, %.2f, %.2f]", mBaseColorFactor.x, mBaseColorFactor.y, mBaseColorFactor.z, mBaseColorFactor.w);
			ImGui::Text(buff);

			sprintf(buff, "Roughness: %.2f", mRoughnessFactor);
			ImGui::Text(buff);

			//ImGui::SliderFloat("Roughness", &mRoughnessFactor, 0.f, 1.f, "%.2f");

			sprintf(buff, "Metallic: %.2f", mMetallicFactor);
			ImGui::Text(buff);


			for (uint32_t i = 0; i < MapType::COUNT; i++)
			{
				Image* map = mAttachedMaps[i];
				if (map)
				{
					switch (i)
					{
						case rsrc::Material::BASE_COLOR:
							ImGui::Text("BASE_COLOR_MAP");
							break;
						case rsrc::Material::NORMAL:
							ImGui::Text("NORMAL_MAP");
							break;
						case rsrc::Material::ROUGHNESS:
							ImGui::Text("ROUGHNESS_MAP");
							break;
						case rsrc::Material::METALLIC:
							ImGui::Text("METALLIC_MAP");
							break;
						case rsrc::Material::METALLIC_ROUGHNESS:
							ImGui::Text("METALLIC_ROUGHNESS_MAP");
							break;
						case rsrc::Material::SPECULAR:
							ImGui::Text("SPECULAR_MAP");
							break;
						case rsrc::Material::GLOSSINESS:
							ImGui::Text("GLOSSINESS_MAP");
							break;
						case rsrc::Material::AMBIENT_OCCLUSION:
							ImGui::Text("AMBIENT_MAP");
							break;
						case rsrc::Material::EMISSIVE:
							ImGui::Text("EMISSIVE_MAP");
							break;
						case rsrc::Material::HEIGHT:
							ImGui::Text("HEIGHT_MAP");
							break;
					}

					ImGuiIO& io = ImGui::GetIO();

					ImVec2 img_size = ImVec2(250.f, 250.f * map->GetHeight() / map->GetWidth());
					ImVec2 pos = ImGui::GetCursorScreenPos();
					ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
					ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right

					ImGui::Image((ImTextureID)map->GetTexture()->GetID(), img_size);
					if (ImGui::IsItemHovered())
					{
						ImGui::BeginTooltip();
						float zoom = 4.0f;
						float region_size = 32.0f;
						float region_x = io.MousePos.x - pos.x - region_size * 0.5f;
						float region_y = io.MousePos.y - pos.y - region_size * 0.5f;

						if (region_x < 0.0f)
						{
							region_x = 0.0f;
						}
						else if (region_x > img_size.x - region_size)
						{
							region_x = img_size.x - region_size;
						}
						if (region_y < 0.0f)
						{
							region_y = 0.0f;
						}
						else if (region_y > img_size.y - region_size)
						{
							region_y = img_size.y - region_size;
						}

						ImVec2 uv0 = ImVec2((region_x) / img_size.x, (region_y) / img_size.y);
						ImVec2 uv1 = ImVec2((region_x + region_size) / img_size.x, (region_y + region_size) / img_size.y);

						ImGui::Image((ImTextureID)map->GetTexture()->GetID(), ImVec2(region_size * zoom, region_size * zoom), uv0, uv1);
						ImGui::EndTooltip();
					}

				}
			}

			//ImGui::Image((ImTextureID)mBackNormalsTex.GetID(), ImVec2(core::Application::GetWidth()/4.f, core::Application::GetHeight() / 4.f));


			

			ImGui::TreePop();
		}

		
	}
}