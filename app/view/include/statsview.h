#ifndef STATSVIEW_H
#define STATSVIEW_H

#include <QStandardItemModel>
#include <QWidget>

namespace Ui {
class StatsView;
}

class StatsView : public QWidget {
  Q_OBJECT

public:
  explicit StatsView(QWidget *parent = nullptr);
  ~StatsView();

private:
  Ui::StatsView *ui;

  void InitView();
  QAbstractItemModel *createStatsModel(QObject *parent, QString playerName);
  void addStatRow(QAbstractItemModel *model, const QString &statsName,
                  const QVariant &value) const;
private slots:
  void UpdateStats(QString name);
};

#endif // STATSVIEW_H
