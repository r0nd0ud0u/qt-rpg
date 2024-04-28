#include "equiponbodyview.h"
#include "ui_equiponbodyview.h"

#include "common.h"

EquipOnBodyView::EquipOnBodyView(QWidget *parent)
    : QWidget(parent), ui(new Ui::EquipOnBodyView) {
  ui->setupUi(this);
  InitEditEquipment();
}

EquipOnBodyView::~EquipOnBodyView() {
  for (auto *it : m_PanelList) {
    delete it;
    it = nullptr;
  }
  m_PanelList.clear();
  delete ui;
}

void EquipOnBodyView::InitEditEquipment() {

  for (const auto &equip : ALL_EQUIP_ON_BODY) {
    if (equip.isEmpty()) {
      continue;
    }
    auto *panel = new EquipOnBodyPanel();
    panel->m_BodyName = equip;
    panel->InitComboBox(equip);

    ui->scrollAreaWidgetContents->layout()->addWidget(panel);
    m_PanelList.push_back(panel);
  }
}

// Add new equipment in 'use stuff' tab for the selected character
void EquipOnBodyView::AddItemInComboBox(const EditStuff &es) const {
  for (const auto &panel : m_PanelList) {
    if (panel->m_BodyName == es.m_BodyPart) {
      const auto name = (es.m_Stuff.m_UniqueName.isEmpty())
                            ? es.m_Name
                            : es.m_Stuff.m_UniqueName;
      panel->AddItemInComboBox(name);
      break;
    }
  }
}

// Init the view with the selected hero or boss
void EquipOnBodyView::InitView(Character *player) const {
  if (player == nullptr) {
    return;
  }
  for (const auto &panel : m_PanelList) {
    if (player->m_WearingEquipment.count(panel->m_BodyName) > 0) {
      panel->SetValue(player->m_WearingEquipment.at(panel->m_BodyName).m_UniqueName);
    }
  }
}

std::unordered_map<QString, QString>
EquipOnBodyView::GetCurrentEquipmentTable() const {
  std::unordered_map<QString, QString> output;
  for (const auto &panel : m_PanelList) {
    if (!panel->m_Equipment.isEmpty()) {
      output[panel->m_BodyName] = panel->m_Equipment;
    }
  }
  return output;
}
