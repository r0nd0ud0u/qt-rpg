#include "statsview.h"
#include "ui_statsview.h"

#include "Application.h"

StatsView::StatsView(QWidget *parent) : QWidget(parent), ui(new Ui::StatsView) {
  ui->setupUi(this);

  ui->stats_table->setModel(createMailModel(parent));
}

StatsView::~StatsView() { delete ui; }

void StatsView::addMail(QAbstractItemModel *model, const QString &subject,
                        const int sender) {
  model->insertRow(0);
  model->setData(model->index(0, 0), subject);
  model->setData(model->index(0, 1), sender);
}

QAbstractItemModel *StatsView::createMailModel(QObject *parent) {
  QStandardItemModel *model = new QStandardItemModel(0, 2, parent);

  model->setHeaderData(0, Qt::Horizontal, QObject::tr("Stats"));
  model->setHeaderData(1, Qt::Horizontal, QObject::tr("Values"));

  const auto &app = Application::GetInstance();
  if (app.m_GameManager != nullptr &&
      app.m_GameManager->m_PlayersManager != nullptr &&
      !app.m_GameManager->m_PlayersManager->m_HeroesList.empty()) {

    const auto &hero =
        app.m_GameManager->m_PlayersManager->m_HeroesList.front();
    addMail(model, "ArmPhy", hero->m_Stats.m_ArmPhy);
    addMail(model, "ArmMag", hero->m_Stats.m_ArmMag);
    addMail(model, "PowPhy", hero->m_Stats.m_powPhy);
    addMail(model, "PowMag", hero->m_Stats.m_powMag);
    addMail(model, "Aggro", hero->m_Stats.m_aggro);
    addMail(model, "Speed", hero->m_Stats.m_speed);
    addMail(model, "Crit", hero->m_Stats.m_CriticalStrike);
    addMail(model, "Dodge", hero->m_Stats.m_dogde);
    addMail(model, "regenHP", hero->m_Stats.m_regenHP);
    addMail(model, "regenMana", hero->m_Stats.m_regenMana);
    addMail(model, "regenVigor", hero->m_Stats.m_regenVigueur);
  }

  return model;
}
