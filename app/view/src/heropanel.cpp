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
  auto &hp =
      std::get<StatsType<int>>(m_Heroe->m_Stats.m_AllStatsTable[STATS_HP]);
  auto &mana =
      std::get<StatsType<int>>(m_Heroe->m_Stats.m_AllStatsTable[STATS_MANA]);
  auto &vigor =
      std::get<StatsType<int>>(m_Heroe->m_Stats.m_AllStatsTable[STATS_VIGOR]);
  auto &berseck =
      std::get<StatsType<int>>(m_Heroe->m_Stats.m_AllStatsTable[STATS_BERSECK]);

  ui->hero_name->setText(hero->m_Name);
  ui->hp_Bar->setFormat(QString::number(hp.m_CurrentValue) + "/" +
                        QString::number(hp.m_MaxValue) + " %p%");
  ui->mana_bar->setFormat(QString::number(mana.m_CurrentValue) + "/" +
                          QString::number(mana.m_MaxValue) + " %p%");
  ui->bersecker_bar->setFormat(
      QString::number(berseck.m_CurrentValue) + "/" +
      QString::number(berseck.m_MaxValue) + " %p%");
  ui->vigor_bar->setFormat(QString::number(vigor.m_CurrentValue) + "/" +
                           QString::number(vigor.m_MaxValue) + " %p%");

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
HeroPanel::SetActive(const bool activated)
{
  if (activated) {
    setStyleSheet("#active_widget{ background:     #40b1fe;  } ");
  } else {

    setStyleSheet("#active_widget{ background:     grey;  } ");
  }
}

void HeroPanel::SetSelected(const bool selected) {
    QFrame::Shape shape = QFrame::NoFrame;
    int lineWidth = 0;
    if(selected) {
        shape = QFrame::Box;
        lineWidth = 3;
        ui->frame->setLineWidth(lineWidth);
    }
    ui->frame->setFrameShape(shape);


    // update buttons
    ui->edit_button->setEnabled(selected);
    ui->talent_tree_button->setEnabled(selected);
    ui->stuff_button->setEnabled(selected);
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
