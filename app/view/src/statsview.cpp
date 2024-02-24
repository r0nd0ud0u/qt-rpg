#include "statsview.h"
#include "ui_statsview.h"

#include "Application.h"

#include "gamedisplay.h"

StatsView::StatsView(QWidget *parent) : QWidget(parent), ui(new Ui::StatsView) {
  ui->setupUi(this);
  ui->stats_table->setModel(createStatsModel(parent));

  connect((GameDisplay *)parentWidget(), &GameDisplay::selectCharacter, this,
          &StatsView::UpdateStats);
  connect((GameDisplay *)parentWidget(), &GameDisplay::SigUpdStatsOnSelCharacter,
          this, &StatsView::UpdateDisplayedCharStats);
}

StatsView::~StatsView() { delete ui; }

void StatsView::addStatRow(QAbstractItemModel *model, const QString &statsName,
                           const QVariant &value) const {
  const int index = model->rowCount();
  model->insertRow(index);
  model->setData(model->index(index, 0), statsName);
  model->setData(model->index(index, 1), value);
}

QAbstractItemModel *StatsView::createStatsModel(QObject *parent) {
  auto *model = new QStandardItemModel(0, 2, parent);

  model->setHeaderData(0, Qt::Horizontal, QObject::tr("Stats"));
  model->setHeaderData(1, Qt::Horizontal, QObject::tr("Values"));

  const auto *selectedHero =
      Application::GetInstance().m_GameManager->GetSelectedHero();
  if (selectedHero == nullptr) {
    return nullptr;
  }

  // define curplayername
  m_CurPlayerName = selectedHero->m_Name;

  addStatRow(model, "ArmPhy",
             std::get<StatsType<int>>(
                 selectedHero->m_Stats.m_AllStatsTable.at(STATS_ARM_PHY))
                 .m_CurrentValue);
  addStatRow(model, "ArmMag",
             std::get<StatsType<int>>(
                 selectedHero->m_Stats.m_AllStatsTable.at(STATS_ARM_MAG))
                 .m_CurrentValue);
  addStatRow(model, "PowPhy",
             std::get<StatsType<int>>(
                 selectedHero->m_Stats.m_AllStatsTable.at(STATS_POW_PHY))
                 .m_CurrentValue);
  addStatRow(model, "PowMag",
             std::get<StatsType<int>>(
                 selectedHero->m_Stats.m_AllStatsTable.at(STATS_POW_MAG))
                 .m_CurrentValue);
  addStatRow(model, "Aggro",
             std::get<StatsType<int>>(
                 selectedHero->m_Stats.m_AllStatsTable.at(STATS_AGGRO))
                 .m_CurrentValue);
  addStatRow(model, "Speed",
             std::get<StatsType<int>>(
                 selectedHero->m_Stats.m_AllStatsTable.at(STATS_SPEED))
                 .m_CurrentValue);
  addStatRow(model, "Crit",
             std::get<StatsType<int>>(
                 selectedHero->m_Stats.m_AllStatsTable.at(STATS_CRIT))
                 .m_CurrentValue);
  addStatRow(model, "Dodge",
             std::get<StatsType<int>>(
                 selectedHero->m_Stats.m_AllStatsTable.at(STATS_DODGE))
                 .m_CurrentValue);
  addStatRow(model, "regenHP",
             std::get<StatsType<int>>(
                 selectedHero->m_Stats.m_AllStatsTable.at(STATS_REGEN_HP))
                 .m_CurrentValue);
  addStatRow(model, "regenMana",
             std::get<StatsType<int>>(
                 selectedHero->m_Stats.m_AllStatsTable.at(STATS_REGEN_MANA))
                 .m_CurrentValue);
  addStatRow(model, "regenVigor",
             std::get<StatsType<int>>(
                 selectedHero->m_Stats.m_AllStatsTable.at(STATS_REGEN_VIGOR))
                 .m_CurrentValue);

  return model;
}

void StatsView::UpdateStats(QString name) {
  ui->stats_table->setModel(createStatsModel(parentWidget()));
}

void StatsView::UpdateDisplayedCharStats() {
  UpdateStats(m_CurPlayerName);
}
