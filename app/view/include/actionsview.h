#ifndef ACTIONSVIEW_H
#define ACTIONSVIEW_H

#include <QWidget>
#include <QAbstractItemModel>

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

  QAbstractItemModel *createModel(QObject *parent,const ActionsStackedWgType &type);
  void addActionRow(QAbstractItemModel *model, const QString &actionName) const;


private slots:
  void LaunchAttak();

};

#endif // ACTIONSVIEW_H
