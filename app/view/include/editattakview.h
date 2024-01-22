#ifndef EDITATTAKVIEW_H
#define EDITATTAKVIEW_H

#include <QWidget>
#include <QStringListModel>

#include "character.h"

#include <vector>

struct EditAttak{
    AttaqueType type;
    bool updated = false;
};

namespace Ui {
class EditAttakView;
}

class EditAttakView : public QWidget
{
    Q_OBJECT

public:
    explicit EditAttakView(QWidget *parent = nullptr);
    ~EditAttakView();
    void InitView();
    void Save();

private slots:
    void on_apply_button_clicked();

    void on_atk_list_view_clicked(const QModelIndex &index);

    void on_photo_comboBox_currentTextChanged(const QString &arg1);

    void on_name_lineEdit_textChanged(const QString &arg1);

    void on_target_comboBox_currentIndexChanged(int index);

    void on_reach_comboBox_currentIndexChanged(int index);

    void on_duration_spinBox_valueChanged(int arg1);

    void on_rage_aggro_spinBox_valueChanged(int arg1);

    void on_mana_cost_spinBox_valueChanged(int arg1);

    void on_photo_comboBox_currentIndexChanged(int index);

    void on_new_atk_button_clicked();

private:
    Ui::EditAttakView *ui;
    std::vector<EditAttak> m_AttakList;
    QString m_SelectedCharaName;
    const QString OFFLINE_IMG = "./offlines/attak/img/";
    const QString OFFLINE_ATK = "./offlines/attak/";

    void UpdateValues(const EditAttak &selectedAttak);
    void Apply();
    void OnValueChange();
};

#endif // EDITATTAKVIEW_H
