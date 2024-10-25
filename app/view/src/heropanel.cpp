#include "heropanel.h"
#include "ui_heropanel.h"

#include "ApplicationView.h"
#include "utils.h"

HeroPanel::HeroPanel(QWidget *parent) : QWidget(parent), ui(new Ui::HeroPanel) {
  ui->setupUi(this);
}

HeroPanel::~HeroPanel() { delete ui; }

void HeroPanel::UpdatePanel(Character *hero,
                            const std::vector<GameAtkEffects> &table) {
  if (hero == nullptr) {
    return;
  }

  SetHero(hero);

  // update level exp
  ui->hero_level->setText(
      QString("Lv: %1 Exp: %2").arg(hero->m_Level).arg(hero->m_Exp));

  const auto &hp = m_Heroe->m_Stats.m_AllStatsTable[STATS_HP];
  const auto &mana = m_Heroe->m_Stats.m_AllStatsTable[STATS_MANA];
  const auto &vigor = m_Heroe->m_Stats.m_AllStatsTable[STATS_VIGOR];
  const auto &berseck = m_Heroe->m_Stats.m_AllStatsTable[STATS_BERSECK];

  ui->hero_name->setText(hero->m_Name);

  // hp
  ui->hp_Bar->setFormat(QString::number(hp.m_CurrentValue) + "/" +
                        QString::number(hp.m_MaxValue) + " %p%");
  int hpValue =
      (hp.m_MaxValue > 0) ? 100 * hp.m_CurrentValue / hp.m_MaxValue : 0;
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

  // for all hot/dot and buf/debuf updates
  int nbBufs = 0;
  int nbDebufs = 0;
  // hot, dot, buf, debuf by ongoing effects
  const auto effects = Utils::GetActiveEffects(table);
  if (effects.has_value()) {
    ui->verticalWidget->SetHotDotValues(effects->hot.nb, effects->dot.nb);
    nbBufs += effects->buf.nb;
    nbDebufs += effects->debuf.nb;
  } else {
    ui->verticalWidget->SetHotDotValues(0, 0);
  }

  // buf/debuf nbs by ongoing buf/debuf table
  const auto bufDebuf = hero->GetBufDebuf();
  if (bufDebuf.has_value()) {
    nbBufs += bufDebuf->buf.nb;
    nbDebufs += bufDebuf->debuf.nb;
  }
  ui->verticalWidget->SetBufDebufValues(nbBufs, nbDebufs);

  // update tables of dotview
  ui->verticalWidget->UpdateData(effects, bufDebuf);
}

void HeroPanel::SetActive(const bool activated) {
  ui->form_comboBox->setEnabled(activated);
  ui->active_widget->setEnabled(activated);
  if (activated) {
    setStyleSheet("#active_widget{ background:     #40b1fe;  } ");
  } else {

    setStyleSheet("#active_widget{ background:     grey;  } ");
  }
}

bool HeroPanel::GetActive() const { return ui->active_widget->isEnabled(); }

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
}

void HeroPanel::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    emit SigPanelSelectCharacter(m_Heroe);
  }
}

void HeroPanel::on_edit_button_clicked() const {
  auto &appView = ApplicationView::GetInstance();
  appView.GetCharacterWindow()->InitWindow(tabType::character, m_Heroe);
  appView.ShowWindow(appView.GetCharacterWindow(), true);
}

void HeroPanel::SetPixmap(const QString &name) {
  // Update image character
  // Resize the photo
  ui->img_character->SetPixmap(name, 100);
}

void HeroPanel::InitComboBox() {
  if (m_Heroe == nullptr) {
    return;
  }
  for (const auto &form : m_Heroe->m_Forms) {
    ui->form_comboBox->addItem(form);
  }
}

void HeroPanel::on_form_comboBox_currentTextChanged(const QString &arg1) {
  emit SigUpdateCharaForm(m_Heroe->m_Name, arg1);
  m_Heroe->m_SelectedForm = arg1;
}

void HeroPanel::on_pushButton_clicked() {
  UpdateActiveRightWidget();
  emit SigUpdateCharacterPlaying(m_Heroe->m_Name);
}

void HeroPanel::UpdateActiveRightWidget() {
  if (!m_Heroe->m_StatsInGame.m_IsPlaying) {
    SetPlayingStatus();
  } else {
    SetSelectStatus();
  }
}

void HeroPanel::SetPlayingStatus() {
  ui->pushButton->setText("Playing");
  setStyleSheet("#right_widget{ background:     #40b1fe;  } ");
}

void HeroPanel::SetSelectStatus() {
  ui->pushButton->setText("Select");
  setStyleSheet("#right_widget{ background:     grey;  } ");
}

void HeroPanel::SetSelectedGameChoiceBtn(const bool value) {
  ui->pushButton->setEnabled(value);
}

void HeroPanel::on_removePushButton_clicked() {
  emit SigRemovePanelByBtn(this);
}

void HeroPanel::SetHero(Character *c) { m_Heroe = c; }

void HeroPanel::ProcessPlayingMode() {
  ui->removePushButton->hide();
  ui->pushButton->hide();
  ui->edit_button->hide();
}
