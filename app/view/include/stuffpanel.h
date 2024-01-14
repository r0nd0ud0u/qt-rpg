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
    void AddName(QString name);

private:
    Ui::StuffPanel *ui;
};

#endif // STUFFPANEL_H
