#include "heropanel.h"
#include "ui_heropanel.h"

#include "ApplicationView.h"

HeroPanel::HeroPanel(QWidget *parent) : QWidget(parent), ui(new Ui::HeroPanel) {
  ui->setupUi(this);
}

HeroPanel::~HeroPanel() { delete ui; }

void HeroPanel::on_pushButton_clicked() { emit addStuff(); }

void HeroPanel::UpdatePanel(Character *hero) {
  if (hero == nullptr) {
    return;
  }

  m_Heroe = hero;
  const auto &hp =
      std::get<StatsType<int>>(m_Heroe->m_Stats.m_AllStatsTable[STATS_HP]);
  const auto &mana =
      std::get<StatsType<int>>(m_Heroe->m_Stats.m_AllStatsTable[STATS_MANA]);
  const auto &vigor =
      std::get<StatsType<int>>(m_Heroe->m_Stats.m_AllStatsTable[STATS_VIGOR]);
  const auto &berseck =
      std::get<StatsType<int>>(m_Heroe->m_Stats.m_AllStatsTable[STATS_BERSECK]);

  ui->hero_name->setText(hero->m_Name);

  // hp
  ui->hp_Bar->setFormat(QString::number(hp.m_CurrentValue) + "/" +
                        QString::number(hp.m_MaxValue) + " %p%");
  int hpValue = 100 * hp.m_CurrentValue / hp.m_MaxValue;
  ui->hp_Bar->setValue(hpValue);

  // mana
  ui->mana_bar->setFormat(QString::number(mana.m_CurrentValue) + "/" +
                          QString::number(mana.m_MaxValue) + " %p%");
  int manaValue = 0;
  if (mana.m_MaxValue > 0) {
    manaValue = 100 * mana.m_CurrentValue / mana.m_MaxValue;
  }
  ui->mana_bar->setValue(manaValue);

  // berseck
  ui->bersecker_bar->setFormat(QString::number(berseck.m_CurrentValue) + "/" +
                               QString::number(berseck.m_MaxValue) + " %p%");
  int berseckValue = 0;
  if (berseck.m_MaxValue > 0) {
    berseckValue = 100 * berseck.m_CurrentValue / berseck.m_MaxValue;
  }
  ui->bersecker_bar->setValue(berseckValue);

  // vigor
  ui->vigor_bar->setFormat(QString::number(vigor.m_CurrentValue) + "/" +
                           QString::number(vigor.m_MaxValue) + " %p%");
  int vigorValue = 0;
  if (vigor.m_MaxValue > 0) {
    vigorValue = 100 * vigor.m_CurrentValue / vigor.m_MaxValue;
  }
  ui->vigor_bar->setValue(vigorValue);

  ui->hp_Bar->setStyleSheet(
      "QProgressBar{text-align: center; border-style: solid; padding: 1px; "
      "padding-left:1px; padding-right: 1px; border-color: grey; border: "
      "1px solid grey; color: white; border-radius: 5px; "
      "background-color: #00ad56; } QProgressBar::chunk {"
      "background-color: green;"
      "margin: 0px;"
      "width: 10px;"
      "border-bottom-right-radius: 10px;"
      "border-bottom-left-radius: 10px;}");

  ui->mana_bar->setStyleSheet(
      "QProgressBar{text-align: center; border-style: solid; padding: 1px; "
      "padding-left:1px; padding-right: 1px; border-color: grey; border: "
      "1px solid grey; color: white; border-radius: 5px; "
      "background-color: #4768cc; } QProgressBar::chunk {"
      "background-color: blue;"
      "margin: 0px;"
      "width: 10px;"
      "border-bottom-right-radius: 10px;"
      "border-bottom-left-radius: 10px;}");
  ui->bersecker_bar->setStyleSheet(
      "QProgressBar{text-align: center; border-style: solid; padding: 1px; "
      "padding-left:1px; padding-right: 1px; border-color: grey; border: "
      "1px solid grey; color: white; border-radius: 5px; "
      "background-color: #ffb2d8; }QProgressBar::chunk {"
      "background-color: red;"
      "margin: 0px;"
      "width: 10px;"
      "border-bottom-right-radius: 10px;"
      "border-bottom-left-radius: 10px;}");
  ui->vigor_bar->setStyleSheet(
      "QProgressBar{text-align: center; border-style: solid; padding: 1px; "
      "padding-left:1px; padding-right: 1px; border-color: grey; border: "
      "1px solid grey; color: white; border-radius: 5px; "
      "background-color: #cc9647; }QProgressBar::chunk {"
      "background-color: orange;"
      "margin: 0px;"
      "width: 10px;"
      "border-bottom-right-radius: 10px;"
      "border-bottom-left-radius: 10px;}");
}

void HeroPanel::SetActive(const bool activated) {
  if (activated) {
    setStyleSheet("#active_widget{ background:     #40b1fe;  } ");
  } else {

    setStyleSheet("#active_widget{ background:     grey;  } ");
  }
}

void HeroPanel::SetSelected(const bool selected) {
  QFrame::Shape shape = QFrame::NoFrame;
  int lineWidth = 0;
  if (selected) {
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

void HeroPanel::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    emit selectCharacter(m_Heroe->m_Name);
  }
}

void HeroPanel::on_edit_button_clicked() const {
  auto &appView = ApplicationView::GetInstance();
  appView.GetCharacterWindow()->InitWindow(actionType::edit);
  appView.ShowWindow(appView.GetCharacterWindow(), true);
}

void HeroPanel::SetPixmap(const QString& name){
    // Update image character
    // Resize the photo
    ui->img_character->SetPixmap(name, 200);
}
