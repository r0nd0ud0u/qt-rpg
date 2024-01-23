#include "statsview.h"
#include "ui_statsview.h"

#include "Application.h"

#include "gamedisplay.h"

StatsView::StatsView(QWidget *parent) : QWidget(parent), ui(new Ui::StatsView) {
  ui->setupUi(this);
  ui->stats_table->setModel(createStatsModel(parent, ""));

  connect((GameDisplay *)parentWidget(), &GameDisplay::selectCharacter, this,
          &StatsView::UpdateStats);
}

StatsView::~StatsView() { delete ui; }

void StatsView::addStatRow(QAbstractItemModel *model, const QString &subject,
                           const int sender) const {
  int index = model->rowCount();
  model->insertRow(index);
  model->setData(model->index(index, 0), subject);
  model->setData(model->index(index, 1), sender);
}

QAbstractItemModel *StatsView::createStatsModel(QObject *parent,
                                                QString playerName) {
  auto *model = new QStandardItemModel(0, 2, parent);

  model->setHeaderData(0, Qt::Horizontal, QObject::tr("Stats"));
  model->setHeaderData(1, Qt::Horizontal, QObject::tr("Values"));

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
  addStatRow(model, "ArmPhy", hero->m_Stats.m_ArmPhy.m_Value);
  addStatRow(model, "ArmMag", hero->m_Stats.m_ArmMag.m_Value);
  addStatRow(model, "PowPhy", hero->m_Stats.m_powPhy.m_Value);
  addStatRow(model, "PowMag", hero->m_Stats.m_powMag.m_Value);
  addStatRow(model, "Aggro", hero->m_Stats.m_aggro.m_Value);
  addStatRow(model, "Speed", hero->m_Stats.m_speed.m_Value);
  addStatRow(model, "Crit", hero->m_Stats.m_CriticalStrike.m_Value);
  addStatRow(model, "Dodge", hero->m_Stats.m_dogde.m_Value);
  addStatRow(model, "regenHP", hero->m_Stats.m_regenHP.m_Value);
  addStatRow(model, "regenMana", hero->m_Stats.m_regenMana.m_Value);
  addStatRow(model, "regenVigor", hero->m_Stats.m_regenVigueur.m_Value);

  return model;
}

void StatsView::UpdateStats(QString name) {
  ui->stats_table->setModel(createStatsModel(parentWidget(), name));
}
