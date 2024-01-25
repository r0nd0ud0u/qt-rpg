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

void StatsView::addStatRow(QAbstractItemModel *model, const QString &statsName,
                           const QVariant &value) const {
  int index = model->rowCount();
  model->insertRow(index);
  model->setData(model->index(index, 0), statsName);
  model->setData(model->index(index, 1), value);
}

QAbstractItemModel *StatsView::createStatsModel(QObject *parent,
                                                QString playerName) {
  auto *model = new QStandardItemModel(0, 2, parent);

  model->setHeaderData(0, Qt::Horizontal, QObject::tr("Stats"));
  model->setHeaderData(1, Qt::Horizontal, QObject::tr("Values"));

  const auto *selectedHero =
      Application::GetInstance().m_GameManager->GetSelectedHero();
  addStatRow(model, "ArmPhy", selectedHero->m_Stats.m_ArmPhy.m_Value);
  addStatRow(model, "ArmMag", selectedHero->m_Stats.m_ArmMag.m_Value);
  addStatRow(model, "PowPhy", selectedHero->m_Stats.m_powPhy.m_Value);
  addStatRow(model, "PowMag", selectedHero->m_Stats.m_powMag.m_Value);
  addStatRow(model, "Aggro", selectedHero->m_Stats.m_aggro.m_Value);
  addStatRow(model, "Speed", selectedHero->m_Stats.m_speed.m_Value);
  addStatRow(model, "Crit", selectedHero->m_Stats.m_CriticalStrike.m_Value);
  addStatRow(model, "Dodge", selectedHero->m_Stats.m_dogde.m_Value);
  addStatRow(model, "regenHP", selectedHero->m_Stats.m_regenHP.m_Value);
  addStatRow(model, "regenMana", selectedHero->m_Stats.m_regenMana.m_Value);
  addStatRow(model, "regenVigor", selectedHero->m_Stats.m_regenVigueur.m_Value);

  return model;
}

void StatsView::UpdateStats(QString name) {
  ui->stats_table->setModel(createStatsModel(parentWidget(), name));
}
