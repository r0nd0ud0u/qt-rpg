#include "ApplicationView.h"

ApplicationView::ApplicationView(QObject *parent) : QObject{parent} {}

ApplicationView::~ApplicationView() {
  if (m_main_window.get() != nullptr) {
    m_main_window->hide();
  }
}

ApplicationView &ApplicationView::GetInstance() {
  static ApplicationView m_instance;
  return m_instance;
}

void ApplicationView::StartMainWindow() {
  if (m_main_window == nullptr) {
    m_main_window = std::make_unique<MainWindow>();
    m_main_window->setWindowState(Qt::WindowMaximized);
    m_main_window->show();
  }
}

void ApplicationView::ShowWindow(QMainWindow *mw, bool show) const {
  if (mw != nullptr) {
    if (show)
      mw->show();
    else
      mw->hide();
  }
}
