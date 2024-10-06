#include "equipmentview.h"
#include "ui_equipmentview.h"

EquipmentView::EquipmentView(QWidget *parent)
    : QWidget(parent), ui(new Ui::EquipmentView) {
  ui->setupUi(this);
  ui->equipment_table->setModel(createEquipmentModel(parent, nullptr));
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
                                                        Character *c) {
  auto *model = new QStandardItemModel(0, 2, parent);

  model->setHeaderData(0, Qt::Horizontal, QObject::tr("Partie du corps"));
  model->setHeaderData(1, Qt::Horizontal, QObject::tr("Equipement"));

  if (c == nullptr) {
    return nullptr;
  }

  for (const auto &e : ALL_EQUIP_ON_BODY) {
    if (e.isEmpty()) {
      continue;
    }
    addEquipmentRow(model, e, c->m_WearingEquipment.at(e).m_Name);
  }

  return model;
}

void EquipmentView::UpdateEquipment(Character *c) {
  ui->equipment_table->setModel(createEquipmentModel(parentWidget(), c));
}
