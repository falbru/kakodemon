#ifndef EDITORCONTROLLER_HPP_INCLUDED
#define EDITORCONTROLLER_HPP_INCLUDED

#include "application/model/kakouneclient.hpp"
#include "application/model/uioptions.hpp"
#include "application/view/kakounecontentview.hpp"
#include "application/view/statusbar.hpp"
#include "domain/color.hpp"
#include "domain/fontmanager.hpp"
#include "domain/mouse.hpp"
#include <functional>
#include <map>

class EditorController // TODO rename to KakouneContentViewController or something?
{
  public:
    EditorController();

    void init(KakouneClient *kakoune_client, KakouneContentView *kakoune_content_view, StatusBarView *status_bar_view,
              domain::FontManager *font_manager, std::function<void(domain::RGBAColor)> set_clear_color);
    void update(UIOptions &ui_options);
    void render(const UIOptions &ui_options);

    void onWindowResize(int width, int height, const UIOptions &ui_options);
    domain::MouseMoveResult onMouseMove(float x, float y, const UIOptions *ui_options, bool obscured);
    void onMouseButton(domain::MouseButtonEvent event, const UIOptions *ui_options, bool obscured);
    void onMouseScroll(int amount, float x, float y, const UIOptions *ui_options);

    int width() const;
    int height() const;

  private:
    KakouneClient *m_kakoune_client;
    KakouneContentView *m_kakoune_content_view;
    StatusBarView *m_status_bar_view;
    domain::FontManager *m_font_manager;

    std::function<void(domain::RGBAColor)> m_set_clear_color;
    void setClearColor(domain::RGBAColor color);

    std::map<domain::MouseButton, bool> m_mouse_button_pressed;

    int m_rows;
    int m_columns;
    int m_width;
    int m_height;
    int m_content_height;
};

#endif
