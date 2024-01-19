#ifndef EQUIPMENTVIEW_H
#define EQUIPMENTVIEW_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class EquipmentView;
}

class EquipmentView : public QWidget
{
    Q_OBJECT

public:
    explicit EquipmentView(QWidget *parent = nullptr);
    ~EquipmentView();

private:
    Ui::EquipmentView *ui;

    void InitView();
    QAbstractItemModel *createEquipmentModel(QObject *parent, QString playerName);
    void addEquipmentRow(QAbstractItemModel *model, const QString &body,
                                        const QString equipmentName) const;
private slots:
    void UpdateEquipment(QString name);
};

#endif // EQUIPMENTVIEW_H
