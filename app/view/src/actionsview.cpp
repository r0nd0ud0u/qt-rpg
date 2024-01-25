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
                             const ActionsStackedWgType &type, const QModelIndex& index) {
  QStandardItemModel *model = nullptr;
  const auto *selectedHero =
      Application::GetInstance().m_GameManager->GetSelectedHero();

  const auto& atkName = ui->actions_table_view->model()->data(index).toString();
  AttaqueType curAtk;
  for(const auto& atk : selectedHero->attakList){
      if(atk.name == atkName){
          curAtk = atk;
          break;
      }
  }

  // attak view
  if (type == ActionsStackedWgType::attak) {
    model = new QStandardItemModel(0, 2, parent);
      addInfoActionRow(model, ATK_TARGET, curAtk.target);
      addInfoActionRow(model, ATK_REACH,  curAtk.reach);
      addInfoActionRow(model, ATK_DURATION, curAtk.turnsDuration);
      addInfoActionRow(model, ATK_MANA_COST,  curAtk.manaCost);
      addInfoActionRow(model, ATK_VIGOR_COST,  curAtk.vigorCost);
      addInfoActionRow(model, ATK_BERSECK_AGGRO,  curAtk.aggroCum);
      addInfoActionRow(model, ATK_DAMAGE, curAtk.damage);
      addInfoActionRow(model, ATK_HEAL,  curAtk.heal);
      addInfoActionRow(model, ATK_REGEN_MANA,  curAtk.regenMana);
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

void ActionsView::LaunchAttak() {
  // on who ?
  // enable buttons on allies or ennemies
}

void ActionsView::on_actions_table_view_clicked(const QModelIndex &index) {
  // code to activate second table and display stats if atk type
  ui->atk_stats_table->setModel(createInfoModel(parentWidget(), m_CurType, index));
}
