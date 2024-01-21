#ifndef ACTIONSVIEW_H
#define ACTIONSVIEW_H

#include <QWidget>

enum class ActionsStackedWgType{
    attak,
    inventory,
    defaultType,
    enumSize
};


namespace Ui {
class ActionsView;
}

class ActionsView : public QWidget
{
    Q_OBJECT

public:
    explicit ActionsView(QWidget *parent = nullptr);
    ~ActionsView();
    void UpdateView(const ActionsStackedWgType& type);
    void RemoveButtons();
private:
    Ui::ActionsView *ui;

    void UpdateAttak();
};


#endif // ACTIONSVIEW_H
