#include "attakview.h"
#include "ui_attakview.h"

#include "Application.h"

#include <QPushButton>

AttakView::AttakView(QWidget *parent) : QWidget(parent), ui(new Ui::AttakView) {
  ui->setupUi(this);
}

AttakView::~AttakView() { delete ui; }

void AttakView::UpdateAttak() {
  // remove current buttons
  RemoveAttak();

  const auto &app = Application::GetInstance();
  if (app.m_GameManager == nullptr ||
      app.m_GameManager->m_PlayersManager == nullptr ||
      app.m_GameManager->m_PlayersManager->m_SelectedHero == nullptr) {
    return;
  }

  for (const auto &atk :
       app.m_GameManager->m_PlayersManager->m_SelectedHero->attakList) {
    auto *button = new QPushButton();
    button->setText(atk.name + " Mana " + QString::number(atk.manaCost) +
                    " Effect: " + atk.effect);
    ui->attak_lay->addWidget(button);
  }
}

void AttakView::RemoveAttak() {
  for (int i = 0; i < ui->attak_lay->count(); i++) {
    auto *widget = ui->attak_lay->itemAt(i)->widget();
    if (widget == nullptr) {
      continue;
    }
    widget->setParent(nullptr);
    ui->attak_lay->removeWidget(widget);
    i--;
  }
}
