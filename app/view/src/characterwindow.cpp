#include "characterwindow.h"
#include "ui_characterwindow.h"

#include "ApplicationView.h"
#include "mainwindow.h"

CharacterWindow::CharacterWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::CharacterWindow) {
  ui->setupUi(this);
  ui->tabWidget->setCurrentIndex(0);

  connect(this, &CharacterWindow::SigNewCharacter, ApplicationView::GetInstance().GetMainWindow(),
          &MainWindow::AddNewCharacter);
}

CharacterWindow::~CharacterWindow() { delete ui; }

void CharacterWindow::InitWindow(const actionType &action) {
  m_CurrentType = action;
  if (action == actionType::edit) {
    ui->edit_atk_tab->InitView();
    ui->tabWidget->setCurrentIndex(1);
    ui->tabWidget->setTabEnabled(1, true);
    ui->tabWidget->setTabEnabled(2, true);
  }
  if (action == actionType::newCharacter) {
    ui->tabWidget->setCurrentIndex(0);
    ui->edit_atk_tab->InitDefaultView();
    ui->tabWidget->setTabEnabled(1, false);
    ui->tabWidget->setTabEnabled(2, false);
    if (m_CurCharacter == nullptr) {
      m_CurCharacter = new Character();
    }
  }
}

void CharacterWindow::on_pushButton_clicked() {
  hide();
  if (m_CurrentType == actionType::edit) {
    ui->edit_atk_tab->Save();
  }
  if (m_CurrentType == actionType::newCharacter) {
    ui->character_def->AddCharacter(m_CurCharacter);
    emit SigNewCharacter(m_CurCharacter);
  }
}
