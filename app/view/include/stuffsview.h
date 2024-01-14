#ifndef STUFFSVIEW_H
#define STUFFSVIEW_H

#include <QWidget>

#include "stuffpanel.h"

namespace Ui {
class StuffsView;
}

class StuffsView : public QWidget
{
    Q_OBJECT

public:
    explicit StuffsView(QWidget *parent = nullptr);
    ~StuffsView();

    void AddStuff(QString name);

     std::vector<StuffPanel*> m_StuffList;

private:
    Ui::StuffsView *ui;
};

#endif // STUFFSVIEW_H
