#include "alleffectpanel.h"
#include "ui_alleffectpanel.h"

#include "gamedisplay.h"

AllEffectPanel::AllEffectPanel(QWidget *parent)
    : QWidget(parent), ui(new Ui::AllEffectPanel) {
  ui->setupUi(this);
  ui->tableView->setModel(createModel(parent));

  connect((GameDisplay *)parentWidget(), &GameDisplay::SigNewEffectLaunched, this,
          &AllEffectPanel::UpdateModel);
  connect((GameDisplay *)parentWidget(), &GameDisplay::SigUpdateAllEffectPanel, this,
          &AllEffectPanel::ResetModelWithAllEffect);
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
  model->setData(model->index(index, 4), ep->value);
}

QAbstractItemModel *AllEffectPanel::createModel(QObject *parent) {
  auto *model = new QStandardItemModel(0, 5, parent);

  model->setHeaderData(0, Qt::Horizontal, QObject::tr("Effet"));
  model->setHeaderData(1, Qt::Horizontal, QObject::tr("Cible"));
  model->setHeaderData(2, Qt::Horizontal, QObject::tr("Lanceur"));
  model->setHeaderData(3, Qt::Horizontal, QObject::tr("Tours restant"));
  model->setHeaderData(4, Qt::Horizontal, QObject::tr("Valeur"));

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

void AllEffectPanel::ResetModelWithAllEffect(const std::unordered_map<QString, std::vector<GameAtkEffects>>& table) {
    if(table.empty()){
        return;
    }
    ui->tableView->setModel(createModel(ui->tableView->parent()));
    for(const auto& [pl, allGae] : table){
        for(const auto& gae : allGae){
            addRow(ui->tableView->model(), &gae.allAtkEffects, gae.launcher,pl);
        }
    }

}
