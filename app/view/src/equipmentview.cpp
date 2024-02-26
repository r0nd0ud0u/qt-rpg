#include "equipmentview.h"
#include "ui_equipmentview.h"

#include "Application.h"
#include "gamedisplay.h"

EquipmentView::EquipmentView(QWidget *parent)
    : QWidget(parent), ui(new Ui::EquipmentView) {
  ui->setupUi(this);
  ui->equipment_table->setModel(createEquipmentModel(parent));

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

QAbstractItemModel *EquipmentView::createEquipmentModel(QObject *parent) {
  auto *model = new QStandardItemModel(0, 2, parent);

  model->setHeaderData(0, Qt::Horizontal, QObject::tr("Partie du corps"));
  model->setHeaderData(1, Qt::Horizontal, QObject::tr("Equipement"));

  const auto *selectedHero =
      Application::GetInstance().m_GameManager->GetSelectedHero();

  for(const auto& e : ALL_EQUIP){
      addEquipmentRow(model, e,
                      selectedHero->m_WearingEquipment.at(e).m_Name);
  }

  return model;
}

void EquipmentView::UpdateEquipment(QString name) {
  ui->equipment_table->setModel(createEquipmentModel(parentWidget()));
}
