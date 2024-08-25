#ifndef STUFFPANEL_H
#define STUFFPANEL_H

#include <QWidget>

namespace Ui {
class StuffPanel;
}

class StuffPanel : public QWidget
{
    Q_OBJECT

public:
    explicit StuffPanel(QWidget *parent = nullptr);
    ~StuffPanel();
    void Init(const QString &name);
    void ResetValues();

    QString m_Name;
    bool m_IsPercent = false;
    int m_BufValue = 0;


private slots:
    void on_percent_radioButton_toggled(bool checked);

    void on_value_spinBox_valueChanged(int arg1);

private:
    Ui::StuffPanel *ui;
};

#endif // STUFFPANEL_H
