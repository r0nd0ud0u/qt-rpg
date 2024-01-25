#include "equipmentview.h"
#include "ui_equipmentview.h"

#include "Application.h"
#include "gamedisplay.h"
#include "stuff.h"

EquipmentView::EquipmentView(QWidget *parent)
    : QWidget(parent), ui(new Ui::EquipmentView) {
  ui->setupUi(this);
  ui->equipment_table->setModel(createEquipmentModel(parent, ""));

  connect((GameDisplay *)parentWidget(), &GameDisplay::selectCharacter, this,
          &EquipmentView::UpdateEquipment);
}

EquipmentView::~EquipmentView() { delete ui; }

void EquipmentView::addEquipmentRow(QAbstractItemModel *model,
                                    const QString &body,
                                    const QString equipmentName) const {
  int index = model->rowCount();
  model->insertRow(index);
  model->setData(model->index(index, 0), body);
  model->setData(model->index(index, 1), equipmentName);
}

QAbstractItemModel *EquipmentView::createEquipmentModel(QObject *parent,
                                                        QString playerName) {
  auto *model = new QStandardItemModel(0, 2, parent);

  model->setHeaderData(0, Qt::Horizontal, QObject::tr("Partie du corps"));
  model->setHeaderData(1, Qt::Horizontal, QObject::tr("Equipement"));

  const auto &app = Application::GetInstance();
  if (app.m_GameManager == nullptr ||
      app.m_GameManager->m_PlayersManager == nullptr) {
    return nullptr;
  }

  const auto &heroList = app.m_GameManager->m_PlayersManager->m_HeroesList;
  // Case hero list empty
  if (heroList.empty()) {
    return model;
  }
  int indexPlayer = 0;
  // lambda function to add here
  for (int i = 0; i < heroList.size(); i++) {
    if (heroList[i]->m_Name == playerName) {
      indexPlayer = i;
      break;
    }
  }

  const auto &hero = heroList[indexPlayer];
  addEquipmentRow(model, EQUIP_HEAD, hero->m_WearingEquipment[EQUIP_HEAD]);
  addEquipmentRow(model, EQUIP_NECKLACE,
                  hero->m_WearingEquipment[EQUIP_NECKLACE]);
  addEquipmentRow(model, EQUIP_LEFT_ARM,
                  hero->m_WearingEquipment[EQUIP_LEFT_ARM]);
  addEquipmentRow(model, EQUIP_RIGHT_ARM,
                  hero->m_WearingEquipment[EQUIP_RIGHT_ARM]);
  addEquipmentRow(model, EQUIP_LEFT_WEAPON,
                  hero->m_WearingEquipment[EQUIP_LEFT_WEAPON]);
  addEquipmentRow(model, EQUIP_RIGHT_WEAPON,
                  hero->m_WearingEquipment[EQUIP_RIGHT_WEAPON]);
  addEquipmentRow(model, EQUIP_CHEST, hero->m_WearingEquipment[EQUIP_CHEST]);
  addEquipmentRow(model, EQUIP_RING, hero->m_WearingEquipment[EQUIP_RING]);
  addEquipmentRow(model, EQUIP_PANTS, hero->m_WearingEquipment[EQUIP_PANTS]);
  addEquipmentRow(model, EQUIP_SHOES, hero->m_WearingEquipment[EQUIP_SHOES]);

  return model;
}

void EquipmentView::UpdateEquipment(QString name) {
  ui->equipment_table->setModel(createEquipmentModel(parentWidget(), name));
}
