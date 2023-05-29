#include "Mesh.h"
#include <utilities/BoundingObjects/AABB.h>
#include <tiny_gltf.h>
#include "Material.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <opengl/Program.h>
#include <opengl/Texture2D.h>
#include <opengl/Pipeline.h>
#include <utilities/Resources/ResourcesManager.h>
#include <imgui.h>
#include <core/Application.h>

namespace rsrc
{
	Mesh::Mesh() : mPrimitiveMode(gl::Primitive::TRIANGLES),
				   mFirstIndex(0),
				   mIndexCount(0),
				   mVertexCount(0),
				   mHasIndices(false)
	{
	}

	Mesh::~Mesh() {}

	Mesh::Mesh(Mesh &&other) noexcept : mPrimitiveMode(other.mPrimitiveMode),
										mFirstIndex(other.mFirstIndex),
										mIndexCount(other.mIndexCount),
										mVertexCount(other.mVertexCount),
										mMaterial(std::move(other.mMaterial)),
										mAABB(std::move(other.mAABB))
	{
	}
	Mesh &Mesh::operator=(Mesh &&other) noexcept
	{
		mPrimitiveMode = other.mPrimitiveMode;
		mFirstIndex = other.mFirstIndex;
		mIndexCount = other.mIndexCount;
		mVertexCount = other.mVertexCount;
		mMaterial = std::move(other.mMaterial);
		mAABB = std::move(other.mAABB);

		return *this;
	}

	static inline int32_t GetTypeSizeInBytes(uint32_t ty)
	{
		if (ty == TINYGLTF_TYPE_SCALAR)
		{
			return 1;
		}
		else if (ty == TINYGLTF_TYPE_VEC2)
		{
			return 2;
		}
		else if (ty == TINYGLTF_TYPE_VEC3)
		{
			return 3;
		}
		else if (ty == TINYGLTF_TYPE_VEC4)
		{
			return 4;
		}
		else if (ty == TINYGLTF_TYPE_MAT2)
		{
			return 4;
		}
		else if (ty == TINYGLTF_TYPE_MAT3)
		{
			return 9;
		}
		else if (ty == TINYGLTF_TYPE_MAT4)
		{
			return 16;
		}
		else
		{
			// Unknown componenty type
			return -1;
		}
	}

	void Mesh::LoadFromTinyGLTF(tinygltf::Model &model,
								const tinygltf::Primitive &primitive,
								std::vector<Material> &materials,
								std::vector<uint32_t> &indexBuffer,
								std::vector<rsrc::Vertex> &vertexBuffer)
	{

		gl::Primitive mode = static_cast<gl::Primitive>(primitive.mode);
		mIndexStart = static_cast<uint32_t>(indexBuffer.size());
		uint32_t vertexStart = static_cast<uint32_t>(vertexBuffer.size());
		mVertexStart = vertexStart;
		uint32_t byteOffset;

		const float *bufferPos = nullptr;
		const float *bufferNorm = nullptr;
		const float *bufferTexCoordSet0 = nullptr;
		const float *bufferTexCoordSet1 = nullptr;

		int posByteStride;
		int normByteStride;
		int uv0ByteStride;
		int uv1ByteStride;

		assert(primitive.attributes.find("POSITION") != primitive.attributes.end());

		const tinygltf::Accessor &posAccessor = model.accessors[primitive.attributes.find("POSITION")->second];
		const tinygltf::BufferView &posView = model.bufferViews[posAccessor.bufferView];
		const tinygltf::Buffer &posBuffer = model.buffers[posView.buffer];

		mVertexCount = static_cast<uint32_t>(posAccessor.count);
		bufferPos = reinterpret_cast<const float *>(&posBuffer.data[posView.byteOffset + posAccessor.byteOffset]);
		posByteStride = posAccessor.ByteStride(posView) ? (posAccessor.ByteStride(posView) / sizeof(float)) : GetTypeSizeInBytes(TINYGLTF_TYPE_VEC3);

		glm::vec3 posMin = glm::vec3(posAccessor.minValues[0], posAccessor.minValues[1], posAccessor.minValues[2]);
		glm::vec3 posMax = glm::vec3(posAccessor.maxValues[0], posAccessor.maxValues[1], posAccessor.maxValues[2]);

		mAABB.Align(posMin, posMax);

		if (primitive.attributes.find("NORMAL") != primitive.attributes.end())
		{
			const tinygltf::Accessor &normAccessor = model.accessors[primitive.attributes.find("NORMAL")->second];
			const tinygltf::BufferView &normView = model.bufferViews[normAccessor.bufferView];
			const tinygltf::Buffer &normBuffer = model.buffers[normView.buffer];

			bufferNorm = reinterpret_cast<const float *>(&normBuffer.data[normView.byteOffset + normAccessor.byteOffset]);
			normByteStride = normAccessor.ByteStride(normView) ? (normAccessor.ByteStride(normView) / sizeof(float)) : GetTypeSizeInBytes(TINYGLTF_TYPE_VEC3);
		}

		if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end())
		{
			const tinygltf::Accessor &texCoordAccessor = model.accessors[primitive.attributes.find("TEXCOORD_0")->second];
			const tinygltf::BufferView &texCoordView = model.bufferViews[texCoordAccessor.bufferView];
			const tinygltf::Buffer &texCoordBuffer = model.buffers[texCoordView.buffer];

			bufferTexCoordSet0 = reinterpret_cast<const float *>(&texCoordBuffer.data[texCoordView.byteOffset + texCoordAccessor.byteOffset]);
			uv0ByteStride = texCoordAccessor.ByteStride(texCoordView) ? (texCoordAccessor.ByteStride(texCoordView) / sizeof(float)) : GetTypeSizeInBytes(TINYGLTF_TYPE_VEC2);
		}
		if (primitive.attributes.find("TEXCOORD_1") != primitive.attributes.end())
		{
			const tinygltf::Accessor &texCoordAccessor = model.accessors[primitive.attributes.find("TEXCOORD_1")->second];
			const tinygltf::BufferView &texCoordView = model.bufferViews[texCoordAccessor.bufferView];
			const tinygltf::Buffer &texCoordBuffer = model.buffers[texCoordView.buffer];

			bufferTexCoordSet1 = reinterpret_cast<const float *>(&texCoordBuffer.data[texCoordView.byteOffset + texCoordAccessor.byteOffset]);
			uv1ByteStride = texCoordAccessor.ByteStride(texCoordView) ? (texCoordAccessor.ByteStride(texCoordView) / sizeof(float)) : GetTypeSizeInBytes(TINYGLTF_TYPE_VEC2);
		}

		for (size_t v = 0; v < posAccessor.count; v++)
		{
			rsrc::Vertex vert{};
			vert.pos = glm::vec4(glm::make_vec3(&bufferPos[v * posByteStride]), 1.0f);
			vert.normal = glm::normalize(glm::vec3(bufferNorm ? glm::make_vec3(&bufferNorm[v * normByteStride]) : glm::vec3(0.0f)));
			vert.uv0 = bufferTexCoordSet0 ? glm::make_vec2(&bufferTexCoordSet0[v * uv0ByteStride]) : glm::vec3(0.0f);
			vert.uv1 = bufferTexCoordSet1 ? glm::make_vec2(&bufferTexCoordSet1[v * uv1ByteStride]) : glm::vec3(0.0f);

			vertexBuffer.push_back(vert);
		}

		if (primitive.indices > -1)
		{
			const tinygltf::Accessor &accessor = model.accessors[primitive.indices > -1 ? primitive.indices : 0];
			const tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
			const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];

			mIndexCount = static_cast<uint32_t>(accessor.count);

			if (mIndexCount > 0)
				mHasIndices = true;

			const void *dataPtr = &(buffer.data[accessor.byteOffset + bufferView.byteOffset]);

			switch (accessor.componentType)
			{
			case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT:
			{
				const uint32_t *buf = static_cast<const uint32_t *>(dataPtr);
				for (size_t index = 0; index < accessor.count; index++)
				{
					indexBuffer.push_back(buf[index] + vertexStart);
				}
				break;
			}
			case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT:
			{
				const uint16_t *buf = static_cast<const uint16_t *>(dataPtr);
				for (size_t index = 0; index < accessor.count; index++)
				{
					indexBuffer.push_back(buf[index] + vertexStart);
				}
				break;
			}
			case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE:
			{
				const uint8_t *buf = static_cast<const uint8_t *>(dataPtr);
				for (size_t index = 0; index < accessor.count; index++)
				{
					indexBuffer.push_back(buf[index] + vertexStart);
				}
				break;
			}
			default:
				std::cerr << "Index component type " << accessor.componentType << " not supported!" << std::endl;
			}
		}

		mMaterial = primitive.material > -1 ? &(materials[primitive.material]) : &(materials.back());


		gl::Texture2D* back_normals_tex = mMaterial->GetBackNormalsTex();
		gl::Texture2D::Sampler sam;
		sam.wrapS = gl::Texture::WrapMode::CLAMP_TO_EDGE;
		sam.wrapT = gl::Texture::WrapMode::CLAMP_TO_EDGE;
		sam.minFilter = gl::Texture::FilterMode::LINEAR;
		sam.magFilter = gl::Texture::FilterMode::LINEAR;
		back_normals_tex->SetSampler(sam);

		mFBO.SetSize((uint32_t)core::Application::GetWidth(), (uint32_t)core::Application::GetHeight());
		mFBO.AttachTexture2D(gl::FrameBuffer::Attachment::COLOR0, back_normals_tex);

		assert(mFBO.IsComplete());
	}

	void Mesh::OnGUI()
	{
		if (ImGui::TreeNodeEx(std::string("Mesh: " + std::to_string((uint64_t)this)).c_str(), ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen))
		{
			char buff[32] = {};
			sprintf(buff, "VertexCount: %d", mVertexCount);
			ImGui::Text(buff);
			sprintf(buff, "IndexCount: %d", mIndexCount);
			ImGui::Text(buff);

			
			//sprintf(buff, "Material: %d", (uint64_t)mMaterial);
			ImGui::Text(std::string( "Material: " + std::to_string((uint64_t)mMaterial)).c_str());

			ImGui::TreePop();
		}
	}

	void Mesh::Draw(util::Rendering::Type render_type, const glm::mat4 &model, rsrc::Material::AlphaMode alpha_mode)
	{
		if (alpha_mode != mMaterial->GetAlphaMode())
			return;

		if (render_type == util::Rendering::Type::LIGHT_MAP)
		{
			util::Rendering::PrepareLightMap(mMaterial, model);
		}
		else if (render_type == util::Rendering::Type::PBR)
		{
			util::Rendering::PreparePBR(mMaterial, model);
		}
		//util::Rendering::PrepareWParallax(mMaterial, model);


		if (mHasIndices)
			gl::Render::DrawIndices(mPrimitiveMode, mIndexCount, gl::DataType::UNSIGNED_INT, mIndexStart);
		else
			gl::Render::DrawVertices(mPrimitiveMode, mVertexCount, mVertexStart);
	}
}