#ifndef _RSRC_MESH_H_
#define _RSRC_MESH_H_

#include <glm/glm.hpp>
#include <vector>
#include <opengl/Render.h>
#include <utilities/BoundingObjects/AABB.h>
#include <utilities/Shape/Vertex.h>
#include <resources/Program.h>
#include <utilities/Renderer/Rendering.h>
#include <resources/Material.h>

namespace tinygltf
{
	class Model;
	struct Material;
	struct Primitive;
}

namespace rsrc
{
	class Material;

	struct Vertex
	{
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 uv0;
		glm::vec2 uv1;
	};

	class Mesh
	{
		uint32_t mFirstIndex;
		uint32_t mIndexCount;
		uint32_t mVertexCount;

		gl::Primitive mPrimitiveMode;
		uint32_t mIndexStart;
		uint32_t mVertexStart;

		Material *mMaterial;

		bool mHasIndices;

		// std::vector<util::Vertex> mVertices;
		// std::vector<uint32_t> mIndices;

		util::AABB mAABB;

		gl::FrameBuffer mFBO;

	public:
		Mesh();
		~Mesh();

		Mesh(const Mesh &) = delete;
		Mesh &operator=(const Mesh &) = delete;
		Mesh(Mesh &&) noexcept;
		Mesh &operator=(Mesh &&) noexcept;

		Material* GetMaterial() { return mMaterial; }

		uint32_t GetFirstIndex() const { return mFirstIndex; }
		uint32_t GetIndexCount() const { return mIndexCount; }
		uint32_t GetVertexCount() const { return mVertexCount; }

		const util::AABB &GetBoundingBox() { return mAABB; }

		void OnGUI();

		void LoadFromTinyGLTF(tinygltf::Model &model,
							  const tinygltf::Primitive &primitive,
							  std::vector<Material> &materials,
							  std::vector<uint32_t> &indexBuffer,
							  std::vector<rsrc::Vertex> &vertexBuffer);

		void Draw(util::Rendering::Type render_type, const glm::mat4 &model, rsrc::Material::AlphaMode alpha_mode);
	};

}

#endif // !_RSRC_MESH_H_
