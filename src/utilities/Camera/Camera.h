#ifndef _UTIL_CAMERA_H_
#define _UTIL_CAMERA_H_

#include <core/Platform.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace util
{
	class ICamera
	{
			enum MotionType
			{
				ABSOLUT,   //priority 2
				ORBIT,      //priority 2
				//AUTONOM,     //priority 1
				STATIC      //priority 0
			};
			enum Motion
			{
				NONE,
				FORWARD,
				BACKWARD,
				TURN,
				PINCH,
				KEYBOARD
			};
			enum Smoothness
			{
				ZERO,
				LINEAR,
				CUBIC,
				QUADRATIC
			};

			//camera states
			//MotionType	mPreviousType;
			MotionType	mCurrentType;
			Motion      mCurrentMotion;

			//save mouse down and up points
			glm::vec2   mStartScreenPos;
			glm::vec2   mFinishScreenPos;

			//save mouse down moment and current time, in second
			double       mStartTime;
			double       mCurrentTime;

			//duration of inertial movment, in second
			float       mInertionDuration;

			bool        mIsDrag;
			bool        mIsPinch;

			double       mPrevTime;

			float       mWheelStep;
			float       mPinchStep;

			glm::vec3   mCenterOfRotation;

			//Position of camera
			glm::vec3 mPos;

			//Orientation by vectors
			glm::vec3 mUp;
			glm::vec3 mLook;

			//Orientation by quaternion or Euler angles
			glm::quat mOrientation;
			glm::vec3 mPitchYawRoll; // pitch -> OX, Yaw -> OY, Roll -> OZ

			glm::mat4 mViewMat;

		public:
			ICamera();
			virtual ~ICamera();

			void SetViewState(const glm::vec3& pos, const glm::vec3& up, const glm::vec3& look);

			void SetPosition(const glm::vec3& pos);
			void SetOrientation(const glm::vec3& up, const glm::vec3& look);
			void SetOrientation(const glm::quat& orientation);
			void SetOrientation(float pitch, float yaw, float roll);


			// mouse event for ABSOLUTEand ORBIT camera
			void OnWheel(float val);
			void OnBeginDrag(float x, float y);
			void OnDrag(float x, float y);
			void OnEndDrag();
			void OnBeginPinch(float x, float y);
			void OnPinch(float x, float y);
			void OnEndPinch();

			void OnUpdate();


			const glm::vec3& GetPosition() const { return mPos; }
			const glm::vec3& GetUp() const { return mUp; }
			const glm::vec3& GetLook() const { return mLook; }
			const glm::quat& GetOrientation() const { return mOrientation; }
			const glm::vec3& GetPitchYawRoll() const { return mPitchYawRoll; }

			const glm::mat4& GetViewMat() { return mViewMat; }

			virtual const glm::mat4& GetProjectionMat() = 0;

	private:
		template<Smoothness Degree>
		float TimeFactor(float time);
	};

	


	enum struct ProjectionType
	{
		ORTHOGRAPHIC,
		PERSPECTIVE
	};

	template<ProjectionType Type>
	class Camera : public ICamera {};


	template<>
	class Camera<ProjectionType::PERSPECTIVE> : public ICamera
	{
		float mNear;
		float mFar;
		float mAspectRatio;
		float mFOV;

		glm::mat4 mProjectionMat;
	public:
		Camera();
		~Camera(){}

		void SetState(float near, float far, float aspectRatio, float fov);

		void SetNear(float near);
		void SetFar(float far);
		void SetAspectRatio(float aspectRatio);
		void SetFOV(float fov);

		float GetNear() const { return mNear; }
		float GetFar() const { return mFar; }
		float GetAspectRatio() const { return mAspectRatio; }
		float GetFOV() const { return mFOV; }

		virtual const glm::mat4& GetProjectionMat() { return mProjectionMat; }

	};


	


	template<>
	class Camera<ProjectionType::ORTHOGRAPHIC> : public ICamera
	{
		float mNear;
		float mFar;
		float mTop;
		float mBottom;
		float mLeft;
		float mRight;

		glm::mat4 mProjectionMat;
	public:
		Camera();
		~Camera(){}

		void SetState(float near, float far, float top, float bottom, float left, float right);

		void SetNear(float near);
		void SetFar(float far);
		void SetTop(float top);
		void SetBottom(float bottom);
		void SetLeft(float left);
		void SetRight(float right);

		float GetNear() const { return mNear; }
		float GetFar() const { return mFar; }
		float GetTop() const { return mTop; }
		float GetBottom() const { return mBottom; }
		float GetLeft() const { return mLeft; }
		float GetRight() const { return mRight; }

		virtual const glm::mat4& GetProjectionMat() { return mProjectionMat; }

	};


	
}

#endif