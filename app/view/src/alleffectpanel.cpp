#include "alleffectpanel.h"
#include "ui_alleffectpanel.h"

#include "gamedisplay.h"

AllEffectPanel::AllEffectPanel(QWidget *parent)
    : QWidget(parent), ui(new Ui::AllEffectPanel) {
  ui->setupUi(this);
  ui->tableView->setModel(createModel(parent));

  connect((GameDisplay *)parentWidget(), &GameDisplay::SigUpdateAllEffectPanel,
          this, &AllEffectPanel::ResetModelWithAllEffect);
}

AllEffectPanel::~AllEffectPanel() { delete ui; }

void AllEffectPanel::addRow(QAbstractItemModel *model, const effectParam *ep,
                            const QString &launcher, const QString &targetName,
                            const QString &atkName) const {
  if (ep == nullptr || model == nullptr) {
    return;
  }
  int index = model->rowCount();
  model->insertRow(index);
  QString effect;
  if(!ep->effect.isEmpty() && !ep->statsName.isEmpty()){
      effect = ep->statsName + "-" + ep->effect;
  } else if(ep->effect.isEmpty()){
      effect = ep->statsName;
  } else if(ep->statsName.isEmpty()){
      effect = ep->effect;
  }
  model->setData(model->index(index, static_cast<int>(columnsPanel::effect)),
                 effect);
  model->setData(model->index(index, static_cast<int>(columnsPanel::atk)),
                 atkName);
  model->setData(
      model->index(index, static_cast<int>(columnsPanel::targetName)),
      targetName);
  model->setData(
      model->index(index, static_cast<int>(columnsPanel::launcherName)),
      launcher);
  model->setData(
      model->index(index, static_cast<int>(columnsPanel::remainingTurns)),
      ep->nbTurns - ep->counterTurn);
  model->setData(model->index(index, static_cast<int>(columnsPanel::value)),
                 ep->value);
}

QAbstractItemModel *AllEffectPanel::createModel(QObject *parent) const {
  auto *model = new QStandardItemModel(
      0, static_cast<int>(columnsPanel::enumSize), parent);

  model->setHeaderData(static_cast<int>(columnsPanel::effect), Qt::Horizontal,
                       QObject::tr("Effet"));
  model->setHeaderData(static_cast<int>(columnsPanel::atk), Qt::Horizontal,
                       QObject::tr("Attaque"));
  model->setHeaderData(static_cast<int>(columnsPanel::targetName),
                       Qt::Horizontal, QObject::tr("Cible"));
  model->setHeaderData(static_cast<int>(columnsPanel::launcherName),
                       Qt::Horizontal, QObject::tr("Lanceur"));
  model->setHeaderData(static_cast<int>(columnsPanel::remainingTurns),
                       Qt::Horizontal, QObject::tr("Tours restant"));
  model->setHeaderData(static_cast<int>(columnsPanel::value), Qt::Horizontal,
                       QObject::tr("Valeur"));

  return model;
}

void AllEffectPanel::ResetModelWithAllEffect(
    const std::unordered_map<QString, std::vector<GameAtkEffects>> &table) {
  if (table.empty()) {
    return;
  }
  ui->tableView->setModel(createModel(ui->tableView->parent()));
  for (const auto &[pl, allGae] : table) {
    for (const auto &gae : allGae) {
          if(gae.allAtkEffects.nbTurns > 100){
            continue; // TODO hide the passive bonus set at 1000 at start of game, improve that
        }
      addRow(ui->tableView->model(), &gae.allAtkEffects, gae.launcher, pl,
             gae.atk.name);
    }
  }
}
