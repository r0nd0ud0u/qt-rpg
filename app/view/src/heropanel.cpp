#include "heropanel.h"
#include "ui_heropanel.h"

#include "ApplicationView.h"

HeroPanel::HeroPanel(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::HeroPanel)
{
  ui->setupUi(this);
}

HeroPanel::~HeroPanel()
{
  delete ui;
}

void
HeroPanel::on_pushButton_clicked()
{
  emit addStuff();
}

void
HeroPanel::UpdatePanel(Character* hero)
{
  if (hero == nullptr) {
    return;
  }

  m_Heroe = hero;

  ui->hero_name->setText(hero->m_Name);
  ui->hp_Bar->setFormat(QString::number(hero->m_Stats.m_HP) + "/" +
                        QString::number(hero->m_Stats.m_HP) + " %p%");
  ui->mana_bar->setFormat(QString::number(hero->m_Stats.m_Mana) + "/" +
                          QString::number(hero->m_Stats.m_Mana) + " %p%");
  ui->bersecker_bar->setFormat(
    QString::number(hero->m_Stats.m_Bersecker) + "/" +
    QString::number(hero->m_Stats.m_Bersecker) + " %p%");
  ui->vigor_bar->setFormat(QString::number(hero->m_Stats.m_Vigor) + "/" +
                           QString::number(hero->m_Stats.m_Vigor) + " %p%");

  ui->hp_Bar->setStyleSheet("QProgressBar{color: white;} QProgressBar::chunk { "
                            "background-color: green; border: white;}");
  ui->mana_bar->setStyleSheet("QProgressBar{color: white;} QProgressBar::chunk "
                              "{ background-color: blue; border: white;}");
  ui->bersecker_bar->setStyleSheet(
    "QProgressBar{color: black;} QProgressBar::chunk "
    "{ background-color: red; border: white;}");
  ui->vigor_bar->setStyleSheet(
    "QProgressBar{color: black;} QProgressBar::chunk "
    "{ background-color: yellow; border: white;}");
}

void
HeroPanel::SetActive(bool activated)
{
  if (activated) {
    setStyleSheet("#left_widget, #right_widget{ background:     #40b1fe;  } "
                  "#left_widget QLabel{color: white;}");
  } else {

    setStyleSheet("#left_widget, #right_widget{ background:     grey;  } "
                  "#left_widget QLabel{color: white;}");
  }
}


void HeroPanel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit selectCharacter(m_Heroe->m_Name);
    }
}

void HeroPanel::on_edit_button_clicked() const
{
    auto& appView = ApplicationView::GetInstance();
    appView.GetCharacterWindow()->InitWindow();
    appView.ShowWindow(appView.GetCharacterWindow(), true);
}

