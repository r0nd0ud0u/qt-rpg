#ifndef ACTIONSVIEW_H
#define ACTIONSVIEW_H

#include <QAbstractItemModel>
#include <QWidget>

#include "playersmanager.h"

#include "gamedisplay.h"

namespace Ui {
class ActionsView;
}

class ActionsView : public QWidget {
  Q_OBJECT

public:
  explicit ActionsView(QWidget *parent = nullptr);
  ~ActionsView();
  void UpdateActions(const ActionsStackedWgType &type);
  void SetCurrentPlayer(Character *player);
  void ResetActionsParam();
  void InitTargetsWidget(const PlayersManager *pm);
  void SetForm(const QString &form);
  void RemoveTarget(QString);
  void ResetUi();

private:
  Ui::ActionsView *ui;
  QModelIndex m_CurIndex;
  AttaqueType m_CurAtk;
  QString m_CurObject;
  Character *m_CurPlayer = nullptr;
  std::vector<TargetInfo *> m_TargetedList;
  ActionsStackedWgType m_CurPage = ActionsStackedWgType::defaultType;
  QString m_Form = STANDARD_FORM;

  // Table of attaks
  QAbstractItemModel *createModel(QObject *parent,
                                  const ActionsStackedWgType &typePage);
  void addActionRow(QAbstractItemModel *model, const QVariant &action,
                    const std::optional<QString> &reason) const;
  // Table of stats of the selected atk
  QAbstractItemModel *createInfoModel(QObject *parent,
                                      const ActionsStackedWgType &typePage);
  void addInfoActionRow(QAbstractItemModel *model, const QVariant &statsType,
                        const QVariant &value) const;
  // Target available of the selected atk
  void DisableTargetsBox() const;
  void CreateTargetCheckBoxes(const std::vector<Character *> &playerList);
  void ProcessEnableTargetsBoxes();
  void RemoveAllTargets();

signals:
  void SigLaunchAttak(const QString &atkName,
                      const std::vector<TargetInfo *> &targetList);
private slots:
  // Slots Table atk
  void on_actions_table_view_clicked(const QModelIndex &index);
  // slots validate button
  void on_validate_action_clicked();
  // slots of target widget
  void UpdateTargetList(const QString &name);
};

#endif // ACTIONSVIEW_H
