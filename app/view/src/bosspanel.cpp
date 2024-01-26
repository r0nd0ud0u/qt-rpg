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

  ui->name_label->setText(boss->m_Name);
  ui->hp_bar->setFormat(QString::number(boss->m_CurrentStats.m_HP.m_Value) + "/" +
                        QString::number(boss->m_Stats.m_HP.m_Value) + " %p%");

  ui->hp_bar->setStyleSheet("QProgressBar{color: white;} QProgressBar::chunk { "
                            "background-color: green; border: white;}");
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
