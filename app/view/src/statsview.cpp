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
  addStatRow(model, "ArmPhy", selectedHero->m_Stats.m_ArmPhy.m_CurrentValue);
  addStatRow(model, "ArmMag", selectedHero->m_Stats.m_ArmMag.m_CurrentValue);
  addStatRow(model, "PowPhy", selectedHero->m_Stats.m_PowPhy.m_CurrentValue);
  addStatRow(model, "PowMag", selectedHero->m_Stats.m_PowMag.m_CurrentValue);
  addStatRow(model, "Aggro", selectedHero->m_Stats.m_Aggro.m_CurrentValue);
  addStatRow(model, "Speed", selectedHero->m_Stats.m_Speed.m_CurrentValue);
  addStatRow(model, "Crit",
             selectedHero->m_Stats.m_CriticalStrike.m_CurrentValue);
  addStatRow(model, "Dodge", selectedHero->m_Stats.m_Dogde.m_CurrentValue);
  addStatRow(model, "regenHP", selectedHero->m_Stats.m_RegenHP.m_CurrentValue);
  addStatRow(model, "regenMana",
             selectedHero->m_Stats.m_RegenMana.m_CurrentValue);
  addStatRow(model, "regenVigor",
             selectedHero->m_Stats.m_RegenVigor.m_CurrentValue);

  return model;
}

void StatsView::UpdateStats(QString name) {
  ui->stats_table->setModel(createStatsModel(parentWidget(), name));
}
