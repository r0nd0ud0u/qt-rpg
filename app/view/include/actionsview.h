#ifndef ACTIONSVIEW_H
#define ACTIONSVIEW_H

#include <QAbstractItemModel>
#include <QWidget>

#include "character.h"

enum class ActionsStackedWgType { attak, inventory, defaultType, enumSize };

namespace Ui {
class ActionsView;
}

class ActionsView : public QWidget {
  Q_OBJECT

public:
  explicit ActionsView(QWidget *parent = nullptr);
  ~ActionsView();
  void UpdateActions(const ActionsStackedWgType &type);
  void SetValidateBtnEnabled(const bool value);

private:
  Ui::ActionsView *ui;
  ActionsStackedWgType m_CurType = ActionsStackedWgType::defaultType;
  QModelIndex m_CurIndex;
  AttaqueType m_CurAtk;

  QAbstractItemModel *createModel(QObject *parent,
                                  const ActionsStackedWgType &type);
  void addActionRow(QAbstractItemModel *model, const QVariant &action) const;
  void addInfoActionRow(QAbstractItemModel *model, const QVariant &statsType,
                        const QVariant &value) const;
  QAbstractItemModel *createInfoModel(QObject *parent,
                                      const ActionsStackedWgType &type);
signals:
  void SigLaunchAttak(const QString &atkName);
  void SigUseObject(const QString &objName);

  void on_actions_table_view_clicked(const QModelIndex &index);
  void on_validate_action_clicked();
};

#endif // ACTIONSVIEW_H
