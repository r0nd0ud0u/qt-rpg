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

  const auto* selectedHero = Application::GetInstance().m_GameManager->GetSelectedHero();
  addEquipmentRow(model, EQUIP_HEAD, selectedHero->m_WearingEquipment.at(EQUIP_HEAD));
  addEquipmentRow(model, EQUIP_NECKLACE,
                  selectedHero->m_WearingEquipment.at(EQUIP_NECKLACE));
  addEquipmentRow(model, EQUIP_LEFT_ARM, selectedHero->m_WearingEquipment.at(EQUIP_LEFT_ARM));
  addEquipmentRow(model, EQUIP_RIGHT_ARM,
                  selectedHero->m_WearingEquipment.at(EQUIP_RIGHT_ARM));
  addEquipmentRow(model, EQUIP_LEFT_WEAPON,
                  selectedHero->m_WearingEquipment.at(EQUIP_LEFT_WEAPON));
  addEquipmentRow(model, EQUIP_RIGHT_WEAPON,
                  selectedHero->m_WearingEquipment.at(EQUIP_RIGHT_WEAPON));
  addEquipmentRow(model, EQUIP_CHEST, selectedHero->m_WearingEquipment.at(EQUIP_CHEST));
  addEquipmentRow(model, EQUIP_RING, selectedHero->m_WearingEquipment.at(EQUIP_RING));
  addEquipmentRow(model, EQUIP_PANTS, selectedHero->m_WearingEquipment.at(EQUIP_PANTS));
  addEquipmentRow(model, EQUIP_SHOES, selectedHero->m_WearingEquipment.at(EQUIP_SHOES));

  return model;
}

void EquipmentView::UpdateEquipment(QString name) {
  ui->equipment_table->setModel(createEquipmentModel(parentWidget(), name));
}
