#ifndef DOTVIEW_H
#define DOTVIEW_H

#include <QWidget>

namespace Ui {
class DotView;
}

class DotView : public QWidget
{
    Q_OBJECT

public:
    explicit DotView(QWidget *parent = nullptr);
    ~DotView();

    void SetHotDotValues(const int hot, const int dotf);
    void SetBufDebufValues(const int buf, const int debuf);

private:
    Ui::DotView *ui;
};

#endif // DOTVIEW_H
