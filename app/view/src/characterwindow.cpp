#include "characterwindow.h"
#include "ui_characterwindow.h"

#include "Application.h"
#include "ApplicationView.h"
#include "mainwindow.h"

#include "stuffsview.h"

CharacterWindow::CharacterWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::CharacterWindow) {
  ui->setupUi(this);

  connect(this, &CharacterWindow::SigUpdateCharacterViews,
          ApplicationView::GetInstance().GetMainWindow(),
          &MainWindow::UpdatCharacterViews);
  connect(this, &CharacterWindow::SigUseNewStuff,
          ApplicationView::GetInstance().GetMainWindow(),
          &MainWindow::UpdateStuffOnUse);
}

CharacterWindow::~CharacterWindow() {
  m_CurCharacter = nullptr;
  delete ui;
}

void CharacterWindow::InitWindow(const tabType &type, Character *c) {
  ui->tabWidget->setCurrentIndex(static_cast<int>(type));
  m_CurCharacter = c;
  // atk
  ui->edit_atk_tab->InitView(c);
  // character
  if (c == nullptr) {
    ui->edit_atk_tab->InitDefaultView();
  }
  // use stuff
  ui->use_stuff_view->InitView(c);
  // character + stats
  ui->character_def->Init(c);

  if (type == tabType::character && m_CurCharacter == nullptr) {
    m_CurCharacter = new Character();
  }
}

void CharacterWindow::on_pushButton_clicked() {
  hide();
  // atk + stats+ character + stuff + use stuff
  Apply();
  // reset cur character
  m_CurCharacter = nullptr;
}

void CharacterWindow::on_apply_pushButton_clicked() { Apply(); }

void CharacterWindow::Apply() {
  if (m_CurCharacter == nullptr) {
    return;
  }
  if (m_CurCharacter->m_Name.isEmpty()) {
    return;
  }
  auto *pm = Application::GetInstance().m_GameManager->m_PlayersManager;
  // atk
  if (ui->edit_atk_tab->isEnabled()) {
    ui->edit_atk_tab->Save();
  }
  // stats + character
  // update stats
  if (ui->character_def->isEnabled()) {
    //  use values stats without effect
    ui->character_def->AddCharacter(m_CurCharacter);
    // output json update on character
    pm->OutputCharactersInJson(std::vector<Character *>{m_CurCharacter},
                               OFFLINE_CHARACTERS);
  }

  // add new stuff
  if (ui->edit_stuff_view->isEnabled()) {
    if (const auto es = ui->edit_stuff_view->Save(); !es.m_Name.isEmpty()) {
      pm->m_Equipments[es.m_BodyPart][es.m_Name] = es.m_Stuff;
    }
  }

  // Init/Update the stats of character first
  // Then update the equipments
  // use stuff
  // first synchronize the pm table
  // Then update the equipments for each character
  const auto &table = ui->use_stuff_view->GetCurrentEquipmentTable();
  m_CurCharacter->SetEquipment(table);
  m_CurCharacter->ApplyEquipOnStats(
      pm->m_AllEffectsOnGame[m_CurCharacter->m_Name]);
  m_CurCharacter->UpdateEquipmentOnJson(OFFLINE_WEARING_EQUIPMENT);
  // update views
  // use stuff
  // TODO assess if a value has been change to perform the update view and the
  // initView
  UpdateViewUseStuff(pm->m_Equipments);
  ui->use_stuff_view->InitView(m_CurCharacter);
  emit SigUseNewStuff(m_CurCharacter->m_Name);
  // update panel
  emit SigUpdateCharacterViews(m_CurCharacter);
}

void CharacterWindow::UpdateViewUseStuff(
    const std::unordered_map<QString, std::map<QString, Stuff>> table) {
  ui->use_stuff_view->ResetUi();
  ui->use_stuff_view->UpdateView();
}

void CharacterWindow::UpdateViewAtGameStart() {
  ui->tabWidget->setTabVisible(static_cast<int>(tabType::character), false);
  ui->tabWidget->setTabVisible(static_cast<int>(tabType::attak), false);
  ui->tabWidget->setTabVisible(static_cast<int>(tabType::stuff), false);
  ui->tabWidget->setTabVisible(static_cast<int>(tabType::useStuff), true);
  ui->character_def->setEnabled(false);
  ui->edit_stuff_view->setEnabled(false);
  ui->edit_atk_tab->setEnabled(false);
  ui->use_stuff_view->setEnabled(true);
}
