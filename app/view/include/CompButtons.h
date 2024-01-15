/*
 * CompButton.h
 *
 * Copyright (C) SkySoft-ATM - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 *
 * Description: Here are different custom button (big primary button,
 * secondary button, color selection button...), extending QPushButton
 */

#ifndef COMPBUTTONS_H
#define COMPBUTTONS_H

#include <QPushButton>


///////////////////////////////////////////////////////////////////////////////
/// CompBigPrimaryButton is our custom QPushButton, has a specifical visual
class CompPrimaryButton : public QPushButton
{
    Q_OBJECT
public:
    using QPushButton::QPushButton;
};

class CompBigPrimaryButton : public CompPrimaryButton
{
    Q_OBJECT
public:
    using CompPrimaryButton::CompPrimaryButton;
};

///////////////////////////////////////////////////////////////////////////////
/// CompSecondaryButton is our custom QPushButton, has a specifical visual
class CompSecondaryButton : public QPushButton
{
    Q_OBJECT
public:
    using QPushButton::QPushButton;
};

///////////////////////////////////////////////////////////////////////////////
/// CompBigSecondaryButton is our custom QPushButton, has a specifical visual
class CompBigSecondaryButton : public CompSecondaryButton
{
    Q_OBJECT
public:
    using CompSecondaryButton::CompSecondaryButton;
};

///////////////////////////////////////////////////////////////////////////////
/// Color button to display color picker box
class CompColorButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CompColorButton(QWidget* parent = nullptr);

    QColor GetColor() const { return m_color; }
    void SetColor(QColor c);

signals:
    void ColorChanged();

protected:
    void mouseReleaseEvent(QMouseEvent* e) override;

private:
    QColor m_color;
};

#endif // COMPBUTTONS_H
