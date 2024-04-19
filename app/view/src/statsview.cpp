#include "statsview.h"
#include "ui_statsview.h"

#include "Application.h"

#include "gamedisplay.h"

StatsView::StatsView(QWidget *parent) : QWidget(parent), ui(new Ui::StatsView) {
  ui->setupUi(this);

  connect((GameDisplay *)parentWidget(), &GameDisplay::selectCharacter, this,
          &StatsView::UpdateStats);
  connect((GameDisplay *)parentWidget(),
          &GameDisplay::SigUpdStatsOnSelCharacter, this,
          &StatsView::UpdateDisplayedCharStats);
  connect((GameDisplay *)parentWidget(), &GameDisplay::SigGameDisplayStart,
          this, &StatsView::UpdateStats);
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

  for (const auto &stat : ALL_STATS) {
    if (stat.isEmpty()) {
      continue;
    }
    addStatRow(model, stat,
               selectedHero->m_Stats.m_AllStatsTable.at(stat).m_CurrentValue);
  }

  return model;
}

void StatsView::UpdateStats() {
  ui->stats_table->setModel(createStatsModel(parentWidget()));
}

void StatsView::UpdateDisplayedCharStats() { UpdateStats(); }
