#if _MSC_VER >= 1400
#pragma execution_character_set("utf-8")
#endif

#include "modebutton.h"
#include <QPropertyAnimation>
ModeButton::ModeButton(QString normalImg,QString pressImg)
{
    //设置按钮样式
    this->normalImgPath = normalImg;
    this->pressImgPath = pressImg;

    QPixmap pix;
    bool ret=pix.load( this->normalImgPath );
    if(!ret)
    {
        return;
    }
    //设置按钮大小
    this->setFixedSize(pix.width(),pix.height());
    //设置不规则图片样式
    this->setStyleSheet("QPushButton{border:0px;}");
    //显示图片
    this->setIcon(pix);
    //设置图片大小
    this->setIconSize(QSize(pix.width(),pix.height()));
}

//动态跳跃up
void ModeButton::Zoom1()
{
    QPropertyAnimation *animation = new QPropertyAnimation (this,"geometry");
    //设定时间间隔
    animation->setDuration(200);
    //设置对象起始位置
    animation->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    //设置对象结束位置
    animation->setEndValue(QRect(this->x(),this->y()+3,this->width(),this->height()));
    //设置曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);
    //执行动画
    animation->start();
}

//动态跳跃down
void ModeButton::Zoom2()
{
    QPropertyAnimation *animation = new QPropertyAnimation (this,"geometry");
    //设定时间间隔
    animation->setDuration(200);
    //设置对象起始位置
    animation->setStartValue(QRect(this->x(),this->y()+3,this->width(),this->height()));
    //设置对象结束位置
    animation->setEndValue(QRect(this->x(),this->y(),this->width(),this->height()));
    //设置曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);
    //执行动画
    animation->start();
}



ModeButton::~ModeButton()
{

}
