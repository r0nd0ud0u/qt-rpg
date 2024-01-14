#ifndef STYLIZERTHEME_H
#define STYLIZERTHEME_H

#include <QColor>
#include <QFont>

#define WHITE 255, 255, 255
#define GREY_LIGHTER 242, 242, 242
#define GREY_LIGHT 224, 224, 224
#define GREY 189, 189, 189
#define GREY_DARK 150, 150, 150
#define GREY_DARKER 111, 111, 111
#define BLACK_LIGHT 27, 27, 27
#define BLACK 0, 0, 0

#define BLUISH_GREY_LIGHTER 79, 80, 84
#define BLUISH_GREY_LIGHT 59, 60, 64
#define BLUISH_GREY 52, 52, 56
#define BLUISH_GREY_DARK 45, 46, 51

#define BLUE_LIGHTERER 180, 200, 255
#define BLUE_LIGHTER 100, 150, 240
#define BLUE_LIGHT 73, 146, 219
#define BLUE 60, 122, 224

#define BLUE_CYAN 64, 177, 254


/// The StylizerTheme struct contains all data for one visual theme
struct StylizerTheme
{
    // Colors
    QColor menu_bar_text{WHITE};
    QColor menu_bar_bg{BLACK_LIGHT};

    QColor menu_text{WHITE};
    QColor menu_text_sec{GREY_LIGHT};
    QColor menu_bg{BLUE};
    QColor menu_border{GREY};
    QColor menu_bg_hover{BLUE_LIGHT};
    QColor menu_bg_pressed{BLUE_LIGHTER};

    QColor calendar_available{WHITE};
    QColor calendar_availableNext{BLUE_LIGHTERER};
    QColor calendar_selected{BLUE};
    QColor calendar_selected_txt{WHITE};

    QColor text_color_normal{GREY_DARKER};
    QColor text_color_selected{BLUE};
    QColor text_color_disabled{GREY};
    QColor text_color_indication{GREY_DARK};

    QColor bg_default{WHITE};
    QColor bg_alternative{GREY_LIGHTER};
    QColor bg_alternative2{GREY_LIGHT};
    QColor list_bg{WHITE};
    QColor list_border{GREY};

    QColor separator_color{GREY};
    QColor separator_color_2nd{GREY_DARK};

    QColor bg_slider{GREY};
    QColor handle_slider{GREY_DARKER};

    QColor btn_border{GREY};
    QColor btn_text{GREY_DARKER};
    QColor btn_text_dis{GREY};
    QColor btn_bg{WHITE};
    QColor btn_bg_hover{GREY_LIGHTER};
    QColor btn_bg_pressed{GREY_LIGHT};
    QColor btn_bg_dis{WHITE};

    QColor btn_ok_border{BLUE};
    QColor btn_ok_text{WHITE};
    QColor btn_ok_text_dis{WHITE};
    QColor btn_ok_bg{BLUE};
    QColor btn_ok_bg_hover{BLUE_LIGHT};
    QColor btn_ok_bg_pressed{BLUE_LIGHTER};
    QColor btn_ok_bg_dis{GREY};

    QColor combobox_background{WHITE};

    // Fonts
    QFont font_normal{"Nimbus Sans L", 9, QFont::Thin};

    // Icons
    QString slider_handle{":/icons/cursor-grey.svg"};
    QString slider_handle_disabled{":/icons/cursor-dis.svg"};
    QString slider_handle_v{":/icons/cursor-round-grey.svg"};
    QString slider_handle_v_dis{":/icons/close-grey.svg"};
    QString drop_up_icon{":/icons/dropup-grey.svg"};

    QString hand_icon{":/icons/move-grey.svg"};
    QString zoom_in_icon{":/icons/zoom-in-grey.svg"};
    QString zoom_out_icon{":/icons/zoom-out-grey.svg"};
    QString dot_rect_icon{":/icons/region-grey.svg"};

    QString radio_button{":/icons/radio-grey.svg"};
    QString radio_button_checked{":/icons/radio-checked-grey.svg"};
    QString radio_button_disabled{":/icons/radio-white.svg"};
};

namespace StylizerSelector {
enum class THEMES
{
    UNDEFINED,
    LIGHT,
    DARK
};                                ///< Different existing themes
StylizerTheme CreateLightTheme(); ///< Get the light colored StylizerTheme
StylizerTheme CreateDarkTheme();  ///< Get the dark colored StylizerTheme

THEMES StringToTheme(const std::string& key);
std::string ThemeToString(THEMES theme);
} // namespace StylizerSelector

#endif // STYLIZERTHEME_H
