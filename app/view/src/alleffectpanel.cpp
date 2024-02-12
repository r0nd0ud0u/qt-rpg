#include "alleffectpanel.h"
#include "ui_alleffectpanel.h"

#include "gamedisplay.h"

AllEffectPanel::AllEffectPanel(QWidget *parent)
    : QWidget(parent), ui(new Ui::AllEffectPanel) {
  ui->setupUi(this);
  ui->tableView->setModel(createModel(parent));

  connect((GameDisplay *)parentWidget(), &GameDisplay::SigNewEffectLaunched, this,
          &AllEffectPanel::UpdateModel);
}

AllEffectPanel::~AllEffectPanel() { delete ui; }

void AllEffectPanel::addRow(QAbstractItemModel *model, const effectParam* ep, const QString& launcher, const QString& targetName) const {
    if(ep == nullptr || model == nullptr){
        return;
    }
  int index = model->rowCount();
  model->insertRow(index);
  const QString effect = (ep->effect.isEmpty()) ? ep->statsName : ep->effect;
  model->setData(model->index(index, 0), effect);
  model->setData(model->index(index, 1), targetName);
  model->setData(model->index(index, 2), launcher);
  model->setData(model->index(index, 3), ep->nbTurns - ep->counterTurn);
}

QAbstractItemModel *AllEffectPanel::createModel(QObject *parent) {
  auto *model = new QStandardItemModel(0, 4, parent);

  model->setHeaderData(0, Qt::Horizontal, QObject::tr("Effet"));
  model->setHeaderData(1, Qt::Horizontal, QObject::tr("Cible"));
  model->setHeaderData(2, Qt::Horizontal, QObject::tr("Lanceur"));
  model->setHeaderData(3, Qt::Horizontal, QObject::tr("Tours restant"));

  return model;
}

void AllEffectPanel::UpdateModel(const std::vector<effectParam>& epTable, const QString& launcher, const QString& targetName) {
    if(epTable.empty()){
        return;
    }
    for(const auto& ep : epTable){
        addRow(ui->tableView->model(), &ep, launcher,targetName);
    }
}
