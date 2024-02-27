#ifndef EQUIPONBODYVIEW_H
#define EQUIPONBODYVIEW_H

#include <QWidget>
#include "equiponbodypanel.h"

// include for editStuff struct
#include "stuffsview.h"
#include "character.h"

namespace Ui {
class EquipOnBodyView;
}

class EquipOnBodyView : public QWidget
{
    Q_OBJECT

public:
    explicit EquipOnBodyView(QWidget *parent = nullptr);
    ~EquipOnBodyView();
    void InitEditEquipment() ;
    void AddItemInComboBox(const EditStuff& es);
    void InitView(Character* player);
    std::unordered_map<QString, QString> GetCurrentEquipmentTable();

private:
    Ui::EquipOnBodyView *ui;
    std::vector<EquipOnBodyPanel*> m_PanelList;
};

#endif // EQUIPONBODYVIEW_H
