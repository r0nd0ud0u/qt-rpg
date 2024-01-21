#ifndef APPLICATIONVIEW_H
#define APPLICATIONVIEW_H

#include <QObject>

#include "mainwindow.h"
#include "characterwindow.h"

#include <memory.h>

class ApplicationView : public QObject
{
    Q_OBJECT
public:
    static ApplicationView& GetInstance(); ///< Get singleton
    ~ApplicationView() override;

    MainWindow* GetMainWindow() { return m_main_window.get(); }
    CharacterWindow* GetCharacterWindow() { return m_Character_Window.get(); }
    void StartMainWindow();
    void StartCharacterWindow();
    void ShowWindow(QMainWindow* mw, bool show);

private:
    explicit ApplicationView(QObject* parent = nullptr);
    std::unique_ptr<MainWindow> m_main_window       = nullptr;
    std::unique_ptr<CharacterWindow> m_Character_Window       = nullptr;
};

#endif // APPLICATIONVIEW_H
