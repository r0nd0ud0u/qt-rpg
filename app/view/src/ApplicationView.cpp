#include "ApplicationView.h"

ApplicationView::ApplicationView(QObject *parent) : QObject{parent} {}

ApplicationView::~ApplicationView() {
  if (m_main_window.get() != nullptr) {
    m_main_window->hide();
  }
  if (m_Character_Window.get() != nullptr) {
    m_Character_Window->hide();
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

void ApplicationView::StartCharacterWindow() {
  if (m_Character_Window == nullptr) {
    m_Character_Window = std::make_unique<CharacterWindow>();
    m_Character_Window->setWindowState(Qt::WindowMaximized);
    m_Character_Window->setWindowModality(Qt::WindowModality::ApplicationModal);
    m_Character_Window->hide();
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
