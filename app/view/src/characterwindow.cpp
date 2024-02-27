#include "characterwindow.h"
#include "ui_characterwindow.h"

#include "Application.h"
#include "ApplicationView.h"
#include "mainwindow.h"

#include "stuffsview.h"

CharacterWindow::CharacterWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::CharacterWindow) {
  ui->setupUi(this);
  ui->tabWidget->setCurrentIndex(static_cast<int>(tabType::character));

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

void CharacterWindow::InitWindow(const tabType &type) {
  ui->tabWidget->setCurrentIndex(static_cast<int>(type));
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
  const tabType type = static_cast<tabType>(ui->tabWidget->currentIndex());
  if (type == tabType::attak) {
    ui->edit_atk_tab->Save();
  }
  if (type == tabType::character) {
    ui->character_def->AddCharacter(m_CurCharacter);
    emit SigNewCharacter(m_CurCharacter);
  }

  if (type == tabType::stuff) {
    EditStuff es = ui->edit_stuff_view->Save();
    Application::GetInstance()
        .m_GameManager->m_PlayersManager
        ->m_Equipments[es.m_BodyPart][es.m_Name] = es.m_Stuff;
    ui->use_stuff_view->AddItemInComboBox(es);
    emit SigAddNewStuff();
  }

  if (type == tabType::useStuff) {
    Application::GetInstance()
        .m_GameManager->m_PlayersManager->LoadAllEquipmentsJson();
  }
}

void CharacterWindow::on_apply_pushButton_clicked() {
  const tabType type = static_cast<tabType>(ui->tabWidget->currentIndex());
  if (type == tabType::stuff) {
    EditStuff es = ui->edit_stuff_view->Save();
    Application::GetInstance()
        .m_GameManager->m_PlayersManager
        ->m_Equipments[es.m_BodyPart][es.m_Name] = es.m_Stuff;
    ui->use_stuff_view->AddItemInComboBox(es);
    emit SigAddNewStuff();
  }
  if (type == tabType::useStuff) {
    const auto &table = ui->use_stuff_view->GetCurrentEquipmentTable();
    auto *ch = Application::GetInstance()
                   .m_GameManager->m_PlayersManager->m_SelectedHero;
    if (ch != nullptr) {
      ch->SetEquipment(table);
      ch->ApplyEquipOnStats();
      // vh->UpdateEquipmentJson();
      emit SigUseNewStuff(ch->m_Name);
    }
  }
}
