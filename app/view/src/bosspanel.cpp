#include "bosspanel.h"
#include "ui_bosspanel.h"

#include "character.h"

BossPanel::BossPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BossPanel)
{
    ui->setupUi(this);
}

BossPanel::~BossPanel()
{
    delete ui;
}

void BossPanel::UpdatePanel(Character *boss) {
    if (boss == nullptr) {
        return;
    }

    m_Boss = boss;

    ui->name_label->setText(boss->m_Name);
    ui->hp_bar->setFormat(QString::number(boss->m_Stats.m_HP) + "/" +
                          QString::number(boss->m_Stats.m_HP)+ " %p%");


    ui->hp_bar->setStyleSheet("QProgressBar{color: white;} QProgressBar::chunk { "
                              "background-color: green; border: white;}");

}

void BossPanel::SetActive(bool activated) {
    if (activated) {
        setStyleSheet("#main_widget { background:     #40b1fe;  } "
                      "#main_widget QLabel{color: white;}");
    } else

    setStyleSheet("#main_widget { background:     grey;  } "
                  "#main_widget QLabel{color: white;}");
}
