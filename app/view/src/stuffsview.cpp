#include "stuffsview.h"
#include "ui_stuffsview.h"

#include "utils.h"

StuffsView::StuffsView(QWidget *parent)
    : QWidget(parent), ui(new Ui::StuffsView) {
  ui->setupUi(this);
  InitEditStuffsView();
}

StuffsView::~StuffsView() { delete ui; }

void StuffsView::InitEditStuffsView() {

  for (const auto &stats : ALL_STATS) {
    if (stats.isEmpty()) {
      continue;
    }
    auto *stuff = new StuffPanel();
    stuff->Init(stats);
    if (stuff == nullptr) {
      continue;
    }
    ui->scrollAreaWidgetContents->layout()->addWidget(stuff);
    m_StuffList.push_back(stuff);
  }

  for (const auto &bodyPart : ALL_EQUIP_ON_BODY) {
    ui->body_comboBox->addItem(bodyPart);
  }
}

EditStuff StuffsView::Save() {
  // prepare result
  EditStuff editStuff;
  editStuff.m_Name = ui->name_textEdit->toPlainText();
  editStuff.m_BodyPart = ui->body_comboBox->currentText();
  editStuff.m_Stuff.m_Name = ui->name_textEdit->toPlainText();
  editStuff.m_Stuff.m_UniqueName = ui->name_textEdit->toPlainText() +"-"+ Utils::getCurrentTimeAsString();
  // no name entered => nothing to save
  if (editStuff.m_Name.isEmpty()) {
    return EditStuff();
  }

  for (const auto &panel : m_StuffList) {
    auto &stat = editStuff.m_Stuff.m_Stats.m_AllStatsTable[panel->m_Name];
    if (panel->m_IsPercent) {
      stat.m_BufEquipPercent = panel->m_BufValue;
      stat.m_BufEquipValue = 0;
    } else {
      stat.m_BufEquipValue = panel->m_BufValue;
      stat.m_BufEquipPercent = 0;
    }
  }
  m_EditStuffList.clear();
  m_EditStuffList.push_back(editStuff);

  EditStuff::SaveStuffInJson(editStuff, ui->body_comboBox->currentText());

  // clean edit stuff view after one save
  ui->name_textEdit->setText("");
  ui->body_comboBox->setCurrentText("");

  return editStuff;
}
