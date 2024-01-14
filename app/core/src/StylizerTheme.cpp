#include "StylizerTheme.h"

#include <map>

StylizerTheme StylizerSelector::CreateLightTheme()
{
    StylizerTheme defaultTheme;
    return defaultTheme;
}

StylizerTheme StylizerSelector::CreateDarkTheme()
{
    StylizerTheme dt;
    dt.menu_bar_text = {GREY_DARK};
    dt.menu_bar_bg   = {BLACK_LIGHT};

    dt.menu_text       = {WHITE};
    dt.menu_text_sec   = {GREY_DARK};
    dt.menu_bg         = {BLUISH_GREY_DARK};
    dt.menu_border     = {BLUE_CYAN};
    dt.menu_bg_hover   = {BLUISH_GREY};
    dt.menu_bg_pressed = {BLUISH_GREY_LIGHT};

    dt.calendar_available     = {BLUISH_GREY_LIGHTER};
    dt.calendar_availableNext = {GREY_DARK};
    dt.calendar_selected      = {BLUE_CYAN};
    dt.calendar_selected_txt  = {WHITE};

    dt.text_color_normal     = {WHITE};
    dt.text_color_selected   = {BLUE_CYAN};
    dt.text_color_disabled   = {GREY_DARKER};
    dt.text_color_indication = {GREY_DARK};

    dt.bg_default      = {BLUISH_GREY};
    dt.bg_alternative  = {BLUISH_GREY_LIGHT};
    dt.bg_alternative2 = {BLUISH_GREY_DARK};
    dt.list_bg         = {BLUISH_GREY_DARK};
    dt.list_border     = {BLUISH_GREY_DARK};

    dt.separator_color     = {BLACK_LIGHT};
    dt.separator_color_2nd = {GREY_DARKER};

    dt.bg_slider     = {BLUISH_GREY_DARK};
    dt.handle_slider = {GREY_DARKER};

    dt.btn_border     = {GREY_DARKER};
    dt.btn_text       = {GREY_LIGHT};
    dt.btn_text_dis   = {GREY_DARKER};
    dt.btn_bg         = {BLUISH_GREY};
    dt.btn_bg_hover   = {BLUISH_GREY_DARK};
    dt.btn_bg_pressed = {BLUISH_GREY};
    dt.btn_bg_dis     = {BLUISH_GREY_LIGHT};

    dt.btn_ok_border     = {BLUE_CYAN};
    dt.btn_ok_text       = {WHITE};
    dt.btn_ok_text_dis   = {WHITE};
    dt.btn_ok_bg         = {BLUE_CYAN};
    dt.btn_ok_bg_hover   = {BLUE_LIGHT};
    dt.btn_ok_bg_pressed = {BLUE_LIGHTER};
    dt.btn_ok_bg_dis     = {GREY_DARK};

    dt.combobox_background = {BLACK_LIGHT};

    dt.slider_handle   = ":/icons/cursor.svg";
    dt.slider_handle_v = ":/icons/cursor-round.svg";
    dt.drop_up_icon    = ":/icons/dropup.svg";
    dt.hand_icon       = ":/icons/move.svg";
    dt.zoom_in_icon    = ":/icons/zoom-in.svg";
    dt.zoom_out_icon   = ":/icons/zoom-out.svg";
    dt.dot_rect_icon   = ":/icons/region.svg";

    dt.radio_button          = ":/icons/radio-white.svg";
    dt.radio_button_checked  = ":/icons/radio-checked-white.svg";
    dt.radio_button_disabled = ":/icons/radio-grey.svg";

    return dt;
}

StylizerSelector::THEMES StylizerSelector::StringToTheme(const std::string& key)
{
    static std::map<std::string, THEMES> EXISTING_THEMES = {{"DARK", THEMES::DARK}, {"LIGHT", THEMES::LIGHT}};

    auto it = EXISTING_THEMES.find(key);
    return it != EXISTING_THEMES.end() ? it->second : THEMES::UNDEFINED;
}

std::string StylizerSelector::ThemeToString(StylizerSelector::THEMES theme)
{
    static std::map<THEMES, std::string> EXISTING_THEMES = {{THEMES::DARK, "DARK"}, {THEMES::LIGHT, "LIGHT"}};
    auto it                                              = EXISTING_THEMES.find(theme);
    return it != EXISTING_THEMES.end() ? it->second : "DARK";
}
