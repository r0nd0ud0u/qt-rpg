#include "bosspanel.h"
#include "ui_bosspanel.h"

#include "character.h"

BossPanel::BossPanel(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::BossPanel)
{
  ui->setupUi(this);
}

BossPanel::~BossPanel()
{
  delete ui;
}

void
BossPanel::UpdatePanel(Character* boss)
{
  if (boss == nullptr) {
    return;
  }

  m_Boss = boss;

  const auto &hp =
      std::get<StatsType<int>>(boss->m_Stats.m_AllStatsTable[STATS_HP]);

  ui->name_label->setText(boss->m_Name);
  ui->hp_bar->setFormat(QString::number(hp.m_CurrentValue) + "/" +
                        QString::number(hp.m_MaxValue) + " %p%");

  ui->hp_bar->setStyleSheet(
      "QProgressBar{text-align: center; border-style: solid; padding: 1px; "
      "padding-left:1px; padding-right: 1px; border-color: grey; border: "
      "1px solid grey; color: white; border-radius: 5px; "
      "background-color: #00ad56; } QProgressBar::chunk {"
      "background-color: green;"
      "margin: 0px;"
      "width: 10px;"
      "border-bottom-right-radius: 10px;"
      "border-bottom-left-radius: 10px;}");
}

void
BossPanel::SetActive(bool activated)
{
  if (activated) {
    setStyleSheet("#verticalWidget { background:     #40b1fe;  } "
                  "#verticalWidget QLabel{color: white;}");
  } else {

    setStyleSheet("#verticalWidget { background:     grey;  } "
                  "#verticalWidget QLabel{color: white;}");
  }
}
