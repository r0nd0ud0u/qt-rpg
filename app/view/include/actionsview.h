#ifndef ACTIONSVIEW_H
#define ACTIONSVIEW_H

#include <QAbstractItemModel>
#include <QWidget>

#include "character.h"

enum class ActionsStackedWgType { attak, inventory, defaultType, enumSize };

struct TargetInfo{
    QString m_Name;
    bool m_IsTargeted = false;
};

namespace Ui {
class ActionsView;
}

class ActionsView : public QWidget {
  Q_OBJECT

public:
  explicit ActionsView(QWidget *parent = nullptr);
  ~ActionsView();
  void UpdateActions(const ActionsStackedWgType &type);
  void ResetActionsParam();

  private:
  Ui::ActionsView *ui;
  QModelIndex m_CurIndex;
  AttaqueType m_CurAtk;
  std::vector<TargetInfo> m_TargetedList;
  ActionsStackedWgType m_CurPage = ActionsStackedWgType::defaultType;

  // Table of attaks
  QAbstractItemModel *createModel(QObject *parent,const ActionsStackedWgType& typePage);
  void addActionRow(QAbstractItemModel *model, const QVariant &action) const;
  // Table of stats of the selected atk
  QAbstractItemModel *createInfoModel(QObject *parent,const ActionsStackedWgType& typePage);
  void addInfoActionRow(QAbstractItemModel *model, const QVariant &statsType,
                        const QVariant &value) const;
  // Target available of the selected atk
  void InitTargetsWidget();
  void RemoveTargetsWidget();
signals:
  void SigLaunchAttak(const QString &atkName, const std::vector<TargetInfo>& targetList);
  void SigUseObject(const QString &objName);

private slots:
  // Slots Table atk
  void on_actions_table_view_clicked(const QModelIndex &index);
  // slots validate button
  void on_validate_action_clicked();
  // slots of target widget
  void UpdateTargetList(const int index);
};

#endif // ACTIONSVIEW_H
