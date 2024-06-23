#ifndef EDITATKPANEL_H
#define EDITATKPANEL_H

#include <QWidget>

namespace Ui {
class EditAtkPanel;
}

class EditAtkPanel : public QWidget
{
    Q_OBJECT

public:
    explicit EditAtkPanel(QWidget *parent = nullptr);
    ~EditAtkPanel();

private:
    Ui::EditAtkPanel *ui;
};

#endif // EDITATKPANEL_H
