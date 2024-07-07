#ifndef EDITATKPANEL_H
#define EDITATKPANEL_H

#include <QWidget>

#include "effect.h"

namespace Ui {
class EditAtkPanel;
}

class EditAtkPanel : public QWidget {
  Q_OBJECT

public:
  explicit EditAtkPanel(QWidget *parent = nullptr);
  ~EditAtkPanel();

  void UpdatePanel(const effectParam &ep);
  void InitComboBoxes();
  effectParam GetEffectParam() { return m_EffectParam; };

signals:
  void SigValueChanged();
  void SigRemove(EditAtkPanel *panel);

private:
  effectParam m_EffectParam;

private slots:
  void on_stats_comboBox_currentTextChanged(const QString &arg1);

  void on_effect_comboBox_currentTextChanged(const QString &arg1);

  void on_effect_value_spinbox_valueChanged(int arg1);

  void on_target_comboBox_currentTextChanged(const QString &arg1);

  void on_reach_comboBox_currentTextChanged(const QString &arg1);

  void on_value_spinBox_valueChanged(int arg1);

  void on_nb_turns_spinBox_valueChanged(int arg1);

  void on_removeButton_clicked();

private:
  Ui::EditAtkPanel *ui;
};

#endif // EDITATKPANEL_H
