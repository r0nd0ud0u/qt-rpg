#ifndef EQUIPMENTVIEW_H
#define EQUIPMENTVIEW_H

#include <QStandardItemModel>
#include <QWidget>

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
  void InitView();
  QAbstractItemModel *createEquipmentModel(QObject *parent);
private slots:
  void UpdateEquipment(QString name);
};

#endif // EQUIPMENTVIEW_H
