#ifndef EFFECTVIEW_H
#define EFFECTVIEW_H

#include <QWidget>

namespace Ui {
class EffectView;
}

class EffectView : public QWidget
{
    Q_OBJECT

public:
    explicit EffectView(QWidget *parent = nullptr);
    ~EffectView();

private:
    Ui::EffectView *ui;
};

#endif // EFFECTVIEW_H
