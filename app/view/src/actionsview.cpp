#include "actionsview.h"
#include "ui_actionsview.h"

#include "Application.h"
#include "character.h"

#include <QPushButton>
#include <QStandardItemModel>

ActionsView::ActionsView(QWidget *parent)
    : QWidget(parent), ui(new Ui::ActionsView) {
  ui->setupUi(this);
}

ActionsView::~ActionsView() { delete ui; }

void ActionsView::addActionRow(QAbstractItemModel *model,
                               const QVariant &action) const {
  int index = model->rowCount();
  model->insertRow(index);
  model->setData(model->index(index, 0), action);
}

void ActionsView::addInfoActionRow(QAbstractItemModel *model,
                                   const QVariant &statsType,
                                   const QVariant &value) const {
  int index = model->rowCount();
  model->insertRow(index);
  model->setData(model->index(index, 0), statsType);
  model->setData(model->index(index, 1), value);
}

QAbstractItemModel *ActionsView::createModel(QObject *parent,
                                             const ActionsStackedWgType &type) {
  QStandardItemModel *model = nullptr;
  const auto *selectedHero =
      Application::GetInstance().m_GameManager->GetSelectedHero();

  // attak view
  if (type == ActionsStackedWgType::attak) {
    model = new QStandardItemModel(0, 1, parent);
    for (const auto &atk : selectedHero->attakList) {
      addActionRow(model, atk.name);
    }
  } else if (type == ActionsStackedWgType::inventory) {
    model = new QStandardItemModel(0, 2, parent);
    for (size_t i = 0; i < selectedHero->m_Inventory.size(); i++) {
      addActionRow(model,
                   Character::GetInventoryString(static_cast<InventoryType>(
                       selectedHero->m_Inventory.at(i))));
    }
  }

  return model;
}

QAbstractItemModel *
ActionsView::createInfoModel(QObject *parent,
                             const ActionsStackedWgType &type) {
  QStandardItemModel *model = nullptr;

  // Get the current atk selected
  const auto *selectedHero =
      Application::GetInstance().m_GameManager->GetSelectedHero();
  const auto &atkName = ui->actions_table_view->model()->data(m_CurIndex).toString();
  for (const auto &atk : selectedHero->attakList) {
    if (atk.name == atkName) {
      m_CurAtk = atk;
      break;
    }
  }

  if (type == ActionsStackedWgType::attak) {
      // Fill the attak stats view
    model = new QStandardItemModel(0, 2, parent);
    addInfoActionRow(model, ATK_TARGET, m_CurAtk.target);
    addInfoActionRow(model, ATK_REACH, m_CurAtk.reach);
    addInfoActionRow(model, ATK_DURATION, m_CurAtk.turnsDuration);
    addInfoActionRow(model, ATK_MANA_COST, m_CurAtk.manaCost);
    addInfoActionRow(model, ATK_VIGOR_COST, m_CurAtk.vigorCost);
    addInfoActionRow(model, ATK_BERSECK_AGGRO, m_CurAtk.aggroCum);
    addInfoActionRow(model, ATK_DAMAGE, m_CurAtk.damage);
    addInfoActionRow(model, ATK_HEAL, m_CurAtk.heal);
    addInfoActionRow(model, ATK_REGEN_MANA, m_CurAtk.regenMana);
  } else if (type == ActionsStackedWgType::inventory) {
    model = new QStandardItemModel(0, 1, parent);
    for (size_t i = 0; i < selectedHero->m_Inventory.size(); i++) {
      addActionRow(model, selectedHero->m_Inventory.at(i));
    }
  }

  return model;
}

void ActionsView::UpdateActions(const ActionsStackedWgType &type) {
  ui->actions_table_view->setModel(createModel(parentWidget(), type));
  m_CurType = type;
}

void ActionsView::on_actions_table_view_clicked(const QModelIndex &index) {
  // code to activate second table and display stats if atk type
  m_CurIndex = index;
  ui->atk_stats_table->setModel(createInfoModel(parentWidget(), m_CurType));
  ui->validate_action->setEnabled(true);
}

void ActionsView::on_validate_action_clicked() {
  if (m_CurType == ActionsStackedWgType::attak) {
    emit SigLaunchAttak(m_CurAtk.name);
  } else if (m_CurType == ActionsStackedWgType::inventory) {
    emit SigUseObject(m_CurAtk.name);
  }
  // reset to init values the members and view
  ui->validate_action->setEnabled(false);
  m_CurIndex = {};
  m_CurType = ActionsStackedWgType::defaultType;
  m_CurAtk = {};
}

void ActionsView::SetValidateBtnEnabled(const bool value) {
  ui->validate_action->setEnabled(value);
}
