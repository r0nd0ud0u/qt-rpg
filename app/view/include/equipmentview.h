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
    QAbstractItemModel *createStatsModel(QObject *parent, QString playerName);
    void addStatRow(QAbstractItemModel *model, const QString &stat,
                    const int value) const;
private slots:
    void UpdateStats(QString name);
};

#endif // EQUIPMENTVIEW_H
