#include "imgui_util.h"

#include <glm/glm.hpp>

namespace ImGui
{
    void StyleColors()
    {
        ImGuiStyle &style = ImGui::GetStyle();

        style.Alpha = 1.0;
        style.FrameBorderSize = 3.f;
        style.WindowRounding = 0.f;
        style.ChildRounding = 0.f;
        style.GrabRounding = 0.f;
        style.GrabMinSize = 17.f;
        style.FrameRounding = 0.f;
        style.WindowPadding = ImVec2(5.f, 5.f);
        style.FramePadding = ImVec2(5.f, 5.f);
        style.CellPadding = ImVec2(5.f, 5.f);
        style.ScrollbarRounding = 0.f;

        // elements of manipulation
        ImVec4 defaultColor = ImVec4(54.f / 256.f, 54.f / 256.f, 54.f / 256.f, 0.5f);
        ImVec4 hoverColor = ImVec4(65.f / 256.f, 63.f / 256.f, 64.f / 256.f, 0.7f);
        ImVec4 activeColor = ImVec4(0.f, 177.f / 256.f, 243.f / 256.f, 0.9f);

        ImVec4 bgColor = ImVec4(92.f / 256.f, 88.f / 256.f, 89.f / 256.f, 0.5f);
        ImVec4 childBgColor = ImVec4(54.f / 256.f, 54.f / 256.f, 54.f / 256.f, 0.5f);

        ImVec4 textColor = ImVec4(1.f, 1.f, 1.f, 1.f);
        ImVec4 titleBgColor = ImVec4(0.f, 0.f, 0.f, 0.7f);
        ImVec4 titleBgCollapsedColor = ImVec4(0.f, 0.f, 0.f, 0.5f);
        ImVec4 titleBgActiveColor = ImVec4(0.f, 177.f / 256.f, 243.f / 256.f, 0.8f);
        ImVec4 scrollbarBg = ImVec4(65.f / 256.f, 63.f / 256.f, 64.f / 256.f, 1.f);
        ImVec4 scrollbarGrab = ImVec4(0.f, 177.f / 256.f, 243.f / 256.f, 0.5f);
        ImVec4 scrollbarGrabHovered = ImVec4(0.f, 177.f / 256.f, 243.f / 256.f, 0.7f);
        ImVec4 sliderGrabActive = ImVec4(0.f, 177.f / 256.f, 243.f / 256.f, 0.9f);

        style.Colors[ImGuiCol_Text] = textColor;
        // style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.00f, 0.40f, 0.41f, 1.00f);
        style.Colors[ImGuiCol_WindowBg] = bgColor;
        style.Colors[ImGuiCol_ChildBg] = childBgColor;
        style.Colors[ImGuiCol_FrameBg] = defaultColor;
        style.Colors[ImGuiCol_FrameBgHovered] = hoverColor;
        style.Colors[ImGuiCol_FrameBgActive] = activeColor;
        style.Colors[ImGuiCol_TitleBg] = titleBgColor;
        style.Colors[ImGuiCol_TitleBgCollapsed] = titleBgCollapsedColor;
        style.Colors[ImGuiCol_TitleBgActive] = titleBgActiveColor;
        // style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
        style.Colors[ImGuiCol_ScrollbarBg] = scrollbarBg;
        style.Colors[ImGuiCol_ScrollbarGrab] = scrollbarGrab;
        style.Colors[ImGuiCol_ScrollbarGrabHovered] = scrollbarGrabHovered;
        style.Colors[ImGuiCol_ScrollbarGrabActive] = sliderGrabActive;
        // style.Colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 1.00f, 1.00f, 0.68f);
        // style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.36f);
        // style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.76f);
        style.Colors[ImGuiCol_Button] = defaultColor;
        style.Colors[ImGuiCol_ButtonHovered] = hoverColor;
        style.Colors[ImGuiCol_ButtonActive] = activeColor;
        style.Colors[ImGuiCol_Header] = defaultColor;
        style.Colors[ImGuiCol_HeaderHovered] = hoverColor;
        style.Colors[ImGuiCol_HeaderActive] = activeColor;
        // style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
        // style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
        // style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
        // style.Colors[ImGuiCol_PlotLines] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
        // style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
        // style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
        // style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
        // style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 1.00f, 1.00f, 0.22f);
    }

    void LoadingIndicatorCircle(const char *label, const float indicator_radius, const ImVec4 &main_color, const ImVec4 &backdrop_color, const int circle_count, const float speed)
    {
        ImGuiWindow *window = GetCurrentWindow();
        if (window->SkipItems)
        {
            return;
        }

        ImGuiContext &g = *GImGui;
        const ImGuiStyle &style = g.Style;
        const ImGuiID id = window->GetID(label);

        const ImVec2 pos = window->DC.CursorPos;
        const float circle_radius = indicator_radius / 10.0f;
        const ImRect bb(pos, ImVec2(pos.x + indicator_radius * 2.0f, pos.y + indicator_radius * 2.0f));
        ItemSize(bb, style.FramePadding.y);
        if (!ItemAdd(bb, id))
        {
            return;
        }

        // const float t = g.Time;
        static float t = 0.0;
        t += 0.005;

        const auto degree_offset = 2.0f * IM_PI / circle_count;

        for (int i = 0; i < circle_count; ++i)
        {
            const auto x = indicator_radius * glm::sin(degree_offset * i);
            const auto y = indicator_radius * glm::cos(degree_offset * i);
            const auto growth = glm::max(0.0f, glm::sin(t * speed - i * degree_offset));
            ImVec4 color;
            color.x = main_color.x * growth + backdrop_color.x * (1.0f - growth);
            color.y = main_color.y * growth + backdrop_color.y * (1.0f - growth);
            color.z = main_color.z * growth + backdrop_color.z * (1.0f - growth);
            color.w = main_color.w * growth + backdrop_color.w * (1.0f - growth);

            window->DrawList->AddCircleFilled(ImVec2(pos.x + indicator_radius + x,
                                                     pos.y + indicator_radius - y),
                                              circle_radius + growth * circle_radius,
                                              GetColorU32(color));
        }
    }

    bool BufferingBar(const char *label, float value, const ImVec2 &size_arg, const ImU32 &bg_col, const ImU32 &fg_col)
    {
        ImGuiWindow *window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext &g = *GImGui;
        const ImGuiStyle &style = g.Style;
        const ImGuiID id = window->GetID(label);

        ImVec2 pos = window->DC.CursorPos;
        ImVec2 size = size_arg;
        size.x -= style.FramePadding.x * 2;

        const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
        ItemSize(bb, style.FramePadding.y);
        if (!ItemAdd(bb, id))
            return false;

        // Render
        const float circleStart = size.x * 0.7f;
        const float circleEnd = size.x;
        const float circleWidth = circleEnd - circleStart;

        window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart, bb.Max.y), bg_col);
        window->DrawList->AddRectFilled(bb.Min, ImVec2(pos.x + circleStart * value, bb.Max.y), fg_col);

        const float t = g.Time;
        const float r = size.y / 2;
        const float speed = 1.5f;

        const float a = speed * 0;
        const float b = speed * 0.333f;
        const float c = speed * 0.666f;

        const float o1 = (circleWidth + r) * (t + a - speed * (int)((t + a) / speed)) / speed;
        const float o2 = (circleWidth + r) * (t + b - speed * (int)((t + b) / speed)) / speed;
        const float o3 = (circleWidth + r) * (t + c - speed * (int)((t + c) / speed)) / speed;

        window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o1, bb.Min.y + r), r, bg_col);
        window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o2, bb.Min.y + r), r, bg_col);
        window->DrawList->AddCircleFilled(ImVec2(pos.x + circleEnd - o3, bb.Min.y + r), r, bg_col);

        return true;
    }

    bool Spinner(const char *label, float radius, int thickness, const ImU32 &color)
    {
        ImGuiWindow *window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext &g = *GImGui;
        const ImGuiStyle &style = g.Style;
        const ImGuiID id = window->GetID(label);

        ImVec2 pos = window->DC.CursorPos;
        ImVec2 size((radius)*2, (radius + style.FramePadding.y) * 2);

        const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
        ItemSize(bb, style.FramePadding.y);
        if (!ItemAdd(bb, id))
            return false;

        // Render
        window->DrawList->PathClear();

        int num_segments = 30;
        int start = glm::abs(ImSin(g.Time * 1.8f) * (num_segments - 5));

        const float a_min = IM_PI * 2.0f * ((float)start) / (float)num_segments;
        const float a_max = IM_PI * 2.0f * ((float)num_segments - 3) / (float)num_segments;

        const ImVec2 centre = ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);

        for (int i = 0; i < num_segments; i++)
        {
            const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
            window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a + g.Time * 8) * radius,
                                                centre.y + ImSin(a + g.Time * 8) * radius));
        }

        window->DrawList->PathStroke(color, false, thickness);

        return true;
    }

    // Implementation
    bool ColoredButton(const char *label, const ImVec2 &size_arg, ImU32 text_color, ImU32 bg_color_1, ImU32 bg_color_2)
    {
        ImGuiWindow *window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext &g = *GImGui;
        const ImGuiStyle &style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = CalcTextSize(label, NULL, true);

        ImVec2 pos = window->DC.CursorPos;
        ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

        const ImRect bb(pos, pos + size);
        ItemSize(size, style.FramePadding.y);
        if (!ItemAdd(bb, id))
            return false;

        ImGuiButtonFlags flags = ImGuiButtonFlags_None;
        if (g.LastItemData.InFlags & ImGuiItemFlags_ButtonRepeat)
            flags |= ImGuiButtonFlags_Repeat;

        bool hovered, held;
        bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

        // Render
        const bool is_gradient = bg_color_1 != bg_color_2;
        if (held || hovered)
        {
            // Modify colors (ultimately this can be prebaked in the style)
            float h_increase = (held && hovered) ? 0.02f : 0.02f;
            float v_increase = (held && hovered) ? 0.20f : 0.07f;

            ImVec4 bg1f = ColorConvertU32ToFloat4(bg_color_1);
            ColorConvertRGBtoHSV(bg1f.x, bg1f.y, bg1f.z, bg1f.x, bg1f.y, bg1f.z);
            bg1f.x = ImMin(bg1f.x + h_increase, 1.0f);
            bg1f.z = ImMin(bg1f.z + v_increase, 1.0f);
            ColorConvertHSVtoRGB(bg1f.x, bg1f.y, bg1f.z, bg1f.x, bg1f.y, bg1f.z);
            bg_color_1 = GetColorU32(bg1f);
            if (is_gradient)
            {
                ImVec4 bg2f = ColorConvertU32ToFloat4(bg_color_2);
                ColorConvertRGBtoHSV(bg2f.x, bg2f.y, bg2f.z, bg2f.x, bg2f.y, bg2f.z);
                bg2f.z = ImMin(bg2f.z + h_increase, 1.0f);
                bg2f.z = ImMin(bg2f.z + v_increase, 1.0f);
                ColorConvertHSVtoRGB(bg2f.x, bg2f.y, bg2f.z, bg2f.x, bg2f.y, bg2f.z);
                bg_color_2 = GetColorU32(bg2f);
            }
            else
            {
                bg_color_2 = bg_color_1;
            }
        }
        RenderNavHighlight(bb, id);

#if 0
        // V1 : faster but prevents rounding
        window->DrawList->AddRectFilledMultiColor(bb.Min, bb.Max, bg_color_1, bg_color_1, bg_color_2, bg_color_2);
        if (g.Style.FrameBorderSize > 0.0f)
            window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(ImGuiCol_Border), 0.0f, 0, g.Style.FrameBorderSize);
#endif

        // V2
        int vert_start_idx = window->DrawList->VtxBuffer.Size;
        window->DrawList->AddRectFilled(bb.Min, bb.Max, bg_color_1, g.Style.FrameRounding);
        int vert_end_idx = window->DrawList->VtxBuffer.Size;
        if (is_gradient)
            ShadeVertsLinearColorGradientKeepAlpha(window->DrawList, vert_start_idx, vert_end_idx, bb.Min, bb.GetBL(), bg_color_1, bg_color_2);
        if (g.Style.FrameBorderSize > 0.0f)
            window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(ImGuiCol_Border), g.Style.FrameRounding, 0, g.Style.FrameBorderSize);

        if (g.LogEnabled)
            LogSetNextTextDecoration("[", "]");
        PushStyleColor(ImGuiCol_Text, text_color);
        RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
        PopStyleColor();

        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
        return pressed;
    }

    bool HoverButton(const char *label, const ImVec2 &size)
    {
        char label_text[128] = {};
        sprintf(label_text, "##", label);

        bool res = ImGui::Button(label_text, size);

        /*ImVec2 pos1 = ImGui::GetCursorPos();
        ImGui::SameLine();
        ImVec2 pos2 = ImGui::GetCursorPos();
        ImGui::GetItemRectSize();
        uint32_t id_img_button = ImGui::GetItemID();
        if (ImGui::GetHoveredID() == id_img_button)
        {
            ImGui::SetCursorPos(pos0);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.f, 1.f, 0.f, 1.f));
//            ImGui::Button(ICON_FA_DOWNLOAD);

            ImGui::PopStyleColor();
            ImGui::SetCursorPos(ImVec2(pos2.x, pos1.y));
            int a = 0;
        }*/

        return res;
    }

    bool Begin(const char *name, bool *p_open, ImGuiWindowFlags flags, ImU32 bg_color_1, ImU32 bg_color_2)
    {
        flags |= ImGuiWindowFlags_NoBackground;
        bool res = ImGui::Begin(name, p_open, flags);

        // ImGuiWindow* window = GetCurrentWindow();

        ImGuiWindow *window = GetCurrentWindow();
        ImGuiContext &g = *GImGui;
        const ImGuiStyle &style = g.Style;
        const ImGuiID id = window->GetID(name);

        ImVec2 pos = ImGui::GetWindowPos();
        ImVec2 size = ImGui::GetWindowSize() - style.WindowPadding;

        const ImRect bb(pos, pos + size);
        /*ItemSize(size, style.WindowPadding.y);
        if (!ItemAdd(bb, id))
            return false;*/

        // V2
        int vert_start_idx = window->DrawList->VtxBuffer.Size;
        window->DrawList->AddRectFilled(bb.Min, bb.Max, bg_color_1, g.Style.WindowRounding);
        int vert_end_idx = window->DrawList->VtxBuffer.Size;
        ShadeVertsLinearColorGradientKeepAlpha(window->DrawList, vert_start_idx, vert_end_idx, bb.Min, bb.GetBL(), bg_color_1, bg_color_2);
        if (g.Style.WindowBorderSize > 0.0f)
            window->DrawList->AddRect(bb.Min, bb.Max, GetColorU32(ImGuiCol_Border), g.Style.WindowRounding, 0, g.Style.WindowBorderSize);
        return res;
    }

    bool ImageButton(const char* label, ImTextureID user_texture_id, const ImVec2& button_size, const ImVec2& image_size, const ImVec2& uv0, const ImVec2& uv1, int frame_padding)
    {
        static float is_hover_acum = 0.f;

        //ImVec2 _uv0 = uv0 + ImVec2(is_hover_acum / 6.f, is_hover_acum / 6.f);
        //ImVec2 _uv1 = uv1 - ImVec2(is_hover_acum / 6.f, is_hover_acum / 6.f);

        bool res = Button(label, button_size);

        uint32_t id_img_button = ImGui::GetItemID();
        ImGuiWindow *window = ImGui::GetCurrentWindow();
        ImGuiContext &g = *GImGui;
        const ImGuiStyle &style = g.Style;

        ImVec2 pos = GetItemRectMin();
        const ImRect bb(pos, pos + image_size * (0.5f + is_hover_acum / 1.5f));

        float step = 0.05f;

        if (ImGui::GetHoveredID() == id_img_button)
        {
            if (is_hover_acum < 1.f)
                is_hover_acum += step;
            if (is_hover_acum > 1.f)
                is_hover_acum = 1.f;
        }
        else
        {
            if (is_hover_acum > 0.f)
                is_hover_acum -= step;
            if (is_hover_acum < 0.f)
                is_hover_acum = 0.f;
        }

        //int vert_start_idx = window->DrawList->VtxBuffer.Size;
        window->DrawList->AddImage((ImTextureID)user_texture_id, bb.Min, bb.Max, uv0, uv1);
        //int vert_end_idx = window->DrawList->VtxBuffer.Size;

        int vert_start_idx = window->DrawList->VtxBuffer.Size;
        window->DrawList->AddRect(bb.Min, bb.Max, ImGui::GetColorU32(IM_COL32(0, 0, 0, 255)), 10.f, NULL, 10.f * is_hover_acum);
        int vert_end_idx = window->DrawList->VtxBuffer.Size;

        //int vert_start_idx = window->DrawList->VtxBuffer.Size;
        //window->DrawList->AddRectFilled(bb.Min, bb.Max, col_1 * is_hover_acum, g.Style.FrameRounding);
        //int vert_end_idx = window->DrawList->VtxBuffer.Size;
        //ShadeVertsLinearColorGradientKeepAlpha(window->DrawList, vert_start_idx, vert_end_idx, bb.Min, bb.GetBL(), col_1, col_2);

        ImDrawVert* vert_start = window->DrawList->VtxBuffer.Data + vert_start_idx;
        ImDrawVert* vert_end = window->DrawList->VtxBuffer.Data + vert_end_idx;

        
        ImVec2 cen = (bb.Min + bb.Max) / 2.f;
        float s = -sin(is_hover_acum), c = cos(is_hover_acum);
        ImVec2 center = ImRotate(cen, s, c) - cen;

        ImVec2 gradient_extent = bb.Min - bb.GetBL();
        float gradient_inv_length2 = 1.0f / ImLengthSqr(gradient_extent);
        ImU32 col0 = ImGui::GetColorU32(IM_COL32(255, 0, 0, 100));
        ImU32 col1 = ImGui::GetColorU32(IM_COL32(0, 255, 0, 100));
        const int col0_r = (int)(col0 >> IM_COL32_R_SHIFT) & 0xFF;
        const int col0_g = (int)(col0 >> IM_COL32_G_SHIFT) & 0xFF;
        const int col0_b = (int)(col0 >> IM_COL32_B_SHIFT) & 0xFF;
        const int col_delta_r = ((int)(col1 >> IM_COL32_R_SHIFT) & 0xFF) - col0_r;
        const int col_delta_g = ((int)(col1 >> IM_COL32_G_SHIFT) & 0xFF) - col0_g;
        const int col_delta_b = ((int)(col1 >> IM_COL32_B_SHIFT) & 0xFF) - col0_b;
        for (ImDrawVert* vert = vert_start; vert < vert_end; vert++)
        {
            
            float d = ImDot(vert->pos - bb.Min, gradient_extent);
            float t = d * gradient_inv_length2;
            int r = (int)(col0_r + col_delta_r * t);
            int g = (int)(col0_g + col_delta_g * t);
            int b = (int)(col0_b + col_delta_b * t);
            vert->col = (r << IM_COL32_R_SHIFT) | (g << IM_COL32_G_SHIFT) | (b << IM_COL32_B_SHIFT) | (vert->col & IM_COL32_A_MASK);
            vert->pos = ImRotate(vert->pos, s, c) - center;
        }

        return res;
    }

    bool ImageButton(ImTextureID user_texture_id, const ImVec2& size, const ImU32& col_1, const ImU32& col_2, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, const ImVec4& tint_col)
    {
        static float is_hover_acum = 0.f;

        ImVec2 _uv0 = uv0 + ImVec2(is_hover_acum / 6.f, is_hover_acum / 6.f);
        ImVec2 _uv1 = uv1 - ImVec2(is_hover_acum / 6.f, is_hover_acum / 6.f);

        bool res = ImageButton(user_texture_id, size, _uv0, _uv1, frame_padding, ImVec4(0.f, 0.f, 0.f, 0.f), tint_col);

        uint32_t id_img_button = ImGui::GetItemID();
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;

        ImVec2 pos = GetItemRectMin();
        ImVec2 item_size = GetItemRectSize();
        const ImRect bb(pos, pos + item_size);

        float step = 0.05f;

        if (ImGui::GetHoveredID() == id_img_button)
        {
            if (is_hover_acum < 1.f)
                is_hover_acum += step;
            if (is_hover_acum > 1.f)
                is_hover_acum = 1.f;
        }
        else
        {
            if (is_hover_acum > 0.f)
                is_hover_acum -= step;
            if (is_hover_acum < 0.f)
                is_hover_acum = 0.f;
        }

        int vert_start_idx = window->DrawList->VtxBuffer.Size;
        window->DrawList->AddRectFilled(bb.Min, bb.Max, col_1 * is_hover_acum, g.Style.FrameRounding);
        // window->DrawList->AddRectFilledMultiColor(bb.Min, bb.Max, col_1, col_1, col_2, col_2);
        int vert_end_idx = window->DrawList->VtxBuffer.Size;
        ShadeVertsLinearColorGradientKeepAlpha(window->DrawList, vert_start_idx, vert_end_idx, bb.Min, bb.GetBL(), col_1, col_2);

        return res;
    }

    // Generic linear color gradient, write to RGB fields, leave A untouched.
    void ShadeVertsLinearColorGradientKeepAlpha_(ImDrawList* draw_list, int vert_start_idx, int vert_end_idx, ImVec2 gradient_p0, ImVec2 gradient_p1, ImU32 col0, ImU32 col1)
    {
        ImVec2 gradient_extent = gradient_p1 - gradient_p0;
        float gradient_inv_length2 = 1.0f / ImLengthSqr(gradient_extent);
        ImDrawVert* vert_start = draw_list->VtxBuffer.Data + vert_start_idx;
        ImDrawVert* vert_end = draw_list->VtxBuffer.Data + vert_end_idx;
        const int col0_r = (int)(col0 >> IM_COL32_R_SHIFT) & 0xFF;
        const int col0_g = (int)(col0 >> IM_COL32_G_SHIFT) & 0xFF;
        const int col0_b = (int)(col0 >> IM_COL32_B_SHIFT) & 0xFF;
        const int col_delta_r = ((int)(col1 >> IM_COL32_R_SHIFT) & 0xFF) - col0_r;
        const int col_delta_g = ((int)(col1 >> IM_COL32_G_SHIFT) & 0xFF) - col0_g;
        const int col_delta_b = ((int)(col1 >> IM_COL32_B_SHIFT) & 0xFF) - col0_b;
        for (ImDrawVert* vert = vert_start; vert < vert_end; vert++)
        {
            float d = ImDot(vert->pos - gradient_p0, gradient_extent);
            float t = ImClamp(d * gradient_inv_length2, 0.0f, 1.0f);
            int r = (int)(col0_r + col_delta_r * t);
            int g = (int)(col0_g + col_delta_g * t);
            int b = (int)(col0_b + col_delta_b * t);
            vert->col = (r << IM_COL32_R_SHIFT) | (g << IM_COL32_G_SHIFT) | (b << IM_COL32_B_SHIFT) | (vert->col & IM_COL32_A_MASK);
        }
    }

    /*void RenderNavHighlight(const ImRect& bb, ImGuiID id, ImGuiNavHighlightFlags flags)
    {
        return;
         
        ImGuiContext& g = *GImGui;
        if (id != g.NavId)
            return;
        if (g.NavDisableHighlight && !(flags & ImGuiNavHighlightFlags_AlwaysDraw))
            return;
        ImGuiWindow* window = g.CurrentWindow;
        if (window->DC.NavHideHighlightOneFrame)
            return;

        float rounding = (flags & ImGuiNavHighlightFlags_NoRounding) ? 0.0f : g.Style.FrameRounding;
        ImRect display_rect = bb;
        display_rect.ClipWith(window->ClipRect);
        if (flags & ImGuiNavHighlightFlags_TypeDefault)
        {
            const float THICKNESS = 2.0f;
            const float DISTANCE = 3.0f + THICKNESS * 0.5f; //??????
            display_rect.Expand(ImVec2(DISTANCE, DISTANCE));
            bool fully_visible = window->ClipRect.Contains(display_rect);
            if (!fully_visible)
                window->DrawList->PushClipRect(display_rect.Min, display_rect.Max);
            window->DrawList->AddRect(display_rect.Min + ImVec2(THICKNESS * 0.5f, THICKNESS * 0.5f), display_rect.Max - ImVec2(THICKNESS * 0.5f, THICKNESS * 0.5f), GetColorU32(ImGuiCol_NavHighlight), rounding, 0, THICKNESS);
            if (!fully_visible)
                window->DrawList->PopClipRect();
        }
        if (flags & ImGuiNavHighlightFlags_TypeThin)
        {
            window->DrawList->AddRect(display_rect.Min, display_rect.Max, GetColorU32(ImGuiCol_NavHighlight), rounding, 0, 1.0f);
        }
    }*/

    enum struct GradientType
    {
        NONE       ,
        HORIZONTAL,
        VERTICAL,
        DIAGONAL_LB_RT,
        DIAGONAL_LT_RB,
        RADIAL
    };

    enum struct Animation
    {
        NONE,
        TRANSLATE, 
        ROTATE,
        SCALE
    };

    template<GradientType gradType>
    void RenderEffect(ImDrawVert* vert_start, ImDrawVert* vert_end, const ImVec2& p_min, const ImVec2& p_max, ImU32 col0, ImU32 col1);

    template<>
    void RenderEffect<GradientType::NONE>(ImDrawVert* vert_start, ImDrawVert* vert_end, const ImVec2& p_min, const ImVec2& p_max, ImU32 col0, ImU32 col1) {}

    template<>
    void RenderEffect<GradientType::HORIZONTAL>(ImDrawVert* vert_start, ImDrawVert* vert_end, const ImVec2& p_min, const ImVec2& p_max, ImU32 col0, ImU32 col1)
    {
        ImVec2 p = p_max - p_min;

        const int col0_r = (int)(col0 >> IM_COL32_R_SHIFT) & 0xFF;
        const int col0_g = (int)(col0 >> IM_COL32_G_SHIFT) & 0xFF;
        const int col0_b = (int)(col0 >> IM_COL32_B_SHIFT) & 0xFF;
        const int col0_a = (int)(col0 >> IM_COL32_B_SHIFT) & 0xFF;
        const int col_delta_r = ((int)(col1 >> IM_COL32_R_SHIFT) & 0xFF) - col0_r;
        const int col_delta_g = ((int)(col1 >> IM_COL32_G_SHIFT) & 0xFF) - col0_g;
        const int col_delta_b = ((int)(col1 >> IM_COL32_B_SHIFT) & 0xFF) - col0_b;
        const int col_delta_a = ((int)(col1 >> IM_COL32_A_SHIFT) & 0xFF) - col0_a;

        for (ImDrawVert* vert = vert_start; vert < vert_end; vert++)
        {
            float d = vert->pos.x - p_min.x;
            float v;
            if (d == 0.f)
                v = 1.f;
            else
                v = 1.f - (p_max.x - p_min.x) / d;
            float t = v;

            int r = (int)(col0_r + col_delta_r * t);
            int g = (int)(col0_g + col_delta_g * t);
            int b = (int)(col0_b + col_delta_b * t);
            int a = (int)(col0_a + col_delta_a * t);

            vert->col = (r << IM_COL32_R_SHIFT) | (g << IM_COL32_G_SHIFT) | (b << IM_COL32_B_SHIFT) | (a << IM_COL32_A_SHIFT);
        }
    }

    template<>
    void RenderEffect<GradientType::VERTICAL>(ImDrawVert* vert_start, ImDrawVert* vert_end, const ImVec2& p_min, const ImVec2& p_max, ImU32 col0, ImU32 col1)
    {
        //ImVec2 p = p_max - p_min;

        const int col0_r = (int)(col0 >> IM_COL32_R_SHIFT) & 0xFF;
        const int col0_g = (int)(col0 >> IM_COL32_G_SHIFT) & 0xFF;
        const int col0_b = (int)(col0 >> IM_COL32_B_SHIFT) & 0xFF;
        const int col0_a = (int)(col0 >> IM_COL32_B_SHIFT) & 0xFF;
        const int col_delta_r = ((int)(col1 >> IM_COL32_R_SHIFT) & 0xFF) - col0_r;
        const int col_delta_g = ((int)(col1 >> IM_COL32_G_SHIFT) & 0xFF) - col0_g;
        const int col_delta_b = ((int)(col1 >> IM_COL32_B_SHIFT) & 0xFF) - col0_b;
        const int col_delta_a = ((int)(col1 >> IM_COL32_A_SHIFT) & 0xFF) - col0_a;

        for (ImDrawVert* vert = vert_start; vert < vert_end; vert++)
        {
            float d = vert->pos.y - p_min.y;
            float v;
            if (d == 0.f)
                v = 1.f;
            else
                v = 1.f - (p_max.y - p_min.y) / d;

            float t = v;

            int r = (int)(col0_r + col_delta_r * t);
            int g = (int)(col0_g + col_delta_g * t);
            int b = (int)(col0_b + col_delta_b * t);
            int a = (int)(col0_a + col_delta_a * t);

            vert->col = (r << IM_COL32_R_SHIFT) | (g << IM_COL32_G_SHIFT) | (b << IM_COL32_B_SHIFT) | (a << IM_COL32_A_SHIFT);
        }
    }

    template<>
    void RenderEffect<GradientType::DIAGONAL_LB_RT>(ImDrawVert* vert_start, ImDrawVert* vert_end, const ImVec2& p_min, const ImVec2& p_max, ImU32 col0, ImU32 col1)
    {
        ImVec2 p = p_max - p_min;

        const int col0_r = (int)(col0 >> IM_COL32_R_SHIFT) & 0xFF;
        const int col0_g = (int)(col0 >> IM_COL32_G_SHIFT) & 0xFF;
        const int col0_b = (int)(col0 >> IM_COL32_B_SHIFT) & 0xFF;
        const int col0_a = (int)(col0 >> IM_COL32_B_SHIFT) & 0xFF;
        const int col_delta_r = ((int)(col1 >> IM_COL32_R_SHIFT) & 0xFF) - col0_r;
        const int col_delta_g = ((int)(col1 >> IM_COL32_G_SHIFT) & 0xFF) - col0_g;
        const int col_delta_b = ((int)(col1 >> IM_COL32_B_SHIFT) & 0xFF) - col0_b;
        const int col_delta_a = ((int)(col1 >> IM_COL32_A_SHIFT) & 0xFF) - col0_a;

        for (ImDrawVert* vert = vert_start; vert < vert_end; vert++)
        {
            ImVec2 d = vert->pos - p_min;

            ImVec2 uv;

            if (d.x == 0.f)
                uv.x = 0.f;
            else
                uv.x = d.x / p.x;

            if (d.y == 0.f)
                uv.y = 1.f;
            else
                uv.y = 1.f - d.y / p.y;

            float t = (uv.x + uv.y) / 2.f;

            int r = (int)(col0_r + col_delta_r * t);
            int g = (int)(col0_g + col_delta_g * t);
            int b = (int)(col0_b + col_delta_b * t);
            int a = (int)(col0_a + col_delta_a * t);

            vert->col = (r << IM_COL32_R_SHIFT) | (g << IM_COL32_G_SHIFT) | (b << IM_COL32_B_SHIFT) | (a << IM_COL32_A_SHIFT);
        }
    }

    template<>
    void RenderEffect<GradientType::DIAGONAL_LT_RB>(ImDrawVert* vert_start, ImDrawVert* vert_end, const ImVec2& p_min, const ImVec2& p_max, ImU32 col0, ImU32 col1)
    {
        ImVec2 p = p_max - p_min;

        const int col0_r = (int)(col0 >> IM_COL32_R_SHIFT) & 0xFF;
        const int col0_g = (int)(col0 >> IM_COL32_G_SHIFT) & 0xFF;
        const int col0_b = (int)(col0 >> IM_COL32_B_SHIFT) & 0xFF;
        const int col0_a = (int)(col0 >> IM_COL32_B_SHIFT) & 0xFF;
        const int col_delta_r = ((int)(col1 >> IM_COL32_R_SHIFT) & 0xFF) - col0_r;
        const int col_delta_g = ((int)(col1 >> IM_COL32_G_SHIFT) & 0xFF) - col0_g;
        const int col_delta_b = ((int)(col1 >> IM_COL32_B_SHIFT) & 0xFF) - col0_b;
        const int col_delta_a = ((int)(col1 >> IM_COL32_A_SHIFT) & 0xFF) - col0_a;

        for (ImDrawVert* vert = vert_start; vert < vert_end; vert++)
        {
            ImVec2 d = vert->pos - p_min;

            ImVec2 uv;

            if (d.x == 0.f)
                uv.x = 0.f;
            else
                uv.x = d.x / p.x;

            if (d.y == 0.f)
                uv.y = 1.f;
            else
                uv.y = 1.f - d.y / p.y;

            float t = (uv.x + uv.y) / 2.f;

            int r = (int)(col0_r + col_delta_r * t);
            int g = (int)(col0_g + col_delta_g * t);
            int b = (int)(col0_b + col_delta_b * t);
            int a = (int)(col0_a + col_delta_a * t);

            vert->col = (r << IM_COL32_R_SHIFT) | (g << IM_COL32_G_SHIFT) | (b << IM_COL32_B_SHIFT) | (a << IM_COL32_A_SHIFT);
        }
    }

    template<>
    void RenderEffect<GradientType::RADIAL>(ImDrawVert* vert_start, ImDrawVert* vert_end, const ImVec2& p_min, const ImVec2& p_max, ImU32 col0, ImU32 col1)
    {
        ImVec2 p = p_max - p_min;

        const int col0_r = (int)(col0 >> IM_COL32_R_SHIFT) & 0xFF;
        const int col0_g = (int)(col0 >> IM_COL32_G_SHIFT) & 0xFF;
        const int col0_b = (int)(col0 >> IM_COL32_B_SHIFT) & 0xFF;
        const int col0_a = (int)(col0 >> IM_COL32_B_SHIFT) & 0xFF;
        const int col_delta_r = ((int)(col1 >> IM_COL32_R_SHIFT) & 0xFF) - col0_r;
        const int col_delta_g = ((int)(col1 >> IM_COL32_G_SHIFT) & 0xFF) - col0_g;
        const int col_delta_b = ((int)(col1 >> IM_COL32_B_SHIFT) & 0xFF) - col0_b;
        const int col_delta_a = ((int)(col1 >> IM_COL32_A_SHIFT) & 0xFF) - col0_a;

        for (ImDrawVert* vert = vert_start; vert < vert_end; vert++)
        {
            ImVec2 d = vert->pos - p_min;
            
            ImVec2 uv;

            if (d.x == 0.f)
                uv.x = 0.f;
            else
                uv.x = d.x / p.x;

            if (d.y == 0.f)
                uv.y = 1.f;
            else
                uv.y = 1.f - d.y / p.y;

            ImVec2 uvc = ImVec2(0.5f, 0.5f);

            float t = ImDot(uvc - uv, uvc - uv) / ImDot(uvc, uvc);

            int r = (int)(col0_r + col_delta_r * t);
            int g = (int)(col0_g + col_delta_g * t);
            int b = (int)(col0_b + col_delta_b * t);
            int a = (int)(col0_a + col_delta_a * t);

            vert->col = (r << IM_COL32_R_SHIFT) | (g << IM_COL32_G_SHIFT) | (b << IM_COL32_B_SHIFT) | (a << IM_COL32_A_SHIFT);
        }
    }

    // https://github.com/ocornut/imgui/issues/5483
// Assuming col_out's alpha is zero means we don't need to draw and outer AA fringe as AddCircleFilled() would.
    void AddRadialGradient(ImDrawList* draw_list, const ImVec2& center, float radius, ImU32 col_in, ImU32 col_out)
    {
        if (((col_in | col_out) & IM_COL32_A_MASK) == 0 || radius < 0.5f)
            return;

        // Use arc with automatic segment count
        draw_list->_PathArcToFastEx(center, radius, 0, IM_DRAWLIST_ARCFAST_SAMPLE_MAX, 0);
        const int count = draw_list->_Path.Size - 1;

        unsigned int vtx_base = draw_list->_VtxCurrentIdx;
        draw_list->PrimReserve(count * 3, count + 1);

        // Submit vertices
        const ImVec2 uv = draw_list->_Data->TexUvWhitePixel;
        draw_list->PrimWriteVtx(center, uv, col_in);
        for (int n = 0; n < count; n++)
            draw_list->PrimWriteVtx(draw_list->_Path[n], uv, col_out);

        // Submit a fan of triangles
        for (int n = 0; n < count; n++)
        {
            draw_list->PrimWriteIdx((ImDrawIdx)(vtx_base));
            draw_list->PrimWriteIdx((ImDrawIdx)(vtx_base + 1 + n));
            draw_list->PrimWriteIdx((ImDrawIdx)(vtx_base + 1 + ((n + 1) % count)));
        }
        draw_list->_Path.Size = 0;
    }


    // Render a rectangle shaped with optional rounding and borders
    void RenderFrameNew(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, bool border, float rounding, ImU32 col0, ImU32 col1)
    {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;
        int vert_start_idx = window->DrawList->VtxBuffer.Size;
        window->DrawList->AddRectFilled(p_min, p_max, fill_col, rounding);
        int vert_end_idx = window->DrawList->VtxBuffer.Size;

        ImDrawVert* vert_start = window->DrawList->VtxBuffer.Data + vert_start_idx;
        ImDrawVert* vert_end = window->DrawList->VtxBuffer.Data + vert_end_idx;

        RenderEffect<GradientType::DIAGONAL_LB_RT>(vert_start, vert_end, p_min, p_max, col0, col1);

        const float border_size = g.Style.FrameBorderSize;
        if (border && border_size > 0.0f)
        {
            ImVec2 gradient_extent = p_max - p_min;
            float gradient_inv_length2 = 1.0f / ImLengthSqr(gradient_extent);

            const int col0_r = (int)(col0 >> IM_COL32_R_SHIFT) & 0xFF;
            const int col0_g = (int)(col0 >> IM_COL32_G_SHIFT) & 0xFF;
            const int col0_b = (int)(col0 >> IM_COL32_B_SHIFT) & 0xFF;
            const int col_delta_r = ((int)(col1 >> IM_COL32_R_SHIFT) & 0xFF) - col0_r;
            const int col_delta_g = ((int)(col1 >> IM_COL32_G_SHIFT) & 0xFF) - col0_g;
            const int col_delta_b = ((int)(col1 >> IM_COL32_B_SHIFT) & 0xFF) - col0_b;

            //window->DrawList->AddRect(p_min + ImVec2(1, 1), p_max + ImVec2(1, 1), GetColorU32(ImGuiCol_BorderShadow), rounding, 0, border_size);
            int vert_start_idx = window->DrawList->VtxBuffer.Size;
            window->DrawList->AddRect(p_min, p_max, GetColorU32(ImGuiCol_Border), rounding, 0, border_size);
            int vert_end_idx = window->DrawList->VtxBuffer.Size;

            ImDrawVert* vert_start = window->DrawList->VtxBuffer.Data + vert_start_idx;
            ImDrawVert* vert_end = window->DrawList->VtxBuffer.Data + vert_end_idx;

            
            
        }
    }

    void RenderNavHighlightNew(const ImRect& bb, ImGuiID id, ImGuiNavHighlightFlags flags)
    {
        ImGuiContext& g = *GImGui;
        if (id != g.NavId)
            return;
        if (g.NavDisableHighlight && !(flags & ImGuiNavHighlightFlags_AlwaysDraw))
            return;
        ImGuiWindow* window = g.CurrentWindow;
        if (window->DC.NavHideHighlightOneFrame)
            return;

        float rounding = (flags & ImGuiNavHighlightFlags_NoRounding) ? 0.0f : g.Style.FrameRounding;
        ImRect display_rect = bb;
        display_rect.ClipWith(window->ClipRect);
        if (flags & ImGuiNavHighlightFlags_TypeDefault)
        {
            const float THICKNESS = 2.0f;
            const float DISTANCE = 3.0f + THICKNESS * 0.5f;
            display_rect.Expand(ImVec2(DISTANCE, DISTANCE));
            bool fully_visible = window->ClipRect.Contains(display_rect);
            if (!fully_visible)
                window->DrawList->PushClipRect(display_rect.Min, display_rect.Max);
            window->DrawList->AddRect(display_rect.Min + ImVec2(THICKNESS * 0.5f, THICKNESS * 0.5f), display_rect.Max - ImVec2(THICKNESS * 0.5f, THICKNESS * 0.5f), GetColorU32(ImGuiCol_NavHighlight), rounding, 0, THICKNESS);
            if (!fully_visible)
                window->DrawList->PopClipRect();
        }
        if (flags & ImGuiNavHighlightFlags_TypeThin)
        {
            window->DrawList->AddRect(display_rect.Min, display_rect.Max, GetColorU32(ImGuiCol_NavHighlight), rounding, 0, 1.0f);
        }
    }

    void RenderTextClippedNew(const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, const ImVec2& align, const ImRect* clip_rect)
    {
        // Hide anything after a '##' string
        const char* text_display_end = FindRenderedTextEnd(text, text_end);
        const int text_len = (int)(text_display_end - text);
        if (text_len == 0)
            return;

        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;
        RenderTextClippedEx(window->DrawList, pos_min, pos_max, text, text_display_end, text_size_if_known, align, clip_rect);
        if (g.LogEnabled)
            LogRenderedText(&pos_min, text, text_display_end);
    }

    bool ButtonNew(const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags)
    {
        ImGuiWindow* window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID(label);
        const ImVec2 label_size = CalcTextSize(label, NULL, true);

        ImVec2 pos = window->DC.CursorPos;
        if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
            pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
        ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

        const ImRect bb(pos, pos + size);
        ItemSize(size, style.FramePadding.y);
        if (!ItemAdd(bb, id))
            return false;

        if (g.LastItemData.InFlags & ImGuiItemFlags_ButtonRepeat)
            flags |= ImGuiButtonFlags_Repeat;

        bool hovered, held;
        bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

        const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);

        //RenderNavHighlight(bb, id);
        RenderFrameNew(bb.Min, bb.Max, col, true, style.FrameRounding, IM_COL32(140, 140, 140, 200), IM_COL32(30, 30, 30, 200));

        if (g.LogEnabled)
            LogSetNextTextDecoration("[", "]");
        RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);

        // Automatically close popups
        //if (pressed && !(flags & ImGuiButtonFlags_DontClosePopups) && (window->Flags & ImGuiWindowFlags_Popup))
        //    CloseCurrentPopup();

        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
        return pressed;
    }

}