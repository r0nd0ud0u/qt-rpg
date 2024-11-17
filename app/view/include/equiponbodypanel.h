#ifndef EQUIPONBODYPANEL_H
#define EQUIPONBODYPANEL_H

#include <QWidget>

namespace Ui {
class EquipOnBodyPanel;
}

class EquipOnBodyPanel : public QWidget
{
    Q_OBJECT

public:
    explicit EquipOnBodyPanel(QWidget *parent = nullptr);
    ~EquipOnBodyPanel();
    void InitComboBox(const QString& name);
    void SetValue(const QString& value);
    void ResetUi();

    QString m_BodyName;
    QString m_Equipment;

private slots:
    void on_equip_comboBox_currentTextChanged(const QString &arg1);

private:
    Ui::EquipOnBodyPanel *ui;


};

#endif // EQUIPONBODYPANEL_H
