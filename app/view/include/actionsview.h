#ifndef ACTIONSVIEW_H
#define ACTIONSVIEW_H

#include <QAbstractItemModel>
#include <QWidget>

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

private:
  Ui::ActionsView *ui;
  ActionsStackedWgType m_CurType = ActionsStackedWgType::defaultType;

  QAbstractItemModel *createModel(QObject *parent,
                                  const ActionsStackedWgType &type);
  void addActionRow(QAbstractItemModel *model, const QVariant &action) const;
  void addInfoActionRow(QAbstractItemModel *model,
                                     const QVariant &statsType,
                        const QVariant &value) const;
  QAbstractItemModel *createInfoModel(QObject *parent,
                                      const ActionsStackedWgType &type, const QModelIndex& index);

private slots:
  void LaunchAttak();

  void on_actions_table_view_clicked(const QModelIndex &index);
};

#endif // ACTIONSVIEW_H
