#ifndef EQUIPONBODYVIEW_H
#define EQUIPONBODYVIEW_H

#include "equiponbodypanel.h"
#include <QWidget>

// include for editStuff struct
#include "character.h"
#include "stuffsview.h"

namespace Ui {
class EquipOnBodyView;
}

class EquipOnBodyView : public QWidget {
  Q_OBJECT

public:
  explicit EquipOnBodyView(QWidget *parent = nullptr);
  ~EquipOnBodyView();
  void InitEditEquipment();
  void InitView(Character *player) const;
  std::unordered_map<QString, QString> GetCurrentEquipmentTable() const;
  void ResetUi();
  void UpdateView();

private:
  Ui::EquipOnBodyView *ui;
  std::vector<EquipOnBodyPanel *> m_PanelList;
};

#endif // EQUIPONBODYVIEW_H
