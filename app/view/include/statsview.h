#ifndef STATSVIEW_H
#define STATSVIEW_H

#include <QStandardItemModel>
#include <QWidget>

#include "character.h"

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

  QString m_CurPlayerName;

  QAbstractItemModel *createStatsModel(QObject *parent, const Character* c);
  void addStatRow(QAbstractItemModel *model, const QString &statsName,
                  const QVariant &value) const;
public slots:
  void UpdateDisplayedCharStats(const Character* c);
};

#endif // STATSVIEW_H
