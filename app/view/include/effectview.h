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
    const std::vector<effectParam>& GetTable() const;
    void SetValues(const std::vector<effectParam>& table);
    void SetIndex(const int index);
    void SetVectorSize(const size_t size);

signals:
    void SigTableUpdated();

private slots:
    void on_effect_comboBox_currentTextChanged(const QString &arg1);

    void on_effect_comboBox_2_currentTextChanged(const QString &arg1);

    void on_effect_comboBox_3_currentTextChanged(const QString &arg1);

    void on_value_spinBox_valueChanged(int arg1);

    void on_value_spinBox_2_valueChanged(int arg1);

    void on_value_spinBox_3_valueChanged(int arg1);

    void on_checkBox_stateChanged(int arg1);

    void on_checkBox_2_stateChanged(int arg1);

    void on_checkBox_3_stateChanged(int arg1);

    void on_reach_comboBox_currentTextChanged(const QString &arg1);

    void on_stats_comboBox_currentTextChanged(const QString &arg1);

    void on_stats_comboBox_2_currentTextChanged(const QString &arg1);

    void on_stats_comboBox_3_currentTextChanged(const QString &arg1);

    void on_target_comboBox_currentTextChanged(const QString &arg1);

    void on_target_comboBox_2_currentTextChanged(const QString &arg1);

    void on_target_comboBox_3_currentTextChanged(const QString &arg1);

    void on_reach_comboBox_2_currentTextChanged(const QString &arg1);

    void on_reach_comboBox_3_currentTextChanged(const QString &arg1);

    void on_nb_turns_spinBox_valueChanged(int arg1);

    void on_nb_turns_spinBox_2_valueChanged(int arg1);

    void on_nb_turns_spinBox_3_valueChanged(int arg1);

private:
    Ui::EffectView *ui;
    bool m_FirstShow = false;
    std::vector<std::vector<effectParam>> m_EffectTable; // each atk contains 3 effects at max.
    int m_Index = 0;


};

#endif // EFFECTVIEW_H
