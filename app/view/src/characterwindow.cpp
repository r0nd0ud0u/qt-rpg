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
  connect(this, &CharacterWindow::SigAddNewStuff,
          ApplicationView::GetInstance().GetMainWindow(),
          &MainWindow::AddNewStuff);
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
  ui->edit_atk_tab->InitDefaultView();
  // use stuff
  ui->use_stuff_view->InitView(c);
  // character + stats
  ui->character_def->Init(c);

  if (type == tabType::attak) {
    ui->tabWidget->setTabEnabled(1, true);
    ui->tabWidget->setTabEnabled(2, true);
  }
  if (type == tabType::character) {
    ui->tabWidget->setTabEnabled(1, false);
    ui->tabWidget->setTabEnabled(2, false);
    if (m_CurCharacter == nullptr) {
      m_CurCharacter = new Character();
    }
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
  if(m_CurCharacter->m_Name.isEmpty()){
      return;
  }
  auto *pm = Application::GetInstance().m_GameManager->m_PlayersManager;
  // atk
  ui->edit_atk_tab->Save();
  // stats + character
  // update stats
  ui->character_def->AddCharacter(m_CurCharacter);
  // output json update on character
  pm->OutputCharactersInJson(std::vector<Character *>{m_CurCharacter});
  // stuff
  EditStuff es = ui->edit_stuff_view->Save();
  if (!es.m_Name.isEmpty()) {
    // name is empty at launch of the window
    // and is reset to empty after one apply
    // this way , no duplicate if "button apply" + "button save" pressed
    // TODO handle behavior enable/disable apply button
    pm->m_Equipments[es.m_BodyPart][es.m_Name] = es.m_Stuff;
    ui->use_stuff_view->AddItemInComboBox(es);
    emit SigAddNewStuff();
  }
  // Init/Update the stats of character first
  // Then update the equipments
  // use stuff
  // first synchronize the pm table
  pm->LoadAllEquipmentsJson();
  // Then update the equipments for each character
  const auto &table = ui->use_stuff_view->GetCurrentEquipmentTable();
  m_CurCharacter->SetEquipment(table);
  m_CurCharacter->ApplyEquipOnStats(
      pm->m_AllEffectsOnGame[m_CurCharacter->m_Name]);
  m_CurCharacter->UpdateEquipmentOnJson();
  emit SigUseNewStuff(m_CurCharacter->m_Name);
  // update panel
  emit SigUpdateCharacterViews(m_CurCharacter);
}

void CharacterWindow::UpdateView(const std::vector<EditStuff> &esTable) {
  for (const auto &es : esTable) {
    ui->use_stuff_view->AddItemInComboBox(es);
  }
}
