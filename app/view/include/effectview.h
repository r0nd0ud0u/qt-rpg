#ifndef EFFECTVIEW_H
#define EFFECTVIEW_H

#include <QWidget>

#include "effect.h"

namespace Ui {
class EffectView;
}

class EffectView : public QWidget
{
    Q_OBJECT

public:
    explicit EffectView(QWidget *parent = nullptr);
    ~EffectView();
    void InitComboBoxes();
    const std::vector<effectParam>& GetTable();

private slots:
    void on_effect_comboBox_currentTextChanged(const QString &arg1);

    void on_effect_comboBox_2_currentTextChanged(const QString &arg1);

    void on_effect_comboBox_3_currentTextChanged(const QString &arg1);

    void on_value_spinBox_valueChanged(int arg1);

    void on_value_spinBox_2_valueChanged(int arg1);

    void on_value_spinBox_3_valueChanged(int arg1);

private:
    Ui::EffectView *ui;
    bool m_FirstShow = false;
    std::vector<effectParam> m_EffectTable;
};

#endif // EFFECTVIEW_H
