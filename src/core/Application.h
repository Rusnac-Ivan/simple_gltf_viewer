#ifndef _CORE_APPLICATION_H_
#define _CORE_APPLICATION_H_

#include <cstdlib>
#include <string>

struct GLFWwindow;

namespace core
{

	class Application
	{
	private:
		
		std::string mGLSLVersion;
		GLFWwindow *mGLFWWindow;
		static uint32_t mWidth;
		static uint32_t mHeight;

		bool mIsSync;

		Application(Application &app) = delete;
		Application &operator=(Application &app) = delete;

	protected:
		float mFPS;

		Application();
		virtual ~Application();

		

		virtual bool OnInitialize() { return true; }
		virtual bool OnGui() { return true; }
		virtual bool OnRender() { return true; }
		virtual bool OnFinalize() { return true; }

		virtual void OnMouseLeftDown(double x, double y) {}
		virtual void OnMouseLeftUp(double x, double y) {}
		virtual void OnMouseRightDown(double x, double y) {}
		virtual void OnMouseRightUp(double x, double y) {}
		virtual void OnMouseMiddleDown(double x, double y) {}
		virtual void OnMouseMiddleUp(double x, double y) {}
		virtual void OnMouseMove(double x, double y) {}
		virtual void OnMouseWhell(double offset) {}
		virtual void OnKeyPress(int key) {}
		virtual void OnKeyRepeat(int key) {}
		virtual void OnKeyRelease(int key) {}
		virtual void OnResize(int width, int height) {}

	public:
		int Run(int argc, char **argv, const char *version, uint32_t width, uint32_t height, const char *title);

		static uint32_t GetWidth();
		static uint32_t GetHeight();

	private:
		static void Loop(void *_arg);

		static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
		static void MouseMoveCallback(GLFWwindow *window, double xpos, double ypos);
		static void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
		static void MouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
		static void FramebufferSizeCallback(GLFWwindow *window, int width, int height);
		static void WindowSizeCallback(GLFWwindow *window, int width, int height);
	};

}

#endif