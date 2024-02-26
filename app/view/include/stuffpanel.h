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

    QString m_Name;

private:
    Ui::StuffPanel *ui;
};

#endif // STUFFPANEL_H
