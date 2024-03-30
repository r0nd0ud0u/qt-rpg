#include "actionsview.h"
#include "ui_actionsview.h"

#include "Application.h"
#include "character.h"
#include "gamedisplay.h"
#include "utils.h"
#include "rust-rpg-bridge/utils.h"

#include <QCheckBox>
#include <QStandardItemModel>

ActionsView::ActionsView(QWidget *parent)
    : QWidget(parent), ui(new Ui::ActionsView) {
  ui->setupUi(this);
  connect((GameDisplay *)parentWidget(), &GameDisplay::SigBossDead, this,
          &ActionsView::RemoveTarget);
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
    // TODO maybe use another container got atk list
    std::vector<AttaqueType> tmpAtkList;
    for (const auto &[atkName, atk] : m_CurPlayer->m_AttakList) {
      tmpAtkList.push_back(atk);
    }
    std::sort(tmpAtkList.begin(), tmpAtkList.end(), Utils::CompareByLevel);
    // for init
    if (!m_CurPlayer->m_Forms.empty() && m_Form == STANDARD_FORM) {
      m_Form = m_CurPlayer->m_Forms.front();
    } else {
      m_Form = m_CurPlayer->m_SelectedForm;
    }
    for (const auto &atk : tmpAtkList) {
      if (m_Form != atk.form) {
        continue;
      }
      addActionRow(model, atk.name);
      // disable atk in case of not enough resources, still displayed!
      for (int column = 0; column < model->columnCount(); ++column) {
        auto *item = model->item(model->rowCount() - 1, column);
        if (item != nullptr && !m_CurPlayer->CanBeLaunched(atk)) {
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
        const auto effectName = build_effect_name("e.effect", "e.effect");
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
  for (auto &tg : m_TargetedList) {
    tg.m_IsTargeted = false;
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
    const QString &activePlayerName,
    const std::vector<Character *> &playerList) {
  for (const auto &ply : playerList) {
    auto *checkbox = new QCheckBox();
    checkbox->setText(ply->m_Name);
    checkbox->setEnabled(false);
    ui->targets_widget->layout()->addWidget(checkbox);
    // init target
    TargetInfo target;
    target.m_Name = ply->m_Name;
    target.m_IsTargeted = false;
    target.m_IsBoss = ply->m_type == characType::Boss;
    m_TargetedList.push_back(target);
    const QString &name = target.m_Name;
    connect(checkbox, &QCheckBox::clicked, this,
            [this, name]() { UpdateTargetList(name); });
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

void ActionsView::UpdateTargetList(const QString &name) {
  for (int i = 0; i < m_TargetedList.size(); i++) {
    const auto test = m_TargetedList[i];
    auto *wg = static_cast<QCheckBox *>(
        ui->targets_widget->layout()->itemAt(i)->widget());
    if (wg == nullptr) {
      continue;
    }
    if (!wg->isEnabled()) {
      continue;
    }

    if (name == m_TargetedList[i].m_Name) {
      m_TargetedList[i].m_IsTargeted = !m_TargetedList[i].m_IsTargeted;
    } else if ((m_CurPlayer->m_type == characType::Hero &&
                !m_TargetedList[i].m_IsBoss) ||
               (m_CurPlayer->m_type == characType::Boss &&
                m_TargetedList[i].m_IsBoss) ||
               (m_CurPlayer->m_type == characType::Hero &&
                m_TargetedList[i].m_IsBoss) ||
               (m_CurPlayer->m_type == characType::Boss &&
                !m_TargetedList[i].m_IsBoss)) {
      if (m_CurAtk.reach == REACH_ZONE) {
        m_TargetedList[i].m_IsTargeted = !m_TargetedList[i].m_IsTargeted;
      } else if (m_CurAtk.reach == REACH_INDIVIDUAL &&
                 m_TargetedList[i].m_IsTargeted) {
        m_TargetedList[i].m_IsTargeted = false;
      }
    } else if (m_CurAtk.reach == REACH_INDIVIDUAL &&
               m_TargetedList[i].m_IsTargeted) {
      m_TargetedList[i].m_IsTargeted = false;
    }
    if (wg != nullptr) {
      wg->setChecked(m_TargetedList[i].m_IsTargeted);
    }
  }
  const bool enableValidateBtn =
      std::any_of(m_TargetedList.begin(), m_TargetedList.end(),
                  [](const TargetInfo &info) { return info.m_IsTargeted; });
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

      if (m_CurAtk.target == TARGET_HIMSELF &&
          m_TargetedList[i].m_Name != m_CurPlayer->m_Name) {
        continue;
      } else if (m_CurPlayer->m_type == characType::Hero) {
        // TODO never entering here
        if (m_TargetedList[i].m_IsBoss && m_CurAtk.target != TARGET_ENNEMY) {
          continue;
        }
        if (!m_TargetedList[i].m_IsBoss &&
            !(m_CurAtk.target == TARGET_ALLY ||
              m_CurAtk.target == TARGET_ALL_HEROES ||
              m_CurAtk.target == TARGET_HIMSELF)) {
          continue;
        }
      } else if (m_CurPlayer->m_type == characType::Boss) {
        if (m_TargetedList[i].m_IsBoss && m_CurAtk.target == TARGET_ENNEMY) {
          continue;
        }
        if (!m_TargetedList[i].m_IsBoss && m_CurAtk.target != TARGET_ENNEMY) {
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
  auto *lay = ui->targets_widget->layout();

  int i = 0;
  for (const auto &it : m_TargetedList) {
    if (it.m_Name == targetName) {
      break;
    }
    i++;
  }

  const auto newEnd = std::remove_if(
      m_TargetedList.begin(), m_TargetedList.end(),
      [&targetName](const TargetInfo &ti) {
        return targetName == ti.m_Name; // remove elements where this is true
      });
  m_TargetedList.erase(newEnd, m_TargetedList.end());
  auto *widget = lay->itemAt(i)->widget();
  widget->hide();
  lay->removeItem(lay->itemAt(i));
  lay->removeWidget(widget);
  delete widget;
}
