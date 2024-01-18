#include "statsview.h"
#include "ui_statsview.h"

#include "Application.h"

StatsView::StatsView(QWidget *parent) : QWidget(parent), ui(new Ui::StatsView) {
  ui->setupUi(this);

  ui->stats_table->setModel(createStatsModel(parent));
}

StatsView::~StatsView() { delete ui; }

void StatsView::addStatRow(QAbstractItemModel *model, const QString &subject,
                           const int sender)const {
  model->insertRow(0);
  model->setData(model->index(0, 0), subject);
  model->setData(model->index(0, 1), sender);
}

QAbstractItemModel *StatsView::createStatsModel(QObject *parent) {
  auto *model = new QStandardItemModel(0, 2, parent);

  model->setHeaderData(0, Qt::Horizontal, QObject::tr("Stats"));
  model->setHeaderData(1, Qt::Horizontal, QObject::tr("Values"));

  const auto &app = Application::GetInstance();
  if (app.m_GameManager != nullptr &&
      app.m_GameManager->m_PlayersManager != nullptr &&
      !app.m_GameManager->m_PlayersManager->m_HeroesList.empty()) {

    const auto &hero =
        app.m_GameManager->m_PlayersManager->m_HeroesList.front();
    addStatRow(model, "ArmPhy", hero->m_Stats.m_ArmPhy);
    addStatRow(model, "ArmMag", hero->m_Stats.m_ArmMag);
    addStatRow(model, "PowPhy", hero->m_Stats.m_powPhy);
    addStatRow(model, "PowMag", hero->m_Stats.m_powMag);
    addStatRow(model, "Aggro", hero->m_Stats.m_aggro);
    addStatRow(model, "Speed", hero->m_Stats.m_speed);
    addStatRow(model, "Crit", hero->m_Stats.m_CriticalStrike);
    addStatRow(model, "Dodge", hero->m_Stats.m_dogde);
    addStatRow(model, "regenHP", hero->m_Stats.m_regenHP);
    addStatRow(model, "regenMana", hero->m_Stats.m_regenMana);
    addStatRow(model, "regenVigor", hero->m_Stats.m_regenVigueur);
  }

  return model;
}
