#ifndef EDITSTATPANEL_H
#define EDITSTATPANEL_H

#include <QWidget>

#include "common.h"

namespace Ui {
class EditStatPanel;
}

class EditStatPanel : public QWidget
{
    Q_OBJECT

public:
    explicit EditStatPanel(QWidget *parent = nullptr);
    ~EditStatPanel();

    void Init(const QString& name, const Stats& stats);

    QString m_Name;
    int m_MaxValue = 0;
    int m_StartingValue = 0;
    int m_RegenValue = 0;

private slots:
    void on_start_spinBox_valueChanged(int arg1);

    void on_max_spinBox_valueChanged(int arg1);

    void on_regen_spinBox_valueChanged(int arg1);

private:

    Ui::EditStatPanel *ui;
};

#endif // EDITSTATPANEL_H
