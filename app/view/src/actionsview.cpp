#include "actionsview.h"
#include "ui_actionsview.h"

#include "Application.h"
#include "character.h"

#include <QPushButton>

ActionsView::ActionsView(QWidget *parent)
    : QWidget(parent), ui(new Ui::ActionsView) {
  ui->setupUi(this);
}

ActionsView::~ActionsView() { delete ui; }

void ActionsView::RemoveButtons() {
  for (int i = 0; i < ui->actions_lay->count(); i++) {
    auto *widget = ui->actions_lay->itemAt(i)->widget();
    if (widget == nullptr) {
      continue;
    }
    widget->setParent(nullptr);
    ui->actions_lay->removeWidget(widget);
    i--;
  }
}

void ActionsView::UpdateView(const ActionsStackedWgType &type) {
  // remove current buttons
  RemoveButtons();

  const auto &app = Application::GetInstance();
  if (app.m_GameManager == nullptr ||
      app.m_GameManager->m_PlayersManager == nullptr ||
      app.m_GameManager->m_PlayersManager->m_SelectedHero == nullptr) {
    return;
  }

  // attak view
  if (type == ActionsStackedWgType::attak) {
    for (const auto &atk :
         app.m_GameManager->m_PlayersManager->m_SelectedHero->attakList) {
      auto *button = new QPushButton();

      button->setText(atk.name + " Mana " + QString::number(atk.manaCost) +
                      " Effect: " + atk.effect);
      ui->actions_lay->addWidget(button);
    }
  }

  // inventory view
  const auto &inventory =
      app.m_GameManager->m_PlayersManager->m_SelectedHero->m_Inventory;
  if (type == ActionsStackedWgType::inventory) {
    for (size_t i = 0; i < inventory.size(); i++) {
      if (inventory[i] == 0) {
        continue;
      }
      auto *button = new QPushButton();
      button->setText(Character::GetInventoryString(static_cast<InventoryType>(i)) + " " + QString::number(inventory[i]) + "/" + QString::number(inventory[i]));
      ui->actions_lay->addWidget(button);
    }
  }
}
