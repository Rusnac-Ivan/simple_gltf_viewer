#include <iostream>
#include <chrono>

#define IMGUI_DEFINE_MATH_OPERATORS

#include <imgui.h>

#include <core/Application.h>
#include <opengl/Render.h>
#include <utilities/Resources/ProgramsManager.h>
#include <utilities/Resources/ResourcesManager.h>
#include <utilities/Camera/Camera.h>
#include <opengl/Pipeline.h>
#include <core/font_awesome_5.h>
#include <imgui_internal.h>
#include <utilities/imgui_tools/imgui_util.h>
#include <utilities/imgui_tools/imgui_animated.h>
//#include <utilities/FileDialog/NativeFileDialog.h>
#include <utilities/FileDialog/FileDialog.h>
#include <implot.h>


class MyApp : public core::Application
{
private:
	util::Camera<util::ProjectionType::PERSPECTIVE> mCamera;
	std::unique_ptr<FileDialog> mFileDialog;

	

	MyApp(MyApp &app) = delete;
	MyApp &operator=(MyApp &app) = delete;
	MyApp() : Application() {}
	~MyApp() {}

public:

	static MyApp &Launch()
	{
		static MyApp app;
		return app;
	}

	virtual bool OnInitialize()
	{
		mFileDialog = std::make_unique<FileDialog>();
		printf("MyApp::OnInitialize\n");
		mCamera.SetViewState(glm::vec3(0.f, 0.8f, 0.8f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, -1.f, -1.f));
		util::ResourcesManager::OnInitialize();

		gl::Pipeline::SetBlendFunc(gl::ComputOption::SRC_ALPHA, gl::ComputOption::ONE_MINUS_SRC_ALPHA);
		gl::Pipeline::EnableDepthTest();
		//gl::Pipeline::EnableCullFace();


		return true;
	};

	// Note: as with every ImDrawList drawing function, this expects that the font atlas texture is bound.
	void RenderChar(ImDrawList* draw_list, float size, ImVec2 pos, ImU32 col, ImWchar c) const
	{
		ImFont* font = ImGui::GetFont();
		const ImFontGlyph* glyph = font->FindGlyph(c);
		if (!glyph || !glyph->Visible)
			return;
		if (glyph->Colored)
			col |= ~IM_COL32_A_MASK;
		float scale = (size >= 0.0f) ? (size / font->FontSize) : 1.0f;
		pos.x = IM_FLOOR(pos.x);
		pos.y = IM_FLOOR(pos.y);
		draw_list->PrimReserve(6, 4);
		draw_list->PrimRectUV(ImVec2(pos.x + glyph->X0 * scale, pos.y + glyph->Y0 * scale), ImVec2(pos.x + glyph->X1 * scale, pos.y + glyph->Y1 * scale), ImVec2(glyph->U1, glyph->V0), ImVec2(glyph->U0, glyph->V1), col);
	}


	virtual bool OnGui()
	{
		
		// printf("MyApp::OnGui\n");
		//ImGui::ShowDemoWindow(nullptr);
		if (util::ResourcesManager::IsLoaded())
		{

			if (mFileDialog->IsInStandby() == false)
			{
				ImGui::SetNextWindowPos(ImVec2(0.f, 0.f), ImGuiCond_Always);
				ImGui::SetNextWindowSize(ImVec2(GetWidth() / 4.f, GetHeight()), ImGuiCond_Always);
				ImGui::Begin("Test", nullptr, ImGuiWindowFlags_NoResize);
				{
					rsrc::Model* model = util::ResourcesManager::GetModelsManager()->GetTmpModel();
					if (model)
						model->OnGUI();
					// Usage, e.g.
					/*ImGui::Text("%s among %d items", ICON_FA_EDIT, 4);
					ImGui::Text("%s", ICON_FA_BIBLE);
					ImVec2 pos0 = ImGui::GetCursorPos();
					ImGui::Button(ICON_FA_EDIT " Search");
					ImVec2 pos1 = ImGui::GetCursorPos();
					uint32_t id_img_button = ImGui::GetItemID();
					if (ImGui::GetHoveredID() == id_img_button)
					{
						ImGui::SetCursorPos(pos0);
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.f, 1.f, 0.f, 1.f));
						ImGui::Text("Test");
						ImGui::PopStyleColor();
						ImGui::SetCursorPos(pos1);
						int a = 0;
					}*/
				}
				ImGui::End();
			}
			else
			{
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.f, 0.f, 0.f, 1.f));
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
				ImGui::SetNextWindowPos(ImVec2(0.f, 0.f), ImGuiCond_Always);
				ImGui::SetNextWindowSize(ImVec2(GetWidth(), GetHeight()), ImGuiCond_Always);
				ImGui::Begin("loading", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs); // ImGuiWindowFlags_NoBackground
				{
					ImVec2 win_size = ImGui::GetWindowSize();
					float dim = win_size.x < win_size.y ? win_size.x : win_size.y;
					float radius = 0.2f * dim;
					const ImVec4 first_color = ImVec4(0.0f, 1.f, 0.f, 0.5f);
					const ImVec4 second_color = ImVec4(0.f, 0.f, 1.f, 0.5f);
					int circle_count = radius > 67 ? 0.12f * radius : 8;
					// circle_count = 8;
					ImVec2 load_indicator_pos = ImVec2(win_size.x / 2.f - radius, win_size.y / 2.f - radius);
					ImGui::SetCursorPos(load_indicator_pos);
					ImGui::LoadingIndicatorCircle("Loading", radius, first_color, second_color, circle_count, 13.f);

					// ImVec2 text_size = ImGui::CalcTextSize("LOADING");
					// ImGui::SetCursorPos(ImVec2((win_size.x - text_size.x) / 2.f, (win_size.y - text_size.y) / 2.f));
					// ImGui::Text("LOADING");


				}
				ImGui::End();
				ImGui::PopStyleVar();
				ImGui::PopStyleColor();
			}

			ImVec2 win_size = ImVec2(170.f, 140.f);
			ImGui::SetNextWindowPos(ImVec2(GetWidth() - win_size.x, 0.f), ImGuiCond_Always);
			ImGui::SetNextWindowSize(win_size);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
			// ImGui::SetNextWindowFocus();
			if(ImGui::Begin("info", nullptr, ImGuiWindowFlags_NoDecoration))
			{

				static float FPS[100] = {};

				float fps = mFPS;

				float average_fps = 0.f;
				for (uint32_t i = 1; i < 100; i++)
				{
					FPS[i - 1] = FPS[i];
					FPS[99] = fps;
					average_fps += FPS[i] / 100.f;
				}

				if (average_fps > 50.f)
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.f, 0.8f, 0.f, 1.f));
				}
				else if (average_fps > 30.f)
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.f, 1.f));
				}
				else
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.f, 0.f, 1.f));
				}

				char text[128] = {};

				sprintf(text, "FPS: %.3f", average_fps);
				ImGui::Dummy(ImVec2(20.f, 5.f));
				ImGui::SameLine();
				ImGui::Text("%-64s", text);

				ImGui::SetNextItemWidth(win_size.x);
				ImGui::PlotLines("##Frame Times", &FPS[0], 100);

				ImGui::PopStyleColor();

				if (ImGui::ColoredButton("Load", ImVec2(win_size.x - 1.f, 30.f), IM_COL32(255, 255, 255, 255), IM_COL32(50, 220, 60, 255), IM_COL32(69, 150, 70, 255)))
				{
					util::ResourcesManager::GetModelsManager()->UnLoadModel();
					mFileDialog->Load("glb", util::OnLoaded([](const char* file, unsigned char* data, int size)
						{
							util::ResourcesManager::GetModelsManager()->LoadModel(file, data, size);
						})
					);
				}

				//ImGui::Dummy(ImVec2(30, 30));

				ImGui::Separator();
				ImGui::Text("Debug Render");

				const char* items[] = { "none", "base color", "normal", "roughness", "metallic" };
				static int item_current = 0;
				ImGui::SetNextItemWidth(ImGui::GetWindowWidth());
				ImGui::Combo("##debug", &item_current, items, IM_ARRAYSIZE(items));

				rsrc::Program* pbr_prog = util::ResourcesManager::GetProgramsManager()->GetPBRProg();
				if (util::ResourcesManager::IsLoaded())
				{
					pbr_prog->Use();
					pbr_prog->SetUniform(rsrc::Program::UNIFORM_DEBUG_VIEW_INPUTS, item_current);
				}

				/*if (ImGui::Button("Load", ImVec2(win_size.x - 1.f, 30.f)))
				{
					util::ResourcesManager::GetModelsManager()->UnLoadModel();
					mFileDialog->Load("glb", util::OnLoaded([](const char *file, unsigned char *data, int size)
															{ util::ResourcesManager::GetModelsManager()->LoadModel(file, data, size); }));
				}*/
				ImGui::End();
			}
			ImGui::PopStyleVar();

			/*ImPlotAxisFlags flags = ImPlotAxisFlags_AutoFit;
			//ImGui::SetNextWindowPos(ImVec2(100.f, 100.f), ImGuiCond_Always);
			if (ImGui::Begin("window"))
			{
				
				
			

				RenderChar(ImGui::GetCurrentWindow()->DrawList, 32, ImVec2(100.f, 100.f), IM_COL32(0, 0, 255, 255), 0xf2f5); ICON_FA_EDIT;
			}
			ImGui::End();
	

			ImPlot::ShowDemoWindow(nullptr);*/

			/*ImGui::Begin("Test widgets", nullptr);
			{



			}
			ImGui::End();*/

			/*ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 20.f);
			ImGui::Begin("Gradient bg", nullptr, NULL, IM_COL32(100, 100, 100, 200), IM_COL32(30, 30, 30, 200));
			{
				
				ImGui::ButtonNew("Simple\nButton", ImVec2(150.f, 80.f), NULL);

				ImGui::ColoredButton("Gradient\nButton", ImVec2(150.f, 80.f), IM_COL32(255, 255, 255, 255), IM_COL32(220, 60, 60, 255), IM_COL32(150, 40, 70, 255));

				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 20.f);
				ImGui::ColoredButton("Gradient\nRounding\nButton", ImVec2(150.f, 80.f), IM_COL32(255, 255, 255, 255), IM_COL32(50, 220, 60, 255), IM_COL32(69, 150, 70, 255));
				ImGui::PopStyleVar();

				static bool f = true;
				//ImGui::Toggle("Toggle", &f);
				ImGui::Checkbox("Toggle", &f);
				ImGui::RadioButton("But", &f);
				

				ImGui::Text("Gradients");
				ImDrawList* draw_list = ImGui::GetWindowDrawList();
				ImVec2 gradient_size = ImVec2(ImGui::CalcItemWidth(), ImGui::GetFrameHeight());
				{
					ImVec2 p0 = ImGui::GetCursorScreenPos();
					ImVec2 p1 = ImVec2(p0.x + gradient_size.x, p0.y + gradient_size.y);
					ImU32 col_a = ImGui::GetColorU32(IM_COL32(0, 0, 0, 255));
					ImU32 col_b = ImGui::GetColorU32(IM_COL32(255, 255, 255, 255));
					draw_list->AddRectFilledMultiColor(p0, p1, col_a, col_b, col_b, col_a);
					ImGui::InvisibleButton("##gradient1", gradient_size);
				}
				{
					ImVec2 p0 = ImGui::GetCursorScreenPos();
					ImVec2 p1 = ImVec2(p0.x + gradient_size.x, p0.y + gradient_size.y);
					ImU32 col_a = ImGui::GetColorU32(IM_COL32(0, 255, 0, 255));
					ImU32 col_b = ImGui::GetColorU32(IM_COL32(255, 0, 0, 255));
					draw_list->AddRectFilledMultiColor(p0, p1, col_a, col_b, col_b, col_a);
					ImGui::InvisibleButton("##gradient2", gradient_size);
				}

				ImGui::Dummy(ImVec2(50.f, 50.f));
				ImGui::SameLine();
				rsrc::Image* img = util::ResourcesManager::GetImagesManager()->GetImage(util::ImagesManager::REAL_ESTATE);
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.f);
				ImGui::ImageButton("Real Estate", (ImTextureID)img->GetTexture()->GetID(), ImVec2(img->GetWidth() / 2.f, img->GetHeight() / 2.f + 100.f), ImVec2(img->GetWidth()/2.f, img->GetHeight()/2.f));
				ImGui::PopStyleVar();

				ImGui::ButtonScrollable("ButtonScrollable ButtonScrollable ButtonScrollable", ImVec2(150.f, 30.f));
			}
			ImGui::End();
			ImGui::PopStyleVar();*/

			/*ImGui::SetNextWindowSize(ImVec2(900.f, 500.f), ImGuiCond_Always);
			ImGui::Begin("test render", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
			{
				mMainPage.Draw();
			}
			ImGui::End();*/
		}
		else
		{
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.f, 0.f, 0.f, 1.f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);
			ImGui::SetNextWindowPos(ImVec2(0.f, 0.f), ImGuiCond_Always);
			ImGui::SetNextWindowSize(ImVec2(GetWidth(), GetHeight()), ImGuiCond_Always);
			ImGui::Begin("loading", nullptr, ImGuiWindowFlags_NoDecoration); // ImGuiWindowFlags_NoBackground
			{
				ImVec2 win_size = ImGui::GetWindowSize();
				float dim = win_size.x < win_size.y ? win_size.x : win_size.y;
				float radius = 0.2f * dim;
				const ImVec4 first_color = ImVec4(0.0f, 0.8f, 0.8f, 1.f);
				const ImVec4 second_color = ImVec4(0.f, 0.8f, 0.f, 1.f);
				int circle_count = radius > 67 ? 0.12f * radius : 8;
				circle_count = 12;
				ImVec2 load_indicator_pos = ImVec2(win_size.x / 2.f - radius, win_size.y / 2.f - radius);
				ImGui::SetCursorPos(load_indicator_pos);
				ImGui::LoadingIndicatorCircle("LoadingPage", radius, first_color, second_color, circle_count, 13.f);

				// ImVec2 text_size = ImGui::CalcTextSize("LOADING");
				// ImGui::SetCursorPos(ImVec2((win_size.x - text_size.x) / 2.f, (win_size.y - text_size.y) / 2.f));
				// ImGui::Text("LOADING");
			}
			ImGui::End();
			ImGui::PopStyleVar();
			ImGui::PopStyleColor();
		}

		return true;
	};

	void InitPrograms(const glm::vec3 &light_pos, const glm::vec3 &view_pos, const glm::mat4 &proj_view)
	{
		util::ProgramsManager *programs = util::ResourcesManager::GetProgramsManager();
		programs->GetPBRProg()->Use();
		programs->GetPBRProg()->SetUniform(rsrc::Program::UNIFORM_PROJ_VIEW, proj_view);
		programs->GetPBRProg()->SetUniform(rsrc::Program::UNIFORM_EYE_POSE, view_pos);
		programs->GetPBRProg()->SetUniform(rsrc::Program::UNIFORM_LIGHT_POSE, light_pos);
		//programs->GetPBRProg()->SetUniform(rsrc::Program::UNIFORM_RESOLUTION, glm::vec2(GetWidth(), GetHeight()));

		programs->GetWParallaxProg()->Use();
		programs->GetWParallaxProg()->SetUniform(rsrc::Program::UNIFORM_VIEW, mCamera.GetViewMat());
		programs->GetWParallaxProg()->SetUniform(rsrc::Program::UNIFORM_PROJ, mCamera.GetProjectionMat());
		//programs->GetWParallaxProg()->SetUniform(rsrc::Program::UNIFORM_EYE_POSE, view_pos);

		programs->GetCubeMapProg()->Use();
		programs->GetCubeMapProg()->SetUniform(rsrc::Program::UNIFORM_PROJ_VIEW, proj_view);

		programs->GetBackNormalsProg()->Use();
		programs->GetBackNormalsProg()->SetUniform(rsrc::Program::UNIFORM_VIEW, mCamera.GetViewMat());
		programs->GetBackNormalsProg()->SetUniform(rsrc::Program::UNIFORM_PROJ, mCamera.GetProjectionMat());
	}

	virtual bool OnRender()
	{
		
		mCamera.OnUpdate();

		InitPrograms(mCamera.GetPosition(), mCamera.GetPosition(), mCamera.GetProjectionMat() * mCamera.GetViewMat());

		util::ResourcesManager::GetCubeMapsManager()->GetCubeMap(util::CubeMapsManager::BASE)->Draw();
		rsrc::Model* tmp_model = util::ResourcesManager::GetModelsManager()->GetTmpModel();
		if (tmp_model)
		{
			tmp_model->Draw(util::Rendering::Type::PBR, glm::mat4(1.f));
		}

		return true;
	};

	virtual bool OnFinalize()
	{
		printf("MyApp::OnFinalize\n");
		mFileDialog.release();
		util::ResourcesManager::OnFinalize();
		return true;
	};

	virtual void OnMouseLeftDown(double x, double y)
	{
		printf("MyApp::OnMouseLeftDown\n");
		mCamera.OnBeginDrag(x / GetWidth(), y / GetHeight());
	}
	virtual void OnMouseLeftUp(double x, double y)
	{
		printf("MyApp::OnMouseLeftUp\n");
		mCamera.OnEndDrag();
	}
	virtual void OnMouseRightDown(double x, double y)
	{
		printf("MyApp::OnMouseRightDown\n");
		mCamera.OnBeginPinch(x / GetWidth(), y / GetHeight());
	}
	virtual void OnMouseRightUp(double x, double y)
	{
		printf("MyApp::OnMouseRightUp\n");
		mCamera.OnEndPinch();
	}
	virtual void OnMouseMiddleDown(double x, double y)
	{
		printf("MyApp::OnMouseMiddleDown\n");
	}
	virtual void OnMouseMiddleUp(double x, double y)
	{
		printf("MyApp::OnMouseMiddleUp\n");
	}
	virtual void OnMouseMove(double x, double y)
	{
		printf("MyApp::OnMouseMove\n");
		mCamera.OnDrag(x / GetWidth(), y / GetHeight());
		mCamera.OnPinch(x / GetWidth(), y / GetHeight());
	}
	virtual void OnMouseWhell(double offset)
	{
		printf("MyApp::OnMouseWhell\n");
		mCamera.OnWheel(offset / 20.f);
	}
	virtual void OnKeyPress(int key)
	{
		printf("MyApp::OnKeyPress\n");
	}
	virtual void OnKeyRepeat(int key)
	{
		printf("MyApp::OnKeyRepeat\n");
	}
	virtual void OnKeyRelease(int key)
	{
		printf("MyApp::OnKeyRelease\n");
	}
	virtual void OnResize(int width, int height)
	{
		if (height == 0)
			return;
		printf("MyApp::OnResize\n");
		gl::Render::SetViewport(width, height);
		mCamera.SetState(0.01f, 100.f, (float)width / (float)height, 60.f);
	}

private:
};


int main(int argc, char **argv)
{
	MyApp::Launch().Run(argc, argv, "v0.0", 1280, 720, "Opengl Window");
	return 0;
}
