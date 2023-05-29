
#include <imgui.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>

namespace ImGui
{
    void StyleColors();

    void LoadingIndicatorCircle(const char *label, const float indicator_radius, const ImVec4 &main_color, const ImVec4 &backdrop_color, const int circle_count, const float speed);

    bool BufferingBar(const char *label, float value, const ImVec2 &size_arg, const ImU32 &bg_col, const ImU32 &fg_col);

    bool Spinner(const char *label, float radius, int thickness, const ImU32 &color);

    bool ColoredButton(const char* label, const ImVec2& size, ImU32 text_color, ImU32 bg_color_1, ImU32 bg_color_2);

    bool HoverButton(const char* label, const ImVec2& size);

    bool Begin(const char* name, bool* p_open, ImGuiWindowFlags flags, ImU32 bg_color_1, ImU32 bg_color_2);

    bool ImageButton(ImTextureID user_texture_id, const ImVec2& size, const ImU32& col_1, const ImU32& col_2,  const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = -1, const ImVec4& tint_col = ImVec4(1, 1, 1, 1));
    bool ImageButton(const char* label, ImTextureID user_texture_id, const ImVec2& button_size, const ImVec2& image_size, const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), int frame_padding = -1);

    void RenderNavHighlightNew(const ImRect& bb, ImGuiID id, ImGuiNavHighlightFlags flags);
    void RenderFrameNew(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, bool border, float rounding, ImU32 col0, ImU32 col1);
    void RenderTextClippedNew(const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, const ImVec2& align, const ImRect* clip_rect);
    bool ButtonNew(const char* label, const ImVec2& size_arg, ImGuiButtonFlags flags);

}