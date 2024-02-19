#ifndef ALLEFFECTPANEL_H
#define ALLEFFECTPANEL_H

#include <QStandardItemModel>
#include <QWidget>

#include "effect.h"
#include "playersmanager.h"

#include <unordered_map>
#include <vector>

enum class columnsPanel {
  effect = 0,
  atk,
  targetName,
  launcherName,
  remainingTurns,
  value,
  enumSize
};

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

  void addRow(QAbstractItemModel *model, const effectParam *ep,
              const QString &launcher, const QString &targetName,
              const QString &atkName) const;
  void InitView();
  QAbstractItemModel *createModel(QObject *parent) const;
private slots:
  void ResetModelWithAllEffect(
      const std::unordered_map<QString, std::vector<GameAtkEffects>> &table);
};

#endif // ALLEFFECTPANEL_H
