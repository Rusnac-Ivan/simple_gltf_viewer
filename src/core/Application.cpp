#include "Application.h"

#include "Platform.h"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include "fa_solid_900.h"
#include "font_awesome_5.h"
#include "helvetica.inl"
#include "courier_new.inl"
#include "courier_new_regular.inl"
#include "courier_new_bold.inl"
#include <utilities/Resources/ResourcesManager.h>
#include <opengl/Render.h>
#include <utilities/imgui_tools/imgui_util.h>
#include <implot.h>
#include <iostream>

namespace core
{
	static Application *thiz = nullptr;

	uint32_t Application::mWidth;
	uint32_t Application::mHeight;

	uint32_t Application::GetWidth() { return mWidth; }
	uint32_t Application::GetHeight() { return mHeight; }

	void Application::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (ImGui::GetIO().WantCaptureKeyboard || ImGui::GetIO().WantTextInput)
			return;

		if (action == GLFW_PRESS)
			thiz->OnKeyPress(key);
		if (action == GLFW_REPEAT)
			thiz->OnKeyRepeat(key);
		if (action == GLFW_RELEASE)
			thiz->OnKeyRelease(key);
	}

	void Application::MouseMoveCallback(GLFWwindow *window, double xpos, double ypos)
	{
		if (ImGui::GetIO().WantCaptureMouse)
			return;

		thiz->OnMouseMove(xpos, ypos);
	}

	void Application::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
	{
		if (ImGui::GetIO().WantCaptureMouse)
			return;

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		if (action == GLFW_PRESS)
		{
			if (button == GLFW_MOUSE_BUTTON_LEFT)
			{
				thiz->OnMouseLeftDown(xpos, ypos);
			}
			else if (button == GLFW_MOUSE_BUTTON_RIGHT)
			{
				thiz->OnMouseRightDown(xpos, ypos);
			}
			else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
			{
				thiz->OnMouseMiddleDown(xpos, ypos);
			}
		}
		else if (action == GLFW_RELEASE)
		{
			if (button == GLFW_MOUSE_BUTTON_LEFT)
			{
				thiz->OnMouseLeftUp(xpos, ypos);
			}
			else if (button == GLFW_MOUSE_BUTTON_RIGHT)
			{
				thiz->OnMouseRightUp(xpos, ypos);
			}
			else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
			{
				thiz->OnMouseMiddleUp(xpos, ypos);
			}
		}
	}

	void Application::MouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
	{
		if (ImGui::GetIO().WantCaptureMouse)
			return;

		thiz->OnMouseWhell(yoffset);
	}

	void Application::FramebufferSizeCallback(GLFWwindow *window, int width, int height)
	{
	}

	void Application::WindowSizeCallback(GLFWwindow *window, int width, int height)
	{
		thiz->mWidth = width;
		thiz->mHeight = height;
		thiz->OnResize(width, height);
	}

	Application::Application() : mIsSync(false)
	{
		thiz = this;
	}

	Application::~Application()
	{
		// Cleanup imgui
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		// Destroy glfw
		glfwDestroyWindow(mGLFWWindow);
		glfwTerminate();
	}

	static void APIENTRY glDebugOutput(GLenum source,
		GLenum type,
		unsigned int id,
		GLenum severity,
		GLsizei length,
		const char* message,
		const void* userParam)
	{
		// ignore non-significant error/warning codes
		if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

		std::cout << "---------------" << std::endl;
		std::cout << "Debug message (" << id << "): " << message << std::endl;

		switch (source)
		{
		case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
		case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
		} std::cout << std::endl;

		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
		case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
		case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
		case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
		case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
		} std::cout << std::endl;

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
		case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
		} std::cout << std::endl;
		std::cout << std::endl;
	}


	int Application::Run(int argc, char **argv, const char *version, uint32_t width, uint32_t height, const char *title)
	{
		mWidth = width;
		mHeight = height;

		if (!glfwInit())
			return EXIT_FAILURE;

			// Decide GL+GLSL versions
#ifdef __APPLE__
		// GL 3.2 + GLSL 150
		mGLSLVersion = "#version 150";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);		   // Required on Mac
#elif __EMSCRIPTEN__
		mGLSLVersion = "#version 300 es";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#else
		mGLSLVersion = "#version 400";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
#endif

		//glfwWindowHint(GLFW_SCALE_TO_MONITOR, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_SAMPLES, 8);

		glfwWindowHint(GLFW_RESIZABLE, 1);

#ifdef __EMSCRIPTEN__
		int canv_width, canv_height;
		if (emscripten_get_canvas_element_size(".emscripten", &canv_width, &canv_height) != EMSCRIPTEN_RESULT_SUCCESS)
			return EXIT_FAILURE;

		mWidth = canv_width;
		mHeight = canv_height;
#endif // __EMSCRIPTEN__

		/*GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		float xscale, yscale;
		glfwGetMonitorContentScale(monitor, &xscale, &yscale);*/
		
		// Create window with graphics context
		mGLFWWindow = glfwCreateWindow(mWidth, mHeight, title, NULL, NULL);
		if (mGLFWWindow == NULL)
			return EXIT_FAILURE;

		

		glfwMakeContextCurrent(mGLFWWindow);

#ifndef __EMSCRIPTEN__
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			fprintf(stderr, "Failed to initialize GLAD!\n");
			exit(1);
		}
#endif

#ifndef __EMSCRIPTEN__
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(mGLFWWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
#endif

		glfwSetKeyCallback(mGLFWWindow, Application::KeyCallback);
		glfwSetCursorPosCallback(mGLFWWindow, Application::MouseMoveCallback);
		glfwSetMouseButtonCallback(mGLFWWindow, Application::MouseButtonCallback);
		glfwSetScrollCallback(mGLFWWindow, Application::MouseScrollCallback);
#ifndef __EMSCRIPTEN__
		glfwSetFramebufferSizeCallback(mGLFWWindow, Application::FramebufferSizeCallback);
		glfwSetWindowSizeCallback(mGLFWWindow, Application::WindowSizeCallback);
#endif

		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);


		int flags; 
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		//if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			GL(Enable(GL_DEBUG_OUTPUT));
			GL(Enable(GL_DEBUG_OUTPUT_SYNCHRONOUS));
			GL(DebugMessageCallback(glDebugOutput, nullptr));
			GL(DebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE));
		}
		// Initialize sample.
		bool success = OnInitialize();

		if (!success)
			return EXIT_FAILURE;

#ifdef __EMSCRIPTEN__

		if (emscripten_get_canvas_element_size(".emscripten", &canv_width, &canv_height) != EMSCRIPTEN_RESULT_SUCCESS)
			return EXIT_FAILURE;

		printf("canvas_element_size %dx%d\n", canv_width, canv_height);

		mWidth = canv_width;
		mHeight = canv_height;

		glfwSetWindowSize(mGLFWWindow, mWidth, mHeight);
#endif
		OnResize(mWidth, mHeight);

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO &io = ImGui::GetIO();



#ifndef __EMSCRIPTEN__
		float _xscale, _yscale;
		glfwGetWindowContentScale(mGLFWWindow, &_xscale, &_yscale);

		_xscale = 1.f;
		_yscale = 1.f;
		io.DisplayFramebufferScale = ImVec2(_xscale, _yscale);
		//io.FontGlobalScale = _yscale;
#endif

		//io.DisplayFramebufferScale = ImVec2(2.5f, 2.5f);
		//io.FontGlobalScale = 2.5f;
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		// ImGui::StyleColorsClassic();
		ImGui::StyleColors();

		float font_size = 17.f;

		ImFontConfig font_cfg;
		font_cfg.FontDataOwnedByAtlas = false;
		io.Fonts->AddFontFromMemoryTTF(__courier_new_bold_ttf, __courier_new_bold_ttf_len, font_size, &font_cfg);

		static const ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
		ImFontConfig icons_config;
		icons_config.MergeMode = true;
		icons_config.PixelSnapH = true;
		icons_config.GlyphMinAdvanceX = font_size; // Use if you want to make the icon monospaced
		icons_config.FontDataOwnedByAtlas = false;
		io.Fonts->AddFontFromMemoryTTF((void *)fa_solid_900, sizeof(fa_solid_900), font_size, &icons_config, icons_ranges);

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(mGLFWWindow, true);
		ImGui_ImplOpenGL3_Init(mGLSLVersion.c_str());

		ImPlotContext* implot_ctx = ImPlot::CreateContext();
		if (implot_ctx == nullptr)
		{
			fprintf(stderr, "Failed to initialize ImPlot!\n");
			return EXIT_FAILURE;
		}
		ImPlot::SetCurrentContext(implot_ctx);

		

#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop_arg(Application::Loop, (void *)this, 0, 1);
#else
		while (!glfwWindowShouldClose(mGLFWWindow))
		{
			Loop(this);
		}
#endif

		success = OnFinalize();
		if (!success)
			return EXIT_FAILURE;

		return EXIT_SUCCESS;
	}

	void Application::Loop(void *_arg)
	{
		Application *app = reinterpret_cast<Application *>(_arg);

		if (!app->mIsSync)
		{
			glfwSwapInterval(1); // Enable vsync
			app->mIsSync = true;
		}

#ifdef __EMSCRIPTEN__
		int width, height;
		if (emscripten_get_canvas_element_size(".emscripten", &width, &height) != EMSCRIPTEN_RESULT_SUCCESS)
		{
			fprintf(stderr, "unsupported emscripten_get_canvas_element_size!\n");
			return;
		}

		if (width != app->mWidth || (height != app->mHeight && height != 0))
		{
			app->mWidth = width;
			app->mHeight = height;
			glfwSetWindowSize(app->mGLFWWindow, width, height);
			app->OnResize(width, height);
		}
#endif

		
		static double prev_time = 0;
		double curr_time = glfwGetTime();
		app->mFPS = (double)1.0 / (curr_time - prev_time);// ImGui::GetIO().Framerate;
		prev_time = curr_time;


		//for (uint32_t i = 0; i < 2; i++)
		{
			gl::Render::SetClearColor(0.f, 0.3f, 0.4f, 1.f);
			gl::Render::Clear(gl::BufferBit::COLOR, gl::BufferBit::DEPTH);

			if (util::ResourcesManager::IsLoaded())
			{ // render scene
				app->OnRender();
			}

			{ // render menu

				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();

				app->OnGui();

				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			}
			gl::Render::Flush();
			
			glfwSwapBuffers(app->mGLFWWindow);
			glfwPollEvents();
		}

		
	}
}