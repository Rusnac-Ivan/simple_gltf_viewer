#include "Camera.h"
#include <glm/gtx/rotate_vector.hpp>

#define M_PI 3.14159265358979323846f

namespace util
{
	ICamera::ICamera() : mPos(0.f),
		mUp(0.f, 1.f, 0.f),
		mLook(0.f, 0.f, 1.f),
		mOrientation(),
		mPitchYawRoll(0.f),
		mCurrentType(ORBIT),
		mCurrentMotion(NONE),
		mStartScreenPos(glm::vec2(0.f)),
		mFinishScreenPos(glm::vec2(0.f)),
		mStartTime(0.f),
		mCurrentTime(0.f),
		mInertionDuration(0.5f),
		mIsDrag(false),
		mIsPinch(false),
		mWheelStep(0.1f),
		mPinchStep(2.f),
		mCenterOfRotation(glm::vec3(0.f))
	{
		mViewMat = glm::lookAt(mPos, mPos + mLook, mUp);
	}
	ICamera::~ICamera() {}

	void ICamera::SetViewState(const glm::vec3& pos, const glm::vec3& up, const glm::vec3& look)
	{
		mPos = pos;
		mUp = up;
		mLook = look;

		mViewMat = glm::lookAt(mPos, mPos + mLook, mUp);
	}

	void ICamera::SetPosition(const glm::vec3 &pos)
	{
		mPos = pos;
		mViewMat = glm::lookAt(mPos, mPos + mLook, mUp);
	}
	void ICamera::SetOrientation(const glm::vec3 &up, const glm::vec3 &look)
	{
		mUp = up;
		mLook = look;

		mViewMat = glm::lookAt(mPos, mPos + mLook, mUp);
	}
	void ICamera::SetOrientation(const glm::quat &orientation)
	{
		mOrientation = glm::normalize(orientation);

		// R^(-1) == transpose(R), if: R * R^(-1) == I
		const glm::mat4 inv_rot = glm::transpose(glm::toMat4(mOrientation));

		const glm::mat4 inv_tran = glm::translate(glm::mat4(1.f), -mPos);

		// VieMat = (T * R)^(-1) = R^(-1) * T^(-1)
		mViewMat = inv_rot * inv_tran;
	}

	void ICamera::SetOrientation(float pitch, float yaw, float roll)
	{
		// R^(-1) == transpose(R), if: R * R^(-1) == I
		const glm::mat4 inv_rot = glm::transpose(glm::eulerAngleYXZ(yaw, pitch, roll));
		const glm::mat4 test = glm::transpose(glm::yawPitchRoll(yaw, pitch, roll));

		assert(inv_rot == test);

		const glm::mat4 inv_tran = glm::translate(glm::mat4(1.f), -mPos);

		// VieMat = (T * R)^(-1) = R^(-1) * T^(-1)
		mViewMat = inv_rot * inv_tran;
	}


	// mouse event for ABSOLUTEand ORBIT camera
	void ICamera::OnWheel(float val)
	{
		if (val < 0.f)
			mCurrentMotion = FORWARD;
		else if (val > 0.f)
			mCurrentMotion = BACKWARD;

		mStartTime = (double)glfwGetTime();
	}
	void ICamera::OnBeginDrag(float x, float y)
	{
		mIsDrag = true;

		mStartScreenPos.x = x;
		mStartScreenPos.y = y;

		mFinishScreenPos = mStartScreenPos;
	}
	void ICamera::OnDrag(float x, float y)
	{
		if (mIsDrag)
		{
			mStartScreenPos = mFinishScreenPos;

			mFinishScreenPos.x = x;
			mFinishScreenPos.y = y;

			mStartTime = (double)glfwGetTime();
			mCurrentMotion = TURN;
		}
	}
	void ICamera::OnEndDrag()
	{
		mIsDrag = false;
	}
	void ICamera::OnBeginPinch(float x, float y)
	{
		mIsPinch = true;

		mStartScreenPos.x = x;
		mStartScreenPos.y = y;

		mFinishScreenPos = mStartScreenPos;
	}
	void ICamera::OnPinch(float x, float y)
	{
		if (mIsPinch)
		{
			mStartScreenPos = mFinishScreenPos;

			mFinishScreenPos.x = x;
			mFinishScreenPos.y = y;

			mStartTime = (double)glfwGetTime();
			mCurrentMotion = PINCH;
		}
	}
	void ICamera::OnEndPinch()
	{
		mIsPinch = false;
	}

	template<ICamera::Smoothness Degree>
	float ICamera::TimeFactor(float time)
	{
		return ((time - mInertionDuration) / mInertionDuration) * TimeFactor<((ICamera::Smoothness)(Degree - 1))>(time);
	}

	template<>
	float ICamera::TimeFactor<ICamera::Smoothness::ZERO>(float time)
	{
		return 1;
	}

	void ICamera::OnUpdate()
	{
		double curr_time = glfwGetTime();
		//double time_per_frame = ((double)((double)curr_time - mPrevTime) / 1000.0);
		//mPrevTime = curr_time;
		double time = (double)curr_time - mStartTime;
		float time_factor = TimeFactor<CUBIC>(time);

		if (time > mInertionDuration)
		{
			mCurrentMotion = NONE;
		}

		if (mCurrentType == ORBIT)
		{
			switch (mCurrentMotion)
			{
			case FORWARD:
			{
				float move_step = time_factor * mWheelStep; //step is in [0..0.05]

				SetPosition(mPos + mLook * move_step);
			}break;
			case BACKWARD:
			{
				float move_step = time_factor * mWheelStep; //step is in [0..0.05]

				SetPosition(mPos - mLook * move_step);
			}break;
			case TURN:
			{
				float hor_rot_step = -time_factor * (mFinishScreenPos.x - mStartScreenPos.x) * M_PI / 2.f; // [0, PI/2]
				float ver_rot_step = time_factor * (mFinishScreenPos.y - mStartScreenPos.y) * M_PI / 2.f; // [0, PI/2]


				glm::vec3 s{ mViewMat[0][0], mViewMat[1][0], mViewMat[2][0] };
				glm::vec3 u{ 0.f, 1.f, 0.f };
				glm::vec3 f{ -mViewMat[0][2], -mViewMat[1][2], -mViewMat[2][2] };



				glm::vec3 new_pos = glm::rotate(mPos - mCenterOfRotation, hor_rot_step, mUp);
				new_pos = glm::rotate(new_pos, -ver_rot_step, s);
				new_pos = new_pos + mCenterOfRotation;

				glm::vec3 new_f = glm::rotate(f, -ver_rot_step, s);

				float cos_fi = glm::abs(glm::dot(new_f, u));

				/*//vert
				if (cos_fi < 0.8f)
				{
					f = new_f;
					mPos = new_pos;
				}
				else
					mCurrentMotion = NONE;*/

				f = new_f;
				mPos = new_pos;
				//hor
				f = glm::rotate(f, hor_rot_step, u);

				SetOrientation(u, f);
			}break;
			case PINCH:
			{
				float hor_move_step = -time_factor * (mFinishScreenPos.x - mStartScreenPos.x) * mPinchStep; // [0, mPinchStep]
				float ver_move_step = time_factor * (mFinishScreenPos.y - mStartScreenPos.y) * mPinchStep; // [0, mPinchStep]

				glm::vec3 new_pos;

				glm::vec3 s{ mViewMat[0][0],   mViewMat[1][0],     mViewMat[2][0] };
				glm::vec3 u{ mViewMat[0][1],   mViewMat[1][1],     mViewMat[2][1] };
				glm::vec3 f{ -mViewMat[0][2],  -mViewMat[1][2],    -mViewMat[2][2] };

				new_pos = mPos + u * ver_move_step + s * hor_move_step;

				SetPosition(new_pos);
			}break;
			
			case NONE:
			{

			}break;
			}
		}
		else if (mCurrentType == ABSOLUT)
		{
			switch (mCurrentMotion)
			{
			case FORWARD:
			{
				float move_step = time_factor * mWheelStep; //step is in [0..0.05]

				glm::vec3 new_pos;

				glm::vec3 f{ -mViewMat[0][2], -mViewMat[1][2], -mViewMat[2][2] };

				new_pos = mPos + f * move_step;

				SetPosition(new_pos);
			}break;
			case BACKWARD:
			{
				float move_step = time_factor * mWheelStep; //step is in [0..0.05]

				glm::vec3 new_pos;

				glm::vec3 f{ -mViewMat[0][2], -mViewMat[1][2], -mViewMat[2][2] };

				new_pos = mPos - f * move_step;

				SetPosition(new_pos);
			}break;
			case TURN:
			{
				float hor_rot_step = -(mFinishScreenPos.x - mStartScreenPos.x) * time_factor * M_PI / 2.f; // [0, PI/2]
				float ver_rot_step = -(mFinishScreenPos.y - mStartScreenPos.y) * time_factor * M_PI / 2.f; // [0, PI/2]

				glm::vec3 s{ mViewMat[0][0], mViewMat[1][0], mViewMat[2][0] };
				glm::vec3 u{ 0.f, 1.f, 0.f };
				glm::vec3 f{ -mViewMat[0][2], -mViewMat[1][2], -mViewMat[2][2] };

				glm::vec3 new_f = glm::rotate(f, -ver_rot_step, s);

				float cos_fi = std::abs(glm::dot(new_f, u));

				//constexpr float lim = sdt::cos(glm::radians(30.f));

				if (cos_fi < 0.75)
					f = new_f;
				else
					mCurrentMotion = NONE;


				SetOrientation(f, u);
			}break;
			case PINCH:
			{
				float hor_move_step = time_factor * (mFinishScreenPos.x - mStartScreenPos.x) * mPinchStep; // [0, mPinchStep]
				float ver_move_step = -time_factor * (mFinishScreenPos.y - mStartScreenPos.y) * mPinchStep; // [0, mPinchStep]

				glm::vec3 new_pos;

				glm::vec3 s{ mViewMat[0][0],   mViewMat[1][0],     mViewMat[2][0] };
				glm::vec3 u{ mViewMat[0][1],   mViewMat[1][1],     mViewMat[2][1] };
				glm::vec3 f{ -mViewMat[0][2],  -mViewMat[1][2],    -mViewMat[2][2] };

				new_pos = mPos + u * ver_move_step + s * hor_move_step;

				SetPosition(new_pos);
			}break;
			
			case NONE:
			{

			}break;
			}
		}
	}

	Camera<ProjectionType::PERSPECTIVE>::Camera() : mNear(0.01f),
													mFar(100.f),
													mAspectRatio(16.f / 9.f),
													mFOV(60.f)
	{
		mProjectionMat = glm::perspective(glm::radians(mFOV), mAspectRatio, mNear, mFar);
	}

	void Camera<ProjectionType::PERSPECTIVE>::SetState(float near, float far, float aspectRatio, float fov)
	{
		mNear = near;
		mFar = far;
		mAspectRatio = aspectRatio;
		mFOV = fov;

		mProjectionMat = glm::perspective(glm::radians(mFOV), mAspectRatio, mNear, mFar);
	}

	void Camera<ProjectionType::PERSPECTIVE>::SetNear(float near)
	{
		mNear = near;
		mProjectionMat = glm::perspective(glm::radians(mFOV), mAspectRatio, mNear, mFar);
	}
	void Camera<ProjectionType::PERSPECTIVE>::SetFar(float far)
	{
		mFar = far;
		mProjectionMat = glm::perspective(glm::radians(mFOV), mAspectRatio, mNear, mFar);
	}
	void Camera<ProjectionType::PERSPECTIVE>::SetAspectRatio(float aspectRatio)
	{
		mAspectRatio = aspectRatio;
		mProjectionMat = glm::perspective(glm::radians(mFOV), mAspectRatio, mNear, mFar);
	}
	void Camera<ProjectionType::PERSPECTIVE>::SetFOV(float fov)
	{
		mFOV = fov;
		mProjectionMat = glm::perspective(glm::radians(mFOV), mAspectRatio, mNear, mFar);
	}

	Camera<ProjectionType::ORTHOGRAPHIC>::Camera() : mNear(0.01f),
													 mFar(100.f),
													 mTop(10.f),
													 mBottom(-10.f),
													 mLeft(-10.f),
													 mRight(10.f)
	{
		mProjectionMat = glm::ortho(mLeft, mRight, mBottom, mTop, mNear, mFar);
	}

	void Camera<ProjectionType::ORTHOGRAPHIC>::SetState(float near, float far, float top, float bottom, float left, float right)
	{
		mNear = near;
		mFar = far;
		mTop = top;
		mBottom = bottom;
		mLeft = left;
		mRight = right;

		mProjectionMat = glm::ortho(mLeft, mRight, mBottom, mTop, mNear, mFar);
	}

	void Camera<ProjectionType::ORTHOGRAPHIC>::SetNear(float near)
	{
		mNear = near;
		mProjectionMat = glm::ortho(mLeft, mRight, mBottom, mTop, mNear, mFar);
	}
	void Camera<ProjectionType::ORTHOGRAPHIC>::SetFar(float far)
	{
		mFar = far;
		mProjectionMat = glm::ortho(mLeft, mRight, mBottom, mTop, mNear, mFar);
	}
	void Camera<ProjectionType::ORTHOGRAPHIC>::SetTop(float top)
	{
		mTop = top;
		mProjectionMat = glm::ortho(mLeft, mRight, mBottom, mTop, mNear, mFar);
	}
	void Camera<ProjectionType::ORTHOGRAPHIC>::SetBottom(float bottom)
	{
		mBottom = bottom;
		mProjectionMat = glm::ortho(mLeft, mRight, mBottom, mTop, mNear, mFar);
	}
	void Camera<ProjectionType::ORTHOGRAPHIC>::SetLeft(float left)
	{
		mLeft = left;
		mProjectionMat = glm::ortho(mLeft, mRight, mBottom, mTop, mNear, mFar);
	}
	void Camera<ProjectionType::ORTHOGRAPHIC>::SetRight(float right)
	{
		mRight = right;
		mProjectionMat = glm::ortho(mLeft, mRight, mBottom, mTop, mNear, mFar);
	}
}