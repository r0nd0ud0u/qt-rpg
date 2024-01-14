#ifndef STYLIZER_H
#define STYLIZER_H

#include "StylizerTheme.h"

/// Stylizer contains statics methods to be called to apply visal theme
class Stylizer
{
public:
    static StylizerTheme& GetTheme() { return m_theme; }                       ///< Get current theme
    static StylizerSelector::THEMES GetThemeName() { return m_themeSelector; } ///< Get current theme (enum)
    static void SetTheme(StylizerSelector::THEMES theme);                      ///< Set theme used, need to be applied after
    static void ApplyTheme(QWidget* widgetToChange);                           ///< Apply on a window and all its children
    static void ApplyButtonTheme(QWidget* widgetToChange);                     ///< Apply on a window and all its children

private:
    Stylizer();
    static StylizerTheme m_theme;
    static StylizerSelector::THEMES m_themeSelector;
};

#endif // STYLIZER_H
