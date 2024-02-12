#ifndef ALLEFFECTPANEL_H
#define ALLEFFECTPANEL_H

#include <QStandardItemModel>
#include <QWidget>

#include "effect.h"

namespace Ui {
class AllEffectPanel;
}

class AllEffectPanel : public QWidget {
  Q_OBJECT

public:
  explicit AllEffectPanel(QWidget *parent = nullptr);
  ~AllEffectPanel();

private:
  Ui::AllEffectPanel *ui;

  void addRow(QAbstractItemModel *model, const effectParam* ep, const QString& launcher, const QString& targetName) const;
  void InitView();
  QAbstractItemModel *createModel(QObject *parent);
private slots:
  void UpdateModel(const std::vector<effectParam>& epTable, const QString& launcher, const QString& targetName);
};

#endif // ALLEFFECTPANEL_H
