#ifndef EQUIPMENTVIEW_H
#define EQUIPMENTVIEW_H

#include <QStandardItemModel>
#include <QWidget>

#include "character.h"

namespace Ui {
class EquipmentView;
}

class EquipmentView : public QWidget {
  Q_OBJECT

public:
  explicit EquipmentView(QWidget *parent = nullptr);
  ~EquipmentView();

private:
  Ui::EquipmentView *ui;

  void addEquipmentRow(QAbstractItemModel *model, const QString &body,
                       const QString equipmentName) const;
  QAbstractItemModel *createEquipmentModel(QObject *parent, Character *c);
public slots:
  void UpdateEquipment(Character *c);
};

#endif // EQUIPMENTVIEW_H
