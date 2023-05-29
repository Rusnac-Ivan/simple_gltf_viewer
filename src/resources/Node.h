#ifndef _RSRC_NODE_H_
#define _RSRC_NODE_H_

#include "Mesh.h"
#include <string>
#include <map>
#include <glm/gtc/quaternion.hpp>
#include <utilities/Renderer/Rendering.h>

namespace tinygltf
{
	class Model;
	class Node;
}

namespace rsrc
{
	class Material;

	class Node
	{
		Node *mParent;
		int32_t mIndex;
		std::vector<Node> mChildrens;

		std::string mName;
		std::vector<Mesh*> mMeshes;

		util::AABB mAABB;
		util::AABB mBVH;

		glm::mat4 mLocalMatrix;
		glm::mat4 mGlobalMatrix;

	public:
		Node();
		~Node();

		Node(const Node &) = delete;
		Node &operator=(const Node &) = delete;
		Node(Node &&) noexcept;
		Node &operator=(Node &&) noexcept;

		const glm::mat4 &GetLocalMatrix() { return mLocalMatrix; }

		util::AABB &GetBoundingBox() { return mAABB; }
		util::AABB &GetBoundingVolumeHierarchy() { return mBVH; }

		bool HaveMesh() { return mMeshes.size() > 0; }
		std::vector<Node> &GetChildrens() { return mChildrens; }

		/*void LoadFromTinyGLTF(Node* parent,
							  const tinygltf::Node &node,
							  std::vector<Material> &materials,
							  std::vector<uint32_t> &indexBuffer,
							  std::vector<rsrc::Vertex> &vertexBuffer,
							  uint32_t nodeIndex,
							  tinygltf::Model &model,
							  float globalscale);*/

		void LoadFromTinyGLTF(Node* parent,
			const tinygltf::Node& node,
			uint32_t nodeIndex,
			tinygltf::Model& model, 
			std::vector<std::vector<rsrc::Mesh*>>& meshes, 
			std::map<rsrc::Mesh*, std::vector<glm::mat4>>& mesh_matrices_map);

		void Draw(util::Rendering::Type render_type, const glm::mat4 &model, rsrc::Material::AlphaMode alpha_mode);

		glm::mat4 GetGlobalMatrix()
		{
			glm::mat4 m = GetLocalMatrix();
			Node *p = mParent;
			while (p)
			{
				m = p->GetLocalMatrix() * m;
				p = p->mParent;
			}
			return m;
		}

		void OnGUI();

		void Update();

	private:
	};

}

#endif