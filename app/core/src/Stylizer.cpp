#include "Stylizer.h"

#include <QWidget>

#define th Stylizer::m_theme

StylizerTheme Stylizer::m_theme;
StylizerSelector::THEMES Stylizer::m_themeSelector;

void Stylizer::SetTheme(StylizerSelector::THEMES theme)
{
    switch (theme)
    {
    case StylizerSelector::THEMES::LIGHT:
    case StylizerSelector::THEMES::UNDEFINED:
        th = StylizerSelector::CreateLightTheme();
        break;
    case StylizerSelector::THEMES::DARK:
        th = StylizerSelector::CreateDarkTheme();
        break;
    default:
	break;
    }
    Stylizer::m_themeSelector = theme;
}

void Stylizer::ApplyTheme(QWidget* widgetToChange)
{
    // General
    QString style_general("  \
        CompMenu, CompMenu::item { \
            color:                  %1; \
            background:             %2; \
            font-size: 8pt; \
        } \
        QMainWindow, QLabel, QCheckBox, QRadioButton, QGroupBox, \
        QDateTimeEdit, QTimeEdit, QListView, QTableView, QStatusBar, QProgressBar, #btn_new_mark { \
            color:                  %3; \
        } \
        CompTitleLabel { \
            font-size: 14pt;\
        } \
        QMainWindow, QMessageBox, QColorDialog, QDialog, QTabWidget, QTabWidget > QWidget { \
          background:               %4; \
        } \
        QTabWidget::pane { \
          margin-top: -1px; \
          border-top: 1px solid #999999; \
        } \
        QTabBar::tab { \
            padding: 8px; \
        } \
        QTabBar::tab:!selected { \
            margin-top: 2px; \
            margin-bottom: 1px; \
        } \
        CompTabButton, QTabBar::tab { \
            color:                    %34; \
            background:               %6; \
        } \
        CompTabButton { \
            border-radius:0; \
            border-top-left-radius:4px; \
            border-top-right-radius:4px; \
            border:0; \
            padding-top:-2px; \
        } \
        CompTabButtonIcon { \
            color:                    %5; \
            border-radius:0; \
            font-size : 8pt; \
            padding-top:15px; \
        } \
        CompTabMinButton { \
            font-size : 10pt; \
        } \
        CompTabButton:hover { background:       %8;  } \
        CompTabButton:disabled, QTabBar::tab:selected { \
            background:             %9; \
            border-bottom : 2px solid %7; \
        } \
        CompTabButton:pressed { background:     %10; } \
        CompIndicationLabel { color :           %13; } \
        CompWidgetTabs CompIndicationLabel { color :  %35; } \
        CompWidget1 { border:0; } \
        CompWidget1, QGroupBox { background:   %14; } \
        CompWidget2, QDateTimeEdit, QTimeEdit, QTableView, QStatusBar { border:0; background:   %15; } \
        QListView, QTreeView { border:1px solid %37; background:   %36; } \
        QTreeView::branch {  border-image: url(none.png);} \
        CompWidgetTabs, CompTabLabel  { \
            background:             %12; \
        } \
        QFrame, QDateTimeEdit, QTimeEdit, CompScrollList { border-bottom : 1px solid %16; } \
        CompTabLabel { \
            color:                  %11; \
            font-size:14pt; \
            padding-right:20px; \
            border-radius:0; \
            border:0; \
        } \
        CompWidgetBtmBorder { border : 0; border-bottom : 1px solid %33; } \
        QColorDialog QPushButton, QDialogButtonBox QPushButton, CompPrimaryButton, CompSecondaryButton, QProgressDialog QPushButton { \
            border:1px solid        %17;  \
            color:                  %18;  \
            background:             %20;  \
            padding-left:15px;  \
            padding-right:15px;  \
            padding-top:4px;  \
            padding-bottom:4px;  \
            border-radius:3px;  \
        } \
        CompWidget1 CompSecondaryButton {background:   %31; } \
        CompWidget2 CompSecondaryButton {background:   %32; } \
        :disabled { color:          %19; } \
        CompSecondaryButton:disabled, CompSecondaryButton:checked, QComboBox:disabled, QLineEdit:disabled, QRadioButton:disabled, QCheckBox:disabled { \
            background:             %23;  \
        } \
        CompBigSecondaryButton { \
          padding-left:25px;  \
          padding-right:25px;  \
          padding-top:10px;  \
          padding-bottom:10px;  \
          border-radius:5px;  \
        } \
        QDialogButtonBox QPushButton:hover, CompSecondaryButton:hover { background:      %21; }  \
        QDialogButtonBox QPushButton:pressed, CompSecondaryButton:pressed { background:    %22; } \
        CompPrimaryButton { \
            border:0px solid        %24;  \
            color:                  %25;  \
            background:             %27;  \
        } \
        CompPrimaryButton:disabled {  \
            color:                  %26;  \
            background:             %30;  \
        } \
        CompPrimaryButton:hover { background:      %28; }  \
        CompPrimaryButton:pressed { background:    %29; } \
        CompBigPrimaryButton { \
            padding-left:25px;  \
            padding-right:25px;  \
            padding-top:10px;  \
            padding-bottom:10px;  \
            border-radius:5px;  \
        }");
    // we chain calls to arg, because of its max of 9 parameters
    style_general = style_general
                        .arg(th.menu_bar_text.name(), // 1
                            th.menu_bar_bg.name(),    // 2

                            th.text_color_normal.name(), // 3
                            th.bg_default.name())        // 4
                        .arg(th.menu_text_sec.name(),    // 5
                            th.menu_bg.name(),           // 6
                            th.menu_border.name(),       // 7
                            th.menu_bg_hover.name(),     // 8
                            th.menu_bg_pressed.name(),   // 9
                            th.menu_bg_pressed.name(),   // 10

                            th.menu_text_sec.name(),          // 11
                            th.menu_bg.name())                // 12
                        .arg(th.text_color_indication.name(), // 13

                            th.bg_alternative.name(),  // 14
                            th.bg_alternative2.name(), // 15

                            th.separator_color.name())   // 16
                        .arg(th.btn_border.name(),       // 17
                            th.btn_text.name(),          // 18
                            th.btn_text_dis.name(),      // 19
                            th.btn_bg.name(),            // 20
                            th.btn_bg_hover.name(),      // 21
                            th.btn_bg_pressed.name(),    // 22
                            th.btn_bg_dis.name())        // 23
                        .arg(th.btn_ok_border.name(),    // 24
                            th.btn_ok_text.name(),       // 25
                            th.btn_ok_text_dis.name(),   // 26
                            th.btn_ok_bg.name(),         // 27
                            th.btn_ok_bg_hover.name(),   // 28
                            th.btn_ok_bg_pressed.name(), // 29
                            th.btn_ok_bg_dis.name(),     // 30

                            th.bg_alternative.name(),       // 31
                            th.bg_alternative2.name())      // 32
                        .arg(th.separator_color_2nd.name(), //33
                            th.menu_text.name(),            // 34
                            th.menu_text.name(),            // 35
                            th.list_bg.name(),              // 36
                            th.list_border.name()           // 37
                        );

    // inputs
    QString style_input(" \
        QComboBox, QLineEdit, QTextEdit, QDoubleSpinBox, QSpinBox { \
            color:                  %4; \
        }\
        QComboBox, QLineEdit, QTextEdit { \
            border: 1px solid       %5; \
            border-radius: 4px; \
            padding: 2px 5px 2px 5px; \
            min-width: 1.5em; \
        } \
        #mark_list {\
            border-radius: 4px; \
            border: 0px solid black; \
            font-size: 8pt; \
            padding : 8px;\
            padding-left : 20px;\
        } \
        QColorDialog QLineEdit { \
            border-radius: 0px; \
            padding: 0px 0px 0px 0px; \
        } \
        QLineEdit, QTextEdit, QDoubleSpinBox, QSpinBox { \
            background:             %6; \
        } \
        QComboBox QAbstractItemView, QComboBox { \
            background:             %7; \
        } \
        QLineEdit:disabled, QTextEdit:disabled, QDoubleSpinBox:disabled, QSpinBox:disabled{ \
            background:             %25; \
        } \
        QComboBox QAbstractItemView{ \
            border : 0; \
            padding : 2px; \
        } \
        CompMarksComboBox { \
            border-top : 0px; \
            border-left : 0px; \
            border-right : 0px; \
            border-radius: 0px; \
        } \
        CompSearchInput, QDateTimeEdit, QTimeEdit { \
            border:0px; \
            border-radius: 0px; \
            border-bottom:1px solid %2; \
        } \
        QDateTimeEdit, QTimeEdit{ \
            color :                 %3; \
            background:rgba(0,0,0,0); \
            padding : 2px 20px 2px 4px; \
            font-weight: bold; \
        } \
        QDateTimeEdit::up-button, QTimeEdit::up-button { \
            width:0; \
            height:0; \
        } \
        QDateTimeEdit::down-button, QTimeEdit::down-button { \
            width:0; \
            height:0; \
        } \
        QComboBox:focus, QLineEdit:focus { \
            border-color:           %1; \
        } \
        QComboBox::drop-down { border:0; } \
        QComboBox::down-arrow {  image: url(%20); } \
        QComboBox::down-arrow:on { top: 1px; left: 1px;} \
        CompWidget1 CompSearchInput, CompWidget1 QComboBox {background:   %8; }\
        CompWidget2 CompSearchInput, CompWidget2 QComboBox {background:   %9; }\
        QSlider::groove:horizontal { \
            height: 1px;    \
            image: 0;\
        }   \
        QSlider::groove:vertical { \
            width: 1px;    \
            image: 0;\
        }   \
        QSlider::handle:horizontal:disabled, CompRangeHandle:disabled { \
            background:             url(%10) center no-repeat; \
        } \
        QSlider::handle, CompRangeHandle { \
            background:             url(%11) center no-repeat; \
        } \
        QSlider::handle:vertical { \
            background:             url(%21) center no-repeat; \
        } \
        QSlider::handle, CompRangeHandle { \
            border : 0; \
            width: 12px; \
            min-width: 12px; \
            max-width: 12px; \
            height: 16px; \
            min-height: 16px; \
            max-height: 16px; \
        } \
        QSlider::handle:vertical:disabled { \
            background:             url(%26) center no-repeat; \
        }    \
        QSlider::handle:horizontal{ \
            margin:-8px -0px; \
        }    \
        QSlider::handle:vertical{ \
            margin:-3px -6px; \
        }    \
        QSlider::sub-page:horizontal, QSlider::add-page:vertical { \
            background:             %12; \
        } \
        CompWidgetTabs QSlider::sub-page:horizontal { \
            background:             %19; \
        } \
        QSlider::add-page:horizontal, CompRangeSlider::sub-page:horizontal, QSlider::sub-page:vertical { \
            background:             %16; \
        } \
        QCheckBox::indicator { \
            width: 12px;        \
            height: 12px;       \
            border:1px solid        %13; \
        }   \
        QCheckBox::indicator:checked { \
            background:             %14; \
            border:1px solid        %15;  \
        }   \
        QCheckBox::indicator:checked:disabled { \
            background:             %17; \
            border:1px solid        %18;  \
        }   \
        QRadioButton::indicator \
        { \
            width: 12px; \
            height: 12px; \
        } \
        QRadioButton::indicator \
        { \
            image:                  url(%22); \
        } \
        QRadioButton::indicator::checked \
        { \
            image:                  url(%23); \
        } \
        QRadioButton::indicator:disabled \
        { \
            image:                  url(%24); \
        } \
          ");

    style_input = style_input
                      .arg(th.text_color_selected.name(), // 1
                          th.btn_border.name(),           // 2
                          th.text_color_selected.name(),  // 3
                          th.btn_text.name(),             // 4
                          th.btn_border.name(),           // 5
                          th.list_bg.name(),              // 6
                          th.combobox_background.name(),  // 7

                          th.bg_alternative.name(), // 8
                          th.bg_alternative2.name() // 9
                          )
                      .arg(th.slider_handle_disabled,    // 10
                          th.slider_handle,              // 11
                          th.text_color_selected.name(), // 12

                          th.btn_border.name(),          // 13
                          th.text_color_selected.name(), // 14
                          th.text_color_selected.name(), // 15
                          th.separator_color_2nd.name(), // 16
                          th.btn_border.name(),          // 17
                          th.separator_color_2nd.name()  // 18
                          )
                      .arg(th.menu_text.name(),     // 19
                          th.drop_up_icon,          // 20
                          th.slider_handle_v,       // 21
                          th.radio_button,          // 22
                          th.radio_button_checked,  // 23
                          th.radio_button_disabled, // 24
                          th.btn_ok_bg_dis.name(),  // 25
                          th.slider_handle_v_dis    // 26
                      );

    // lists
    QString style_lists(" \
        QScrollArea { \
            border:0; \
        } \
        QScrollBar { \
            border:1px solid        %1; \
            background:             %2; \
        } \
        QScrollBar::handle { \
            background:             %3; \
            border-radius: 2px; \
        } \
        QScrollBar:vertical { \
            width: 6px; \
        } \
        QScrollBar:horizontal { \
            height: 6px; \
        } \
        QScrollBar::add-line, QScrollBar::sub-line { \
            width:0;height:0 \
        } \
        QTableView { \
            selection-background-color:rgba(0,0,0,0); \
        } \
        QHeaderView::section { \
            font-size: 8pt; \
            background-color:       %4; \
            border: 0; \
            color:                 %5; \
            padding-bottom: 6px; \
        } \
        ");
    style_lists = style_lists.arg(th.separator_color.name(), // 1
        th.bg_slider.name(),                                 // 2
        th.handle_slider.name(),                             // 3
        th.bg_default.name(),                                // 4
        th.text_color_normal.name());

    // lists
    QString style_marks(" \
        CompMarkElement CompWidget1{ \
            border: 1px solid           %1; \
            border-left: 5px solid      %2; \
        } \
        CompMarkElement CompWidget2{ \
            border: 1px solid           %3; \
        } \
        CompMarkElement QPushButton { \
            color :                     %4; \
            border :0; \
            background:  rgba(0,0,0,0); \
        } \
        CompMarkElement QPushButton:hover { \
            color :                     %5; \
        } \
        #marks_list_frame > QFrame { \
            border:0; \
            margin-left : 2px; \
            border-left : 1px solid     %6; \
            min-height : 15px; \
        } \
        QCalendarWidget { font-size : 7pt; } \
        QCalendarWidget QWidget#qt_calendar_navigationbar, QCalendarWidget QToolButton { \
            color:                      %11; \
            background-color:           %7; \
        } \
        QCalendarWidget QWidget { alternate-background-color: %8;} \
        QCalendarWidget QAbstractItemView:enabled \
        { \
            color: %9 ; \
            selection-background-color: %10; \
        } \
        ");

    style_marks = style_marks
                      .arg(th.bg_alternative2.name(), // 1
                          th.btn_ok_bg.name(),        // 2
                          th.bg_alternative2.name(),  // 3
                          th.btn_text.name(),         // 4
                          th.btn_text_dis.name(),     // 5
                          th.btn_ok_bg.name(),        // 6
                          th.bg_alternative2.name(),  // 7
                          th.bg_alternative2.name(),  // 8
                          th.text_color_normal.name() // 9
                          )
                      .arg(th.text_color_selected.name(), // 10
                          th.text_color_indication.name() // 11
                      );
    QString style_audio("\
        CompListSlider{ \
            border-right:1px solid %1; \
            border-top:1px solid %2; \
            border-bottom:1px solid %3; \
            background : url(:/icons/greypx.png) center repeat-x; \
        } \
        CompListFirst{ \
            border-top-left-radius:3px; \
            border-bottom-left-radius:3px; \
            border-left:1px solid %4; \
        } \
        CompListLast{ \
            border-top-right-radius:3px; \
            border-bottom-right-radius:3px; \
            border-right:1px solid %5; \
        } \
        ");
    style_audio = style_audio.arg(th.separator_color.name(), // 1
        th.separator_color_2nd.name(),                       // 2
        th.separator_color_2nd.name(),                       // 3
        th.separator_color_2nd.name(),                       // 4
        th.separator_color_2nd.name()                        // 5
    );


    widgetToChange->setStyleSheet(style_general.append(style_input).append(style_lists).append(style_marks).append(style_audio));
}

void Stylizer::ApplyButtonTheme(QWidget* widgetToChange)
{
    QString style_general("  \
        CompSecondaryButton:disabled, QComboBox:disabled, QLineEdit:disabled, QRadioButton:disabled, QCheckBox:disabled { \
            background:             %23;  \
        } \
        CompBigSecondaryButton { \
          padding-left:25px;  \
          padding-right:25px;  \
          padding-top:10px;  \
          padding-bottom:10px;  \
          border-radius:5px;  \
        } \
        CompBigPrimaryButton { \
            border:0px solid        %24;  \
            color:                  %25;  \
            background:             %27;  \
            padding-left:25px;  \
            padding-right:25px;  \
            padding-top:10px;  \
            padding-bottom:10px;  \
            border-radius:5px;  \
        } \
        CompBigPrimaryButton:disabled {  \
            color:                  %26;  \
            background:             %30;  \
        } \
        CompBigPrimaryButton:hover { background:      %28; }  \
        CompBigPrimaryButton:pressed { background:    %29; }");
    // we chain calls to arg, because of its max of 9 param
    style_general = style_general.arg(th.btn_bg_dis.name(), // 23
        th.btn_ok_border.name(),                            // 24
        th.btn_ok_text.name(),                              // 25
        th.btn_ok_text_dis.name(),                          // 26
        th.btn_ok_bg.name(),                                // 27
        th.btn_ok_bg_hover.name(),                          // 28
        th.btn_ok_bg_pressed.name(),                        // 29
        th.btn_ok_bg_dis.name());                           // 30

    widgetToChange->setStyleSheet(style_general);
}
