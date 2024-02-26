#include "characterwindow.h"
#include "ui_characterwindow.h"

#include "ApplicationView.h"
#include "mainwindow.h"

CharacterWindow::CharacterWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::CharacterWindow) {
  ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(static_cast<int>(tabType::character));

  connect(this, &CharacterWindow::SigNewCharacter, ApplicationView::GetInstance().GetMainWindow(),
          &MainWindow::AddNewCharacter);
}

CharacterWindow::~CharacterWindow() { delete ui; }

void CharacterWindow::InitWindow(const tabType &type) {
  if (type == tabType::attak) {
    ui->edit_atk_tab->InitView();
    ui->tabWidget->setCurrentIndex(static_cast<int>(type));
    ui->tabWidget->setTabEnabled(1, true);
    ui->tabWidget->setTabEnabled(2, true);
  }
  if (type == tabType::character) {
    ui->tabWidget->setCurrentIndex(static_cast<int>(type));
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
  const tabType type = static_cast<tabType>(ui->tabWidget->currentIndex());
  if (type == tabType::attak) {
    ui->edit_atk_tab->Save();
  }
  if (type == tabType::character) {
    ui->character_def->AddCharacter(m_CurCharacter);
    emit SigNewCharacter(m_CurCharacter);
  }
}
