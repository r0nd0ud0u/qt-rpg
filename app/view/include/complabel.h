#ifndef COMPLABEL_H
#define COMPLABEL_H

#include <QLabel>

class CompLabel: public QLabel
{
    Q_OBJECT
public:

    explicit CompLabel(QWidget* parent = nullptr);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
signals:
    void SigCompLabelClicked();
};

#endif // COMPLABEL_H
