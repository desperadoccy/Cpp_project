#ifndef MODEBUTTON_H
#define MODEBUTTON_H

#include <QPushButton>
#include <QDebug>

class ModeButton : public QPushButton
{
    Q_OBJECT
public:
    //explicit ModeButton(QWidget *parent = nullptr);
    ModeButton(QString normalImg,QString pressImg = "");
    ~ModeButton();
    QString normalImgPath;
    QString pressImgPath;
    //动态跳跃up
    void Zoom1();
    //动态跳跃down
    void Zoom2();
signals:

public slots:
};

#endif // MODEBUTTON_H
