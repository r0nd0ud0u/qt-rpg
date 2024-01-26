#include "channel.h"
#include "ui_channel.h"

#include "Application.h"

Channel::Channel(QWidget *parent) : QWidget(parent), ui(new Ui::Channel) {
  ui->setupUi(this);

  ShowTurnPage();
  // retrueve instance main window and find ui hero panel
  // connect(ui->list_stuffs, &HeroPanel::addStuff, this,
  // &Channel::ShowPageStuffs);
}

Channel::~Channel() { delete ui; }

void Channel::ShowPageStuffs() { ui->stackedWidget->setCurrentIndex(0); }

void Channel::ShowTurnPage() { ui->stackedWidget->setCurrentIndex(1); }

void Channel::AddStuff() { ui->list_stuffs->AddStuff("test"); }

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
