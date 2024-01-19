#ifndef APPLICATIONVIEW_H
#define APPLICATIONVIEW_H

#include <QObject>

#include "mainwindow.h"

#include <memory.h>

class ApplicationView : public QObject
{
    Q_OBJECT
public:
    static ApplicationView& GetInstance(); ///< Get singleton
    ~ApplicationView() override;

    std::unique_ptr<MainWindow>& GetMainWindow() { return m_main_window; }
    void StartMainWindow();

private:
    explicit ApplicationView(QObject* parent = nullptr);
    void ShowWindow(QMainWindow* mw, bool show) const;

    std::unique_ptr<MainWindow> m_main_window       = nullptr;
};

#endif // APPLICATIONVIEW_H
