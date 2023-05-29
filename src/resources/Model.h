#ifndef _RSRC_MODEL_H_
#define _RSRC_MODEL_H_

#include <vector>
#include <utilities/BoundingObjects/AABB.h>
#include <opengl/VertexBuffer.h>
#include <opengl/VertexArray.h>
#include <opengl/IndexBuffer.h>
#include <opengl/VertexAttribute.h>
#include <opengl/Texture2D.h>
#include <opengl/Program.h>
#include <string>
#include <memory>
#include <map>
#include <cstdint>
#include "Resource.h"
#include <utilities/Renderer/Rendering.h>

namespace tinygltf
{
	class Model;
}

namespace rsrc
{
	class Image;
	class Material;
	class Mesh;
	class Node;

	class Model : public Resource
	{
		struct Dimensions
		{
			glm::vec3 min = glm::vec3(FLT_MAX);
			glm::vec3 max = glm::vec3(-FLT_MAX);
		} dimensions;

		std::vector<Node> mNodes;
		util::AABB mAABB;

		std::vector<gl::Texture2D::Sampler> mSamplers;
		std::vector<Mesh> mMeshes;
		std::vector<Image> mTextures;
		std::vector<Material> mMaterials;

		gl::VertexBuffer mVBO;
		std::unique_ptr<gl::IndexBuffer> mIBO;
		gl::VertexArray mVAO;

		glm::mat4 mAABBScale;
		glm::mat4 mTransform;
		glm::mat4 mModel;

		std::map<Node*, Mesh*> mOpaqueMeshes;
		std::map<Node*, Mesh*> mMaskMeshes;
		std::map<Node*, Mesh*> mBlendMeshes;



		glm::vec3 mCenter;

		bool mIsBinary;

		bool mIsReady;

		std::string mBaseDir;

	public:
		Model();
		~Model();

		void Load(util::IResourceManager *manager, Type type, const char *file);
		void Load(const char *file_name, unsigned char *data, size_t size);

		bool IsReady() { return mIsReady; }

		void Draw(util::Rendering::Type render_type, const glm::mat4 &model);
		void DrawEditor(const glm::vec4 &ray_origin, const glm::vec4 &ray_vector, const glm::mat4 &view, const glm::mat4 &proj);
		void CalculateBoundingBox(Node *node, Node *parent);
		void GetSceneDimensions();
		glm::vec3 GetCenter() { return mCenter; }

		void OnGUI();

	private:
		void LoadTextures(tinygltf::Model &model);
		void LoadMaterials(tinygltf::Model &model);
		void LoadMeshes(tinygltf::Model &model);

		void LoadFromFile(std::string filename, float scale = 1.0f);
		void LoadFromMemoryTinyGLTF(const char *base_dir, const uint8_t *data, size_t dataSize, bool is_binary, float scale = 1.0f);

		void LoadGltfModel(tinygltf::Model &gltfModel, float scale);

		virtual void OnLoaded(uint32_t key, void *data, unsigned size);
		virtual void OnError(uint32_t key, int error_code, const char *status);
		virtual void OnProgress(uint32_t key, int bytes_loaded, int total_size);
	};

}

#endif
