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
  addEquipmentRow(model, Stuff::GetStringBody(Body::head),
                  hero->m_Stuffs[Body::head].m_Name);
  addEquipmentRow(model, Stuff::GetStringBody(Body::necklace),
                  hero->m_Stuffs[Body::necklace].m_Name);
  addEquipmentRow(model, Stuff::GetStringBody(Body::arm_left),
                  hero->m_Stuffs[Body::arm_left].m_Name);
  addEquipmentRow(model, Stuff::GetStringBody(Body::arm_right),
                  hero->m_Stuffs[Body::arm_right].m_Name);
  addEquipmentRow(model, Stuff::GetStringBody(Body::weapon_left),
                  hero->m_Stuffs[Body::weapon_left].m_Name);
  addEquipmentRow(model, Stuff::GetStringBody(Body::weapon_right),
                  hero->m_Stuffs[Body::weapon_right].m_Name);
  addEquipmentRow(model, Stuff::GetStringBody(Body::chest),
                  hero->m_Stuffs[Body::chest].m_Name);
  addEquipmentRow(model, Stuff::GetStringBody(Body::ring),
                  hero->m_Stuffs[Body::ring].m_Name);
  addEquipmentRow(model, Stuff::GetStringBody(Body::pants),
                  hero->m_Stuffs[Body::pants].m_Name);
  addEquipmentRow(model, Stuff::GetStringBody(Body::shoes),
                  hero->m_Stuffs[Body::shoes].m_Name);

  return model;
}

void EquipmentView::UpdateEquipment(QString name) {
  ui->equipment_table->setModel(createEquipmentModel(parentWidget(), name));
}
