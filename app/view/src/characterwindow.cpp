#include "characterwindow.h"
#include "ui_characterwindow.h"

#include "Application.h"
#include "ApplicationView.h"
#include "mainwindow.h"

#include "stuffsview.h"

CharacterWindow::CharacterWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::CharacterWindow) {
  ui->setupUi(this);

  connect(this, &CharacterWindow::SigNewCharacter,
          ApplicationView::GetInstance().GetMainWindow(),
          &MainWindow::AddNewCharacter);
  connect(this, &CharacterWindow::SigAddNewStuff,
          ApplicationView::GetInstance().GetMainWindow(),
          &MainWindow::AddNewStuff);
  connect(this, &CharacterWindow::SigUseNewStuff,
          ApplicationView::GetInstance().GetMainWindow(),
          &MainWindow::UpdateStuffOnUse);
}

CharacterWindow::~CharacterWindow() { delete ui; }

void CharacterWindow::InitWindow(const tabType &type, const bool setIndex) {
  if (setIndex) {
    ui->tabWidget->setCurrentIndex(static_cast<int>(type));
  }
  if (type == tabType::attak) {
    ui->edit_atk_tab->InitView();
    ui->tabWidget->setTabEnabled(1, true);
    ui->tabWidget->setTabEnabled(2, true);
  }
  if (type == tabType::character) {
    ui->edit_atk_tab->InitDefaultView();
    ui->tabWidget->setTabEnabled(1, false);
    ui->tabWidget->setTabEnabled(2, false);
    if (m_CurCharacter == nullptr) {
      m_CurCharacter = new Character();
    }
  }
  const auto &pm = Application::GetInstance().m_GameManager->m_PlayersManager;
  if (type == tabType::useStuff) {
    ui->use_stuff_view->InitView(pm->m_SelectedHero);
  }
}

void CharacterWindow::on_pushButton_clicked() {
  hide();
  const auto type = static_cast<tabType>(ui->tabWidget->currentIndex());
  if (type == tabType::attak) {
    ui->edit_atk_tab->Save();
  }
  if (type == tabType::character) {
    ui->character_def->AddCharacter(m_CurCharacter);
    emit SigNewCharacter(m_CurCharacter);
  }

  Apply();
}

void CharacterWindow::on_apply_pushButton_clicked() {
    Apply();
}

void CharacterWindow::Apply(){
    const auto type = static_cast<tabType>(ui->tabWidget->currentIndex());
    auto *pm = Application::GetInstance().m_GameManager->m_PlayersManager;
    if (type == tabType::stuff) {
        EditStuff es = ui->edit_stuff_view->Save();
        pm->m_Equipments[es.m_BodyPart][es.m_Name] = es.m_Stuff;
        ui->use_stuff_view->AddItemInComboBox(es);
        emit SigAddNewStuff();
    }
    if (type == tabType::useStuff) {
        if (auto *ch = pm->m_SelectedHero; ch != nullptr) {
            const auto &table = ui->use_stuff_view->GetCurrentEquipmentTable();
            ch->SetEquipment(table);
            ch->ApplyEquipOnStats();
            ch->UpdateEquipmentOnJson();
            emit SigUseNewStuff(ch->m_Name);
        }
        pm->LoadAllEquipmentsJson();
    }
}

void CharacterWindow::on_tabWidget_currentChanged(int index) {
  ApplicationView::GetInstance().GetCharacterWindow()->InitWindow(
      static_cast<tabType>(index), false);
}
