#ifndef _UTIL_CONE_H_
#define _UTIL_CONE_H_

#include <vector>
#include "Shape.h"
#include <glm/glm.hpp>

namespace util
{
	template <Attrib::underlying_type Options> // Options = ATTRIB_POS | ATTRIB_NORM | ...
	class Cone : public Shape<Options>
	{
		float mTopRadius;
		float mBottomRadius;
		float mHeight;
		float mWidth;
		float mBegAngle;
		float mEndAngle;
		uint32_t mSectorCount;

	public:
		Cone();
		~Cone();

		float GetWidth() { return mWidth; }
		float GetHeight() { return mHeight; }
		float GetRadius() { return mTopRadius; }

		void Generate(float height, float r0, float r1, uint32_t sectorCount, float angle0 = 0.f, float angle1 = M_PI * 2.f);

		virtual bool IntersectRay(const glm::vec3 &orig, const glm::vec3 &dir, float *distance, float *xp, float *yp);

	private:
		virtual void Generate();
		std::vector<glm::vec3> GetUnitCircleVertices(uint32_t sectorCount, float angle0, float angle1);
	};

	template <Attrib::underlying_type Options> // Options = ATTRIB_POS | ATTRIB_NORM | ...
	Cone<Options>::Cone() : mTopRadius(0.f),
							mBottomRadius(0.f),
							mHeight(0.f),
							mWidth(0.f),
							mBegAngle(0.f),
							mEndAngle(0.f),
							mSectorCount(0)
	{
	}
	template <Attrib::underlying_type Options> // Options = ATTRIB_POS | ATTRIB_NORM | ...
	Cone<Options>::~Cone()
	{
	}

	template <Attrib::underlying_type Options> // Options = ATTRIB_POS | ATTRIB_NORM | ...
	void Cone<Options>::Generate()
	{
		std::vector<glm::vec3> unitCircleVertices = GetUnitCircleVertices(mSectorCount, mBegAngle, mEndAngle);

		// write vertices
		/*for (uint32_t i = 0; i < 2; i++)
		{
			float h = -mHeight / 2.f + mHeight * i;
			float t = i;
			float r = i == 0 ? mBottomRadius : mTopRadius;
			for (uint32_t j = 0; j <= mSectorCount; j++)
			{
				glm::vec3 n = unitCircleVertices[j];
				glm::vec3 p = n * r;

				float u;
				if(mFront == IN_SIDE)
					u = (float)j / mSectorCount;
				else
					u = 1.f - (float)j / mSectorCount;
				float v = t;

				Vertex<Options> vertex;
				if constexpr (Options & Attrib::POS)
					vertex.pos = p;
				if constexpr (Options & Attrib::NORM)
				{
					if(mFront == IN_SIDE)
						vertex.norm = glm::normalize(-n);
					else
						vertex.norm = glm::normalize(n);
				}
				if constexpr (Options & Attrib::UV)
					vertex.uv = glm::vec2(u, v);

				Shape<Options>::mVertices.push_back(vertex);
			}
		}*/
		for (uint32_t j = 0; j <= mSectorCount; j++)
		{
			for (uint32_t i = 0; i < 2; i++)
			{
				float h = -mHeight / 2.f + mHeight * i;
				float t = i;
				float r = i == 0 ? mBottomRadius : mTopRadius;

				glm::vec3 n = unitCircleVertices[j];
				glm::vec3 p = n * r;
				p.y = h;

				float u;
				// if (mFront == Shape<Options>::Front::IN_SIDE)
				u = (float)j / mSectorCount;
				// else
				// u = 1.f - (float)j / mSectorCount;
				float v = t;

				Vertex<Options> vertex;
				if constexpr (Options & Attrib::POS)
					vertex.pos = p;
				if constexpr (Options & Attrib::NORM)
				{
					// if (mFront == Shape<Options>::Front::IN_SIDE)
					vertex.norm = glm::normalize(-n);
					// else
					// vertex.norm = glm::normalize(n);
				}
				if constexpr (Options & Attrib::UV)
					vertex.uv = glm::vec2(u, v);

				Shape<Options>::mVertices.push_back(vertex);
			}
		}

		// write indices
		int j = 0;
		for (uint32_t i = 0; i < mSectorCount; i++)
		{

			// if (mFront == Shape<Options>::Front::IN_SIDE)
			{
				uint32_t idx0 = j;
				uint32_t idx1 = j + 3;
				uint32_t idx2 = j + 1;
				uint32_t idx3 = j;
				uint32_t idx4 = j + 2;
				uint32_t idx5 = j + 3;

				Shape<Options>::mIndices.push_back(idx0);
				Shape<Options>::mIndices.push_back(idx1);
				Shape<Options>::mIndices.push_back(idx2);
				Shape<Options>::mIndices.push_back(idx3);
				Shape<Options>::mIndices.push_back(idx4);
				Shape<Options>::mIndices.push_back(idx5);
			}
			/*else
			{
				uint32_t idx0 = j;
				uint32_t idx1 = j + 1;
				uint32_t idx2 = j + 3;
				uint32_t idx3 = j;
				uint32_t idx4 = j + 3;
				uint32_t idx5 = j + 2;

				Shape<Options>::mIndices.push_back(idx0);
				Shape<Options>::mIndices.push_back(idx1);
				Shape<Options>::mIndices.push_back(idx2);
				Shape<Options>::mIndices.push_back(idx3);
				Shape<Options>::mIndices.push_back(idx4);
				Shape<Options>::mIndices.push_back(idx5);
			}*/
			j += 2;
		}
	}

	template <Attrib::underlying_type Options> // Options = ATTRIB_POS | ATTRIB_NORM | ...
	void Cone<Options>::Generate(float height, float r0, float r1, uint32_t sectorCount, float angle0, float angle1)
	{
		mTopRadius = r0;
		mBottomRadius = r1;
		mHeight = height;
		mBegAngle = angle0;
		mEndAngle = angle1;
		mSectorCount = sectorCount;

		mWidth = mTopRadius * glm::abs(mEndAngle - mBegAngle);

		Generate();

		Shape<Options>::MakeGLVAO();
	}

	template <Attrib::underlying_type Options> // Options = ATTRIB_POS | ATTRIB_NORM | ...
	std::vector<glm::vec3> Cone<Options>::GetUnitCircleVertices(uint32_t sectorCount, float angle0, float angle1)
	{
		float angleStep = (angle1 - angle0) / sectorCount;

		std::vector<glm::vec3> unitCircleVertices;
		for (int i = sectorCount; i >= 0; --i)
		{
			float sectorAngle = angle0 + i * angleStep;
			glm::vec3 unitCircleVert;
			unitCircleVert.x = glm::cos(sectorAngle);
			unitCircleVert.y = 0.f;
			unitCircleVert.z = glm::sin(sectorAngle);

			unitCircleVertices.push_back(glm::normalize(unitCircleVert));
		}
		return unitCircleVertices;
	}

	template <Attrib::underlying_type Options> // Options = ATTRIB_POS | ATTRIB_NORM | ...
	bool Cone<Options>::IntersectRay(const glm::vec3 &O, const glm::vec3 &v, float *distance, float* x, float* y)
	{
		// O - origin of ray, v - vector of ray
		// equation (vx^2 + vy^2) * t^2 + 2*(Ox*vx + Oz*vz) * t + (Ox^2 + Oz^2 - R^2) = 0
		// where R radius of cilinder, t is param in expression P = O + v * t, P - intersect point

		constexpr float eps = 1.19e-7;
		float determinant = 4.f * (mTopRadius * mTopRadius * (v.x * v.x + v.z * v.z) - (O.x * v.z - O.z * v.x) * (O.x * v.z - O.z * v.x));
		if (determinant > 0.f)
		{
			float a = v.x * v.x + v.z * v.z;
			float b = 2.f * (O.x * v.x + O.z * v.z);

			float sqrt_delta = glm::sqrt(determinant);
			float t0 = (-b - sqrt_delta) / (2.f * a);
			float t1 = (-b + sqrt_delta) / (2.f * a);

			glm::vec3 P0 = O + v * t0;
			glm::vec3 P1 = O + v * t1;

			bool is_P0_in_height = P0.y >= -mHeight / 2.f && P0.y <= mHeight / 2.f;
			bool is_P1_in_height = P1.y >= -mHeight / 2.f && P1.y <= mHeight / 2.f;

			float alpha0 = atan2f(P0.z, P0.x);
			float alpha1 = atan2f(P1.z, P1.x);

			bool is_alpha0_in_angle = (glm::abs(mEndAngle - alpha0) <= glm::abs(mEndAngle - mBegAngle)) && (glm::abs(mBegAngle - alpha0) <= glm::abs(mEndAngle - mBegAngle));
			bool is_alpha1_in_angle = (glm::abs(mEndAngle - alpha1) <= glm::abs(mEndAngle - mBegAngle)) && (glm::abs(mBegAngle - alpha1) <= glm::abs(mEndAngle - mBegAngle));

			if (is_P0_in_height && is_alpha0_in_angle)
			{
				*distance = glm::distance(O, P0);
				*x = glm::abs(mEndAngle - alpha0) * mTopRadius;
				*y = P0.y + mHeight / 2.f;
				return true;
			}
			if (is_P1_in_height && is_alpha1_in_angle)
			{
				*distance = glm::distance(O, P1);
				*x = glm::abs(mEndAngle - alpha1) * mTopRadius;
				*y = P1.y + mHeight / 2.f;
				return true;
			}
			return false;
		}
		else if (determinant == 0.f)
		{
			float a = v.x * v.x + v.z * v.z;
			float b = 2.f * (O.x * v.x + O.z * v.z);

			float t = -b / (2.f * a);

			glm::vec3 P = O + v * t;

			bool is_P_in_height = P.y >= -mHeight / 2.f && P.y <= mHeight / 2.f;

			float alpha = atan2f(P.x, P.z);

			bool is_alpha_in_angle = alpha >= mBegAngle && alpha <= mEndAngle;

			if (is_P_in_height && is_alpha_in_angle)
			{
				*distance = glm::distance(O, P);
				*x = glm::abs(mEndAngle - alpha) * mTopRadius;
				*y = P.y + mHeight / 2.f;
				return true;
			}
			return false;
		}
		else
		{
			return false;
		}
	}
}

#endif