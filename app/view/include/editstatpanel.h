#ifndef EDITSTATPANEL_H
#define EDITSTATPANEL_H

#include <QWidget>

#include "common.h"

namespace Ui {
class EditStatPanel;
}

class EditStatPanel : public QWidget {
  Q_OBJECT

public:
  explicit EditStatPanel(QWidget *parent = nullptr);
  ~EditStatPanel();

  void Init(const QString &name, const Stats &stats);
  void SetStatsValue(const StatsType &statsType);

  QString m_Name;
  int m_RawMax = -1;
  int m_CurrRaw = -1;

private slots:
  void on_max_spinBox_valueChanged(int arg1);

    void on_spinBox_valueChanged(int arg1);

private:
  Ui::EditStatPanel *ui;
};

#endif // EDITSTATPANEL_H
