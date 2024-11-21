#include "actionsview.h"
#include "ui_actionsview.h"

#include "Application.h"
#include "character.h"
#include "gamedisplay.h"
#include "rust-rpg-bridge/players_manager.h"
#include "rust-rpg-bridge/utils.h"
#include "utils.h"

#include <QCheckBox>
#include <QStandardItemModel>

ActionsView::ActionsView(QWidget *parent)
    : QWidget(parent), ui(new Ui::ActionsView) {
  ui->setupUi(this);
}

ActionsView::~ActionsView() {
  m_CurPlayer = nullptr;
  m_TargetedList.clear();
  delete ui;
}

void ActionsView::RemoveAllTargets() {
  // delete widget
  auto *lay = ui->targets_widget->layout();
  if(lay ==nullptr){
      return;
  }
  while (QLayoutItem *item = lay->takeAt(0)) {
      if (QWidget *widget = item->widget()) {
          widget->hide();
          widget->deleteLater();
      }
      delete item; // Deletes the layout item
  }
  m_TargetedList.clear();
}

void ActionsView::addActionRow(QAbstractItemModel *model,
                               const QVariant &action,
                               const std::optional<QString> &reason) const {
  int index = model->rowCount();
  model->insertRow(index);
  model->setData(model->index(index, 0), action);
  if (reason != std::nullopt) {
    model->setData(model->index(index, 1), reason.value());
  }
}

void ActionsView::addInfoActionRow(QAbstractItemModel *model,
                                   const QVariant &statsType,
                                   const QVariant &value) const {
  const int index = model->rowCount();
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
    model = new QStandardItemModel(0, 2, parent);
    // for init
    if (!m_CurPlayer->m_Forms.empty() && m_Form == STANDARD_FORM) {
      m_Form = m_CurPlayer->m_Forms.front();
    } else {
      m_Form = m_CurPlayer->m_SelectedForm;
    }
    for (const auto &atk : m_CurPlayer->m_AtksByLevel) {
      if (m_Form != atk.form) {
        continue;
      }
      const auto [canBeLaunched, reason] = m_CurPlayer->CanBeLaunched(atk);
      addActionRow(model, atk.name, reason);
      // disable atk in case of not enough resources, still displayed!
      for (int column = 0; column < model->columnCount(); ++column) {
        auto *item = model->item(model->rowCount() - 1, column);
        if (item != nullptr && !canBeLaunched) {
          item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
        }
      }
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
    addInfoActionRow(model, ATK_MANA_COST, m_CurAtk.manaCost);
    addInfoActionRow(model, ATK_VIGOR_COST, m_CurAtk.vigorCost);
    addInfoActionRow(model, ATK_BERSECK_COST, m_CurAtk.berseckCost);
    addInfoActionRow(model, ATK_LEVEL, m_CurAtk.level);
    for (const auto &e : m_CurAtk.m_AllEffects) {
      const auto effectName = build_effect_name(
          e.effect.toStdString(), e.statsName.toStdString(), true);
      addInfoActionRow(model, effectName.data(), e.value);
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
  // enable/disable target
  for (auto *tg : m_TargetedList) {
    if (tg != nullptr) {
      tg->set_is_targeted(false);
    }
  }
  ProcessEnableTargetsBoxes();
}

void ActionsView::ResetActionsParam() {
  DisableTargetsBox();
  ui->validate_action->setEnabled(false);
  m_CurIndex = {};
  m_CurPage = ActionsStackedWgType::defaultType;
  m_CurAtk = {};
}

void ActionsView::on_validate_action_clicked() {
  if (m_CurPage == ActionsStackedWgType::attak) {
    emit SigLaunchAttak(m_CurAtk.name, m_TargetedList);
  }
  // reset to init values the members and view
  ResetActionsParam();
}

void ActionsView::CreateTargetCheckBoxes(
    const std::vector<Character *> &playerList) {
  for (const auto *ply : playerList) {
    if (ply == nullptr) {
      continue;
    }
    auto *checkbox = new QCheckBox();
    checkbox->setText(ply->m_Name);
    checkbox->setEnabled(false);
    ui->targets_widget->layout()->addWidget(checkbox);
    // init target
    auto *target = target_info_new().into_raw();
    const auto t = ply->m_Name.toStdString();
    target->set_name(ply->m_Name.toStdString());
    target->set_is_targeted(false);
    target->set_is_boss(ply->m_type == characType::Boss);
    m_TargetedList.push_back(target);
    const QString &name = target->get_name().data();
    connect(checkbox, &QCheckBox::clicked, this,
            [this, name]() { UpdateTargetList(name); });
  }
}

void ActionsView::InitTargetsWidget(const PlayersManager *pm) {
  if (pm == nullptr) {
    return;
  }
  RemoveAllTargets();
  CreateTargetCheckBoxes(pm->m_HeroesList);
  CreateTargetCheckBoxes(pm->m_BossesList);
}

void ActionsView::UpdateTargetList(const QString &name) {
  for (int i = 0; i < m_TargetedList.size(); i++) {
    auto *target = m_TargetedList[i];
    if (target == nullptr) {
      continue;
    }
    auto *wg = static_cast<QCheckBox *>(
        ui->targets_widget->layout()->itemAt(i)->widget());
    if (wg == nullptr) {
      continue;
    }
    if (!wg->isEnabled()) {
      continue;
    }

    if (name == target->get_name().data()) {
      target->set_is_targeted(!target->get_is_targeted());

    } else if ((m_CurPlayer->m_type == characType::Hero &&
                !target->get_is_boss()) ||
               (m_CurPlayer->m_type == characType::Boss &&
                target->get_is_boss()) ||
               (m_CurPlayer->m_type == characType::Hero &&
                target->get_is_boss()) ||
               (m_CurPlayer->m_type == characType::Boss &&
                !target->get_is_boss())) {
      if (m_CurAtk.reach == REACH_ZONE) {
        target->set_is_targeted(!target->get_is_targeted());
      } else if (m_CurAtk.reach == REACH_INDIVIDUAL &&
                 target->get_is_targeted()) {
        target->set_is_targeted(false);
      }
    } else if (m_CurAtk.reach == REACH_INDIVIDUAL &&
               target->get_is_targeted()) {
      target->set_is_targeted(false);
    }
    if (wg != nullptr) {
      wg->setChecked(target->get_is_targeted());
    }
  }
  const bool enableValidateBtn = std::any_of(
      m_TargetedList.begin(), m_TargetedList.end(), [](const TargetInfo *info) {
        if (info == nullptr) {
          return false;
        } else {
          return info->get_is_targeted();
        }
      });
  ui->validate_action->setEnabled(enableValidateBtn);
}

void ActionsView::DisableTargetsBox() const {
  for (int i = 0; i < ui->targets_widget->layout()->count(); i++) {
    auto *wg = static_cast<QCheckBox *>(
        ui->targets_widget->layout()->itemAt(i)->widget());
    if (wg != nullptr) {
      wg->setCheckState(Qt::CheckState::Unchecked);
      wg->setEnabled(false);
    }
  }
}

void ActionsView::ProcessEnableTargetsBoxes() {
  DisableTargetsBox();
  if (m_CurPage == ActionsStackedWgType::attak) {
    for (int i = 0; i < m_TargetedList.size(); i++) {

      const auto target = m_TargetedList[i];
      if (target == nullptr) {
        continue;
      }
      if (m_CurAtk.target == TARGET_HIMSELF &&
          target->get_name().data() != m_CurPlayer->m_Name) {
        continue;
      } else if (m_CurPlayer->m_type == characType::Hero) {
        // TODO never entering here
        if (target->get_is_boss() && m_CurAtk.target != TARGET_ENNEMY) {
          continue;
        }
        if (!target->get_is_boss() && !(m_CurAtk.target == TARGET_ALLY ||
                                        m_CurAtk.target == TARGET_ALL_HEROES ||
                                        m_CurAtk.target == TARGET_HIMSELF)) {
          continue;
        }
        if (!target->get_is_boss() && !(m_CurAtk.target == TARGET_ALLY ||
                                        m_CurAtk.target == TARGET_ALL_HEROES ||
                                        m_CurAtk.target == TARGET_HIMSELF)) {
          continue;
        }
      } else if (m_CurPlayer->m_type == characType::Boss) {
        if (target->get_is_boss() && m_CurAtk.target == TARGET_ENNEMY) {
          continue;
        }
        if (!target->get_is_boss() && m_CurAtk.target != TARGET_ENNEMY) {
          continue;
        }
      }

      auto *wg = static_cast<QCheckBox *>(
          ui->targets_widget->layout()->itemAt(i)->widget());
      if (wg != nullptr) {
        wg->setEnabled(true);
      }
    }
  }
}

void ActionsView::SetForm(const QString &form) { m_Form = form; }

void ActionsView::RemoveTarget(QString targetName) {
  if (targetName.isEmpty()) {
    return;
  }

  int i = 0;
  for (const auto *it : m_TargetedList) {
    if (it != nullptr && it->get_name().data() == targetName) {
      break;
    }
    i++;
  }

  const auto newEnd = std::remove_if(
      m_TargetedList.begin(), m_TargetedList.end(),
      [&targetName](const TargetInfo *ti) {
        if (ti == nullptr) {
          return true;
        } else {
          return targetName ==
                 ti->get_name().data(); // remove elements where this is true
        }
      });
  m_TargetedList.erase(newEnd, m_TargetedList.end());

  // delete widget
  auto *lay = ui->targets_widget->layout();
  if (lay == nullptr) {
    return;
  }
  if (lay->count() <= i) {
    return;
  }
  auto *widget = lay->itemAt(i)->widget();
  widget->hide();
  lay->removeItem(lay->itemAt(i));
  lay->removeWidget(widget);
  delete widget;
}

void ActionsView::ResetUi(){
    RemoveAllTargets();
}
