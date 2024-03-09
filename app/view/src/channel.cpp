#include "channel.h"
#include "ui_channel.h"

#include "Application.h"
#include "gamedisplay.h"

Channel::Channel(QWidget *parent) : QWidget(parent), ui(new Ui::Channel) {
  ui->setupUi(this);

  ShowTurnPage();
  connect((GameDisplay*)parentWidget(), &GameDisplay::SigBossDead, this, &Channel::EndOfGame);
  connect((GameDisplay*)parentWidget(), &GameDisplay::SigUpdateChannelView, this, &Channel::UpdateLog);
}

Channel::~Channel() { delete ui; }

void Channel::ShowPageStuffs() { ui->stackedWidget->setCurrentIndex(0); }

void Channel::ShowTurnPage() { ui->stackedWidget->setCurrentIndex(1); }

void Channel::on_next_turn_button_clicked() {
  ui->next_turn_button->setEnabled(false);
  ui->end_round_button->setEnabled(true);
  emit SigNewTurn();
}

void Channel::on_end_round_button_clicked() {
  auto &gs = Application::GetInstance().m_GameManager->m_GameState;
  if (gs->m_CurrentRound < gs->m_OrderToPlay.size()) {
      emit SigNextRound();
  } else {
    ui->end_round_button->setEnabled(false);
    ui->next_turn_button->setEnabled(true);
    emit SigEndOfTurn();
  }
}

void Channel::EndOfGame(){
    // TODO end of game
    // if no more ennemies set false end round
    //ui->end_round_button->setEnabled(false);
}

void Channel::UpdateLog(const QString& topic, const QString& log, const QColor color){
    if(log.isEmpty()){
        return;
    }
    ui->channel_textEdit->setTextColor(color);
    ui->channel_textEdit->append(QString("[" + topic + "]: "));
    ui->channel_textEdit->setTextColor("dark");
    ui->channel_textEdit->append(log);
}
