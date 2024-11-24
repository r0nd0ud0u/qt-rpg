#ifndef EDITATTAKVIEW_H
#define EDITATTAKVIEW_H

#include <QStringListModel>
#include <QWidget>

#include "character.h"

#include <vector>

struct EditAttak {
  AttaqueType type;
  bool updated = false;
};

namespace Ui {
class EditAttakView;
}

class EditAttakView : public QWidget {
  Q_OBJECT

public:
  explicit EditAttakView(QWidget *parent = nullptr);
  ~EditAttakView();
  void InitView(Character* c);
  void Save();
  void InitComboBoxes();
  void InitDefaultView();

private slots:
  void on_apply_button_clicked();

  void on_atk_list_view_clicked(const QModelIndex &index);

  void on_photo_comboBox_currentTextChanged(const QString &arg1);

  void on_name_lineEdit_textChanged(const QString &arg1);

  void on_mana_cost_spinBox_valueChanged(int arg1);

  void on_new_atk_button_clicked();

  void on_target_comboBox_currentTextChanged(const QString &arg1);

  void on_reach_comboBox_currentTextChanged(const QString &arg1);

  void on_level_spinBox_valueChanged(int arg1);

  void on_form_comboBox_currentTextChanged(const QString &arg1);

  void on_sound_comboBox_currentTextChanged(const QString &arg1);

  private:
  Ui::EditAttakView *ui;
  std::vector<EditAttak> m_AttakList;
  Character* m_CurCharacter = nullptr;

  void UpdateValues(const EditAttak &selectedAttak, const int index);
  void Apply();
  void OnValueChange(const int index);
  int GetIndexSelectedRow() const;
  void EnableAllWidgets(const bool value) const;
  void UpdateEffectOn();

  bool m_FirstShow = false;
};

#endif // EDITATTAKVIEW_H
