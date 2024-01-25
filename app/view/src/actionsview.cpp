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
                               const QString &actionName) const {
  int index = model->rowCount();
  model->insertRow(index);
  model->setData(model->index(index, 0), actionName);
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

void ActionsView::UpdateActions(const ActionsStackedWgType &type) {
  ui->actions_table_view->setModel(createModel(parentWidget(), type));
}

void ActionsView::LaunchAttak() {
  // on who ?
  // enable buttons on allies or ennemies
}
