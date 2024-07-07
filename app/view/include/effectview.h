#ifndef EFFECTVIEW_H
#define EFFECTVIEW_H

#include <QWidget>

#include "editatkpanel.h"
#include "effect.h"

namespace Ui {
class EffectView;
}

class EffectView : public QWidget {
  Q_OBJECT

public:
  explicit EffectView(QWidget *parent = nullptr);
  ~EffectView();
  const std::vector<effectParam> &GetTable() const;
  void SetVectorSize(const size_t size);
  void InitValues(const std::vector<effectParam> &table);
  void SetIndex(const int index);
  std::vector<EditAtkPanel *> m_AtkPanels;
  void InitEditAtkPanels();

signals:
  void SigTableUpdated();

private slots:
  void AddEditAtkPanel(const effectParam &ep);
  void SlotValueChange();
  void SlotRemove(EditAtkPanel *panel);
  void on_add_effect_Button_clicked();

private:
  void RemovePanel(EditAtkPanel *panel);

  Ui::EffectView *ui;
  bool m_FirstShow = false;
  std::vector<std::vector<effectParam>> m_EffectTable;
  int m_Index = 0;
};

#endif // EFFECTVIEW_H
