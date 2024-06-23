#ifndef EFFECTVIEW_H
#define EFFECTVIEW_H

#include <QWidget>

#include "effect.h"

#include "editatkpanel.h"

namespace Ui {
class EffectView;
}

class EffectView : public QWidget
{
    Q_OBJECT

public:
    explicit EffectView(QWidget *parent = nullptr);
    ~EffectView();
    // void InitComboBoxes();
    const std::vector<effectParam>& GetTable() const;
    void SetVectorSize(const size_t size);
    void InitValues(const std::vector<effectParam>& table);
    void SetIndex(const int index);
    std::vector<EditAtkPanel *> m_AtkPanels;
    void InitEditAtkPanels();

signals:
    void SigTableUpdated();

private slots:

    void AddEditAtkPanel(const effectParam& ep);
    // void on_effect_comboBox_currentTextChanged(const QString &arg1);

    // void on_value_spinBox_valueChanged(int arg1);

    // void on_checkBox_stateChanged(int arg1);

    // void on_reach_comboBox_currentTextChanged(const QString &arg1);

    // void on_stats_comboBox_currentTextChanged(const QString &arg1);

    // void on_target_comboBox_currentTextChanged(const QString &arg1);

    // void on_nb_turns_spinBox_valueChanged(int arg1);

    // void on_effect_value_spinbox_valueChanged(int arg1);


private:
    Ui::EffectView *ui;
    bool m_FirstShow = false;
    std::vector<std::vector<effectParam>> m_EffectTable;
    int m_Index = 0;


    // void DisconnectComboSig();
    //void ReconnectComboSig();
};

#endif // EFFECTVIEW_H
