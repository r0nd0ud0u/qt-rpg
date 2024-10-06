#include "statsview.h"
#include "ui_statsview.h"

#include "Application.h"

StatsView::StatsView(QWidget *parent) : QWidget(parent), ui(new Ui::StatsView) {
  ui->setupUi(this);
}

StatsView::~StatsView() { delete ui; }

void StatsView::addStatRow(QAbstractItemModel *model, const QString &statsName,
                           const QVariant &value) const {
  const int index = model->rowCount();
  model->insertRow(index);
  model->setData(model->index(index, 0), statsName);
  model->setData(model->index(index, 1), value);
}

QAbstractItemModel *StatsView::createStatsModel(QObject *parent,
                                                const Character *c) {
  auto *model = new QStandardItemModel(0, 2, parent);

  model->setHeaderData(0, Qt::Horizontal, QObject::tr("Stats"));
  model->setHeaderData(1, Qt::Horizontal, QObject::tr("Values"));

  if (c == nullptr) {
    return nullptr;
  }

  // define curplayername
  m_CurPlayerName = c->m_Name;

  const auto &statsTable = c->m_Stats.m_AllStatsTable;
  for (const auto &stat : ALL_STATS) {
    if (stat.isEmpty()) {
      continue;
    }
    addStatRow(model, stat, statsTable.at(stat).m_CurrentValue);
  }

  return model;
}

void StatsView::UpdateDisplayedCharStats(const Character *c) {
  ui->stats_table->setModel(createStatsModel(parentWidget(), c));
}
