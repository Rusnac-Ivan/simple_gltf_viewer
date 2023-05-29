#include "Model.h"
#include <fstream>
#include <vector>
#include <utilities/Resources/ResourcesManager.h>
//#include <utilities/Shape/Vertex.h>
#include <ImGuizmo.h>
#include <glm/gtc/type_ptr.hpp>

#ifndef __EMSCRIPTEN__
#ifdef __GNUC__
#define errno_t int
errno_t fopen_s(FILE **f, const char *name, const char *mode)
{
	errno_t ret = 0;
	assert(f);
	*f = fopen(name, mode);
	/* Can't be sure about 1-to-1 mapping of errno and MS' errno_t */
	if (!*f)
		ret = errno;
	return ret;
}
errno_t _wfopen_s(FILE **f, const wchar_t *name, const wchar_t *mode)
{
	errno_t ret = 0;
	assert(f);
	*f = _wfopen(name, mode);
	/* Can't be sure about 1-to-1 mapping of errno and MS' errno_t */
	if (!*f)
		ret = errno;
	return ret;
}
#endif
#endif

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>
#include "Image.h"
#include "Mesh.h"
#include "Material.h"
#include "Node.h"

namespace rsrc
{
	Model::Model() : mIsBinary(false), mIsReady(false)
	{
		mTransform = glm::mat4(1.f);
		mCenter = glm::vec3(0.f);
	}

	Model::~Model()
	{
	}

	void Model::LoadTextures(tinygltf::Model &model)
	{
		mTextures.clear();
		mTextures.resize(model.textures.size());
		for (uint32_t i = 0; i < model.textures.size(); i++)
		{
			tinygltf::Texture &tex = model.textures[i];
			tinygltf::Image &image = model.images[tex.source];
			int sampler_idx = tex.sampler;

			mTextures[i].LoadFromGLTF(image, model.samplers, tex.sampler);
		}
	}

	void Model::LoadMaterials(tinygltf::Model &model)
	{
		mMaterials.clear();
		mMaterials.resize(model.materials.size());
		for (uint32_t i = 0; i < model.materials.size(); i++)
		{
			tinygltf::Material &mat = model.materials[i];
			mMaterials[i].LoadFromTinyGLTF(mat, mTextures);
		}
		// Push a default material at the end of the list for meshes with no material assigned
		mMaterials.push_back(std::move(Material()));
	}

	void Model::LoadFromFile(std::string filename, float scale)
	{
		tinygltf::Model gltfModel;
		tinygltf::TinyGLTF gltfContext;
		std::string error;
		std::string warning;

		bool binary = false;
		size_t extpos = filename.rfind('.', filename.length());
		if (extpos != std::string::npos)
		{
			binary = (filename.substr(extpos + 1, filename.length() - extpos) == "glb");
		}

		bool fileLoaded = false;
		if (binary)
			fileLoaded = gltfContext.LoadBinaryFromFile(&gltfModel, &error, &warning, filename);
		else
			fileLoaded = gltfContext.LoadASCIIFromFile(&gltfModel, &error, &warning, filename);

		if (fileLoaded)
		{
			LoadGltfModel(gltfModel, scale);
		}

		mIsReady = true;
	}

	void Model::Load(util::IResourceManager *manager, Type type, const char *file)
	{
		mIsReady = false;
		std::string fileName = std::string(file);
		mIsBinary = false;
		size_t extpos = fileName.rfind('.', fileName.length());
		if (extpos != std::string::npos)
		{
			mIsBinary = (fileName.substr(extpos + 1, fileName.length() - extpos) == "glb");
		}

		std::vector<Resource::ResData> model = {{0, std::string(file)}};
		Resource::Load(manager, type, model);
	}

	void Model::Load(const char* file_name, unsigned char* data, size_t size)
	{
		mIsReady = false;
		std::string fileName = std::string(file_name);
		mIsBinary = false;
		size_t extpos = fileName.rfind('.', fileName.length());

		size_t pos = fileName.rfind('\\', fileName.length());
		std::string base_dir(file_name, pos + 1);

		if (extpos != std::string::npos)
		{
			mIsBinary = (fileName.substr(extpos + 1, fileName.length() - extpos) == "glb");
		}

		LoadFromMemoryTinyGLTF(base_dir.c_str(), (const uint8_t*)data, size, mIsBinary);
	}

	void Model::OnLoaded(uint32_t key, void *data, unsigned size)
	{
		LoadFromMemoryTinyGLTF("", (const uint8_t*)data, size, mIsBinary);
	}
	void Model::OnError(uint32_t key, int error_code, const char *status)
	{
		fprintf(stderr, "Failed to load model error_code: %d, status: %s\n", error_code, status);
	}
	void Model::OnProgress(uint32_t key, int bytes_loaded, int total_size)
	{
	}

	void Model::LoadFromMemoryTinyGLTF(const char* base_dir, const uint8_t *data, size_t dataSize, bool is_binary, float scale)
	{
		tinygltf::Model gltfModel;
		tinygltf::TinyGLTF gltfContext;
		std::string error;
		std::string warning;

		bool fileLoaded = false;

		if (is_binary)
			fileLoaded = gltfContext.LoadBinaryFromMemory(&gltfModel, &error, &warning, (const unsigned char *)data, dataSize);
		else
			fileLoaded = gltfContext.LoadASCIIFromString(&gltfModel, &error, &warning, (const char *)data, dataSize, base_dir);

		assert(fileLoaded && "TinyGLTF can not load model !");

		if (fileLoaded)
		{
			LoadGltfModel(gltfModel, scale);
		}

		gltfModel.samplers;
		gltfModel.textures;
		gltfModel.materials;
		gltfModel.meshes;
		gltfModel.nodes;



		int a = 0;

		mIsReady = true;
		
	}

	void Model::LoadMeshes(tinygltf::Model& model)
	{
		std::vector<tinygltf::Primitive*> primitives;
		std::vector<std::vector<rsrc::Mesh*>> meshes_map;
		primitives.clear();
		meshes_map.clear();
		meshes_map.resize(model.meshes.size());

		for (uint32_t i = 0; i < model.meshes.size(); i++)
		{
			tinygltf::Mesh& mesh = model.meshes[i];
			for (size_t j = 0; j < mesh.primitives.size(); j++)
			{
				tinygltf::Primitive* p = &mesh.primitives[j];
				primitives.push_back(p);
			}
		}

		mMeshes.clear();
		mMeshes.resize(primitives.size());

		std::vector<uint32_t> indexBuffer;
		std::vector<rsrc::Vertex> vertexBuffer;

		for (uint32_t i = 0; i < primitives.size(); i++)
		{
			mMeshes[i].LoadFromTinyGLTF(model, *primitives[i], mMaterials, indexBuffer, vertexBuffer);
		}


		

		uint32_t idx = 0;
		for (uint32_t i = 0; i < model.meshes.size(); i++)
		{
			tinygltf::Mesh& mesh = model.meshes[i];
			for (size_t j = 0; j < mesh.primitives.size(); j++)
			{
				meshes_map[i].push_back(&mMeshes[idx++]);
			}
		}

		std::map<rsrc::Mesh*, std::vector<glm::mat4>> mesh_matrices_map;

		const tinygltf::Scene& scene = model.scenes[model.defaultScene > -1 ? model.defaultScene : 0];
		mNodes.clear();
		mNodes.resize(scene.nodes.size());
		for (size_t i = 0; i < scene.nodes.size(); i++)
		{
			const tinygltf::Node node = model.nodes[scene.nodes[i]];
			mNodes[i].LoadFromTinyGLTF(nullptr, node, scene.nodes[i], model, meshes_map, mesh_matrices_map);
		}

		/*mOpaqueMeshes.clear();
		mMaskMeshes.clear();
		mBlendMeshes.clear();
		for (uint32_t i = 0; i < mMeshes.size(); i++)
		{
			if (mMeshes[i].GetMaterial()->GetAlphaMode() == rsrc::Material::AlphaMode::OPAQUE)
			{
				mOpaqueMeshes.push_back(&mMeshes[i]);
			}
			else if (mMeshes[i].GetMaterial()->GetAlphaMode() == rsrc::Material::AlphaMode::MASK)
			{
				mMaskMeshes.push_back(&mMeshes[i]);
			}
			else if (mMeshes[i].GetMaterial()->GetAlphaMode() == rsrc::Material::AlphaMode::BLEND)
			{
				mBlendMeshes.push_back(&mMeshes[i]);
			}
		}*/

		mVBO.SetData(sizeof(rsrc::Vertex) * vertexBuffer.size(), vertexBuffer.data());

		mVAO.AddVertexLayout(
			mVBO,
			{
				gl::VertexAttribute::Entry<glm::vec3>(), // position
				gl::VertexAttribute::Entry<glm::vec3>(), // normal
				gl::VertexAttribute::Entry<glm::vec2>(), // uv0 coords
				gl::VertexAttribute::Entry<glm::vec2>() // uv1 coords
			},
			gl::VertexAttributeRate::PER_VERTEX
		);

		//gl::VertexAttribute::Entry<glm::mat4>() // global matrix




		if (indexBuffer.size() > 0)
		{
			mIBO = std::make_unique<gl::IndexBuffer>();
			mIBO->Data(sizeof(uint32_t) * indexBuffer.size(), indexBuffer.data(), gl::DataType::UNSIGNED_INT);
			mVAO.LinkIndexBuffer(*mIBO);
		}

		GetSceneDimensions();
	}

	void Model::LoadGltfModel(tinygltf::Model &gltfModel, float scale)
	{
		const tinygltf::Scene &scene = gltfModel.scenes[gltfModel.defaultScene > -1 ? gltfModel.defaultScene : 0];

		LoadTextures(gltfModel);
		LoadMaterials(gltfModel);
		LoadMeshes(gltfModel);
	}

	void Model::CalculateBoundingBox(Node *node, Node *parent)
	{
		util::AABB parentBvh = parent ? parent->GetBoundingVolumeHierarchy() : util::AABB(dimensions.min, dimensions.max);

		if (node->HaveMesh())
		{
			if (node->GetBoundingBox().IsValid())
			{
				node->GetBoundingBox().SetState(node->GetGlobalMatrix());
				if (node->GetChildrens().size() == 0)
					node->GetBoundingVolumeHierarchy().Align(node->GetBoundingBox().GetMin(), node->GetBoundingBox().GetMax());
			}
		}

		if (node->GetBoundingVolumeHierarchy().IsValid())
		{
			dimensions.min = glm::min(dimensions.min, node->GetBoundingVolumeHierarchy().GetMin());
			dimensions.max = glm::max(dimensions.max, node->GetBoundingVolumeHierarchy().GetMax());
		}

		for (auto &child : node->GetChildrens())
			CalculateBoundingBox(&child, node);
	}

	void Model::GetSceneDimensions()
	{
		dimensions.min = glm::vec3(FLT_MAX);
		dimensions.max = glm::vec3(-FLT_MAX);

		// Calculate binary volume hierarchy for all nodes in the scene
		for (auto &node : mNodes)
		{
			CalculateBoundingBox(&node, nullptr);
		}

		// Calculate scene aabb
		mAABBScale = glm::scale(glm::mat4(1.0f), glm::vec3(dimensions.max[0] - dimensions.min[0], dimensions.max[1] - dimensions.min[1], dimensions.max[2] - dimensions.min[2]));
		mAABBScale[3][0] = dimensions.min[0];
		mAABBScale[3][1] = dimensions.min[1];
		mAABBScale[3][2] = dimensions.min[2];

		mCenter = (dimensions.min + dimensions.max) / 2.f;

		printf("model min: [%.3f, %.3f, %.3f]\n", dimensions.min.x, dimensions.min.y, dimensions.min.z);
		printf("model max: [%.3f, %.3f, %.3f]\n", dimensions.max.x, dimensions.max.y, dimensions.max.z);
		printf("model_center: [%.3f, %.3f, %.3f]\n", mCenter.x, mCenter.y, mCenter.z);

		mTransform = glm::translate(glm::mat4(1.f), -mCenter);
	}

	void Model::OnGUI()
	{
		if (!mIsReady)
			return;

		ImGui::Text("Model");
		char buff[64] = {};
		sprintf(buff, "Min: [ %.3f, %.3f, %.3f]", dimensions.min.x, dimensions.min.y, dimensions.min.z);
		ImGui::Text(buff);
		sprintf(buff, "Max: [ %.3f, %.3f, %.3f]", dimensions.max.x, dimensions.max.y, dimensions.max.z);
		ImGui::Text(buff);

		sprintf(buff, "Width: %.3f m", dimensions.max.x - dimensions.min.x);
		ImGui::Text(buff);
		sprintf(buff, "Height: %.3f m", dimensions.max.y - dimensions.min.y);
		ImGui::Text(buff);
		sprintf(buff, "Depth: %.3f m", dimensions.max.z - dimensions.min.z);
		ImGui::Text(buff);

		/*for (auto& node : mNodes)
		{
			node.OnGUI();
		}*/

		if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Textures"))
			{
				sprintf(buff, "Textures Count: %d", mTextures.size());
				ImGui::Text(buff);

				for (uint32_t i = 0; i < mTextures.size(); i++)
				{
					mTextures[i].OnGUI();
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Materials"))
			{
				sprintf(buff, "Material Count: %d", mMaterials.size());
				ImGui::Text(buff);

				for (uint32_t i = 0; i < mMaterials.size(); i++)
				{
					mMaterials[i].OnGUI();
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Meshes"))
			{
				sprintf(buff, "Meshes Count: %d", mMeshes.size());
				ImGui::Text(buff);
				for (uint32_t i = 0; i < mMeshes.size(); i++)
				{
					mMeshes[i].OnGUI();
				}
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}

	void Model::Draw(util::Rendering::Type render_type, const glm::mat4 &model)
	{
		if (!mIsReady)
			return;

		mModel = model;
		{
			mVAO.Bind();

			for (auto &node : mNodes)
			{
				node.Draw(render_type, mModel * mTransform, rsrc::Material::AlphaMode::OPAQUE);
			}

			for (auto& node : mNodes)
			{
				node.Draw(render_type, mModel * mTransform, rsrc::Material::AlphaMode::MASK);
			}

			for (auto& node : mNodes)
			{
				node.Draw(render_type, mModel * mTransform, rsrc::Material::AlphaMode::BLEND);
			}
		}
	}

	void Model::DrawEditor(const glm::vec4& ray_origin, const glm::vec4& ray_vector, const glm::mat4 &view, const glm::mat4 &proj)
	{
		/*if (mIsEditable)
		{
			{
				// ImGuizmo::Enable(true);

				ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE_X | ImGuizmo::TRANSLATE_Y | ImGuizmo::TRANSLATE_Z;
				//ImGuizmo::OPERATION operation = ImGuizmo::ROTATE_X | ImGuizmo::ROTATE_Y | ImGuizmo::ROTATE_Z;
				glm::vec3 snap = glm::vec3(0.05f, 0.05f, 0.05f);

				glm::mat4 matrix = mTransform * mModel;
				ImGuizmo::SetGizmoSizeClipSpace(0.2f);
				//if (ImGuizmo::Manipulate(glm::value_ptr(ray_origin), glm::value_ptr(ray_vector), glm::value_ptr(view), glm::value_ptr(proj), operation, ImGuizmo::LOCAL, (float*)glm::value_ptr(matrix), NULL, (float*)glm::value_ptr(snap), NULL, NULL))
				//{
					//mTransform = matrix * glm::inverse(mModel);
				//}

				// ImGuizmo::DrawCube(glm::value_ptr(view), glm::value_ptr(proj), mModel);

				//float matrixTranslation[3], matrixRotation[3], matrixScale[3];
				//ImGuizmo::DecomposeMatrixToComponents(gizmoMatrix.m16, matrixTranslation, matrixRotation, matrixScale);
				//ImGui::InputFloat3("Tr", matrixTranslation, 3);
				//ImGui::InputFloat3("Rt", matrixRotation, 3);
				//ImGui::InputFloat3("Sc", matrixScale, 3);
				//ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, gizmoMatrix.m16);
			}
		}*/
	}
}