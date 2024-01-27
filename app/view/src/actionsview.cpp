#include "actionsview.h"
#include "ui_actionsview.h"

#include "Application.h"
#include "character.h"

#include <QCheckBox>
#include <QStandardItemModel>

ActionsView::ActionsView(QWidget *parent)
    : QWidget(parent), ui(new Ui::ActionsView) {
  ui->setupUi(this);
}

ActionsView::~ActionsView() {
  delete ui;
  m_CurPlayer = nullptr;
  m_TargetedList.clear();
}

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

QAbstractItemModel *
ActionsView::createModel(QObject *parent,
                         const ActionsStackedWgType &typePage) {
  if (m_CurPlayer == nullptr) {
    return nullptr;
  }
  QStandardItemModel *model = nullptr;

  // attak view
  if (typePage == ActionsStackedWgType::attak) {
    model = new QStandardItemModel(0, 1, parent);
    for (const auto &[atkName, atk] : m_CurPlayer->m_AttakList) {
      addActionRow(model, atkName);
      // disable atk in case of not enough resources, still displayed!
      for (int column = 0; column < model->columnCount(); ++column) {
        auto *item = model->item(model->rowCount() - 1, column);
        if (item != nullptr && !m_CurPlayer->CanBeLaunched(atk)) {
          item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
        }
      }
    }
  } else if (typePage == ActionsStackedWgType::inventory) {
    model = new QStandardItemModel(0, 2, parent);
    for (size_t i = 0; i < m_CurPlayer->m_Inventory.size(); i++) {
      addActionRow(model,
                   Character::GetInventoryString(static_cast<InventoryType>(
                       m_CurPlayer->m_Inventory.at(i))));
    }
  }

  return model;
}

QAbstractItemModel *
ActionsView::createInfoModel(QObject *parent,
                             const ActionsStackedWgType &typePage) {
  if (m_CurPlayer == nullptr) {
    return nullptr;
  }
  QStandardItemModel *model = nullptr;

  const auto &curAtkName =
      ui->actions_table_view->model()->data(m_CurIndex).toString();
  for (const auto &[atkName, atk] : m_CurPlayer->m_AttakList) {
    if (atkName == curAtkName) {
      m_CurAtk = atk;
      break;
    }
  }

  if (typePage == ActionsStackedWgType::attak) {
    // Fill the attak stats view
    model = new QStandardItemModel(0, 2, parent);
    addInfoActionRow(model, ATK_TARGET, m_CurAtk.target);
    addInfoActionRow(model, ATK_REACH, m_CurAtk.reach);
    addInfoActionRow(model, ATK_DURATION, m_CurAtk.turnsDuration);
    addInfoActionRow(model, ATK_MANA_COST, m_CurAtk.manaCost);
    addInfoActionRow(model, ATK_VIGOR_COST, m_CurAtk.vigorCost);
    addInfoActionRow(model, ATK_BERSECK_COST, m_CurAtk.berseckCost);
    addInfoActionRow(model, ATK_AGGRO, m_CurAtk.aggroCum);
    addInfoActionRow(model, ATK_DAMAGE, m_CurAtk.damage);
    addInfoActionRow(model, ATK_LEVEL, m_CurAtk.level);
    addInfoActionRow(model, ATK_HEAL, m_CurAtk.heal);
    addInfoActionRow(model, ATK_REGEN_MANA, m_CurAtk.regenMana);
  } else if (typePage == ActionsStackedWgType::inventory) {
    model = new QStandardItemModel(0, 1, parent);
    for (size_t i = 0; i < m_CurPlayer->m_Inventory.size(); i++) {
      addActionRow(model, m_CurPlayer->m_Inventory.at(i));
    }
  }

  return model;
}

void ActionsView::UpdateActions(const ActionsStackedWgType &type) {
  m_CurPage = type;
  ui->actions_table_view->setModel(createModel(parentWidget(), type));
}

void ActionsView::SetCurrentPlayer(Character *player) { m_CurPlayer = player; }

void ActionsView::on_actions_table_view_clicked(const QModelIndex &index) {
  // code to activate second table and display stats if atk type
  m_CurIndex = index;
  ui->atk_stats_table->setModel(createInfoModel(parentWidget(), m_CurPage));
  // enable/disable targeta
  ProcessEnableTargetsBoxes();
}

void ActionsView::ResetActionsParam() {
  ui->validate_action->setEnabled(false);
  m_CurIndex = {};
  m_CurPage = ActionsStackedWgType::defaultType;
  m_CurAtk = {};
}

void ActionsView::on_validate_action_clicked() {
  if (m_CurPage == ActionsStackedWgType::attak) {
    emit SigLaunchAttak(m_CurAtk.name, m_TargetedList);
  } else if (m_CurPage == ActionsStackedWgType::inventory) {
    emit SigUseObject(m_CurAtk.name);
  }
  // reset to init values the members and view
  ResetActionsParam();
}

void ActionsView::CreateTargetCheckBoxes(
    const QString &activePlayerName,
    const std::vector<Character *> &playerList) {
  for (size_t i = 0; i < playerList.size(); i++) {
    auto *checkbox = new QCheckBox();
    checkbox->setText(playerList[i]->m_Name);
    checkbox->setEnabled(false);
    ui->targets_widget->layout()->addWidget(checkbox);
    // init target
    TargetInfo target;
    target.m_Name = playerList[i]->m_Name;
    target.m_IsTargeted = false;
    m_TargetedList.push_back(target);
    connect(checkbox, &QCheckBox::clicked, this,
            [this, i]() { UpdateTargetList(static_cast<int>(i)); });
  }
}

void ActionsView::InitTargetsWidget() {
  if (m_CurPlayer == nullptr) {
    return;
  }
  const auto &gm = Application::GetInstance().m_GameManager;
  const auto &heroList = gm->m_PlayersManager->m_HeroesList;
  const auto &bossList = gm->m_PlayersManager->m_BossesList;
  CreateTargetCheckBoxes(m_CurPlayer->m_Name, heroList);
  CreateTargetCheckBoxes(m_CurPlayer->m_Name, bossList);
}

void ActionsView::UpdateTargetList(const int index) {
  if (static_cast<size_t>(index) >= m_TargetedList.size()) {
    return;
  }
  m_TargetedList.at(index).m_IsTargeted =
      !m_TargetedList.at(index).m_IsTargeted;

  const bool enableValidateBtn =
      std::any_of(m_TargetedList.begin(), m_TargetedList.end(),
                  [](TargetInfo info) { return info.m_IsTargeted; });
  ui->validate_action->setEnabled(enableValidateBtn);
}

void ActionsView::RemoveTargetsWidget() {
  m_TargetedList.clear();
  for (int i = 0; i < ui->targets_widget->layout()->count(); i++) {
    auto *widget = ui->targets_widget->layout()->itemAt(i)->widget();
    if (widget != nullptr) {
      delete widget;
      widget = nullptr;
    }
  }
}

void ActionsView::ProcessEnableTargetsBoxes() {
  if (m_CurPage == ActionsStackedWgType::attak) {
    for (int i = 0; i < m_TargetedList.size(); i++) {
      if (m_TargetedList[i].m_Name == m_CurPlayer->m_Name) {
        continue;
      }
      auto *wg = static_cast<QCheckBox *>(
          ui->targets_widget->layout()->itemAt(i)->widget());
      if (wg != nullptr) {
        wg->setEnabled(true);
      }
    }
  }
  if (m_CurPage == ActionsStackedWgType::inventory) {
  }
}
