#ifndef STATSVIEW_H
#define STATSVIEW_H

#include <QWidget>

#include <QSqlTableModel>

#include <QStandardItemModel>
#include <QTime>

namespace Ui {
class StatsView;
}

class StatsView : public QWidget
{
    Q_OBJECT

public:
    explicit StatsView(QWidget *parent = nullptr);
    ~StatsView();

private:
    Ui::StatsView *ui;

    void InitView();
    void initializeModel(QSqlTableModel *model);

    void addMail(QAbstractItemModel *model, const QString &subject,
                            const int sender);
    QAbstractItemModel *createMailModel(QObject *parent);
};

#endif // STATSVIEW_H
