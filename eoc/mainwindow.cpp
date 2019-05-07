#if _MSC_VER >= 1400
#pragma execution_character_set("utf-8")
#endif

#include "mainwindow.h"
#include "modebutton.h"
#include "ui_mainwindow.h"
#include <QToolButton>
#include <QPixmap>
#include <QMessageBox>
#include <QPainter>
#include <QTextCodec>
#include <QtCore>
#include <QSound>
#include <QSqlDatabase>
#include <QSqlQuery>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    //设置主窗口大小并固定
    this->setFixedSize(1024,768);
    //设置标题
    this->setWindowTitle("图书管理系统");
    //设置应用图片
    this->setWindowIcon(QIcon(":/image/icon.png"));

    //链接数据库
    //使用sql odbc插件
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    //远程连接
    db.setHostName("106.15.177.94");
    db.setPort(3306);
    db.setDatabaseName("desperado");
    db.setUserName("desperado");
    db.setPassword("ccy13003563808");

    if(db.open()==false){
        QMessageBox::critical(this,"warning","it cannot open datebase \nPlease check the network connection");
    }


    //区分模块按钮和主界面的分割线
    QFrame *line_1 = new QFrame(this);
    line_1->setGeometry(QRect(40,123,944,3));
    line_1->setFrameShape(QFrame::HLine);
    line_1->setFrameShadow(QFrame::Sunken);
    line_1->raise();

    //菜单栏
    //about菜单设置
    //Author设置
    connect(ui->actionauthor,&QAction::triggered,[=](){
        QString message="作者：陈楚燚 \nid：desperado \ngithub:github.com/desperadoccy \n ";
        QMessageBox::information(this,"Author",message);
    });
    //Special_Thanks设置
    connect(ui->action_Special_Thanks,&QAction::triggered,[=](){
        QString message="感谢郭俊杰（南大，id：Akeal）对本软件的ui设计 \n感谢刘志赞（南航，id：Kitty）对本软件服务器的提供 \n";
        QMessageBox::information(this,"Special_Thanks",message);
    });
    //Reference_material设置
    connect(ui->actionReference_material,&QAction::triggered,[=](){
        QString message="参考资料：黑马qt视频教程 \n网址：https://www.bilibili.com/video/av41374178";
        QMessageBox::information(this,"Special_Thanks",message);
    });
    //help菜单设置
    //帮助设置
    connect(ui->actionhow,&QAction::triggered,[=](){
       QMessageBox::information(this,"help","具体操作请查看帮助文档");
    });





    //销售模块ui
    saleMode = new SalesMode(this);
    saleMode->setFixedSize(900,575);
    saleMode->move(65,115);
    //默认隐藏ui界面
    saleMode->hide();
    //销售模块按钮
    ModeButton *button_1 = new ModeButton(":/image/button1.png");
    button_1->setParent(this);
    button_1->move(185,38);
    connect(button_1,&ModeButton::clicked,[=](){
        //按钮弹跳效果
        button_1->Zoom1();
        button_1->Zoom2();
        //显示销售模块ui,隐藏其他ui
        saleMode->show();
        bookManage->hide();
        reportForm->hide();
    });


    //图书管理ui
    bookManage = new BookManage(this);
    bookManage->setFixedSize(900,575);
    bookManage->move(65,115);
    //默认隐藏ui界面
    bookManage->hide();
    //图书管理按钮
    ModeButton *button_2 = new ModeButton(":/image/button2.png");
    button_2->setParent(this);
    button_2->move(435,38);
    connect(button_2,&ModeButton::clicked,[=](){
        //按钮弹跳效果
        button_2->Zoom1();
        button_2->Zoom2();
        //显示图书管理ui,隐藏其他ui
        bookManage->show();
        saleMode->hide();
        reportForm->hide();
        double *a,*b,*c;
        int *d;
        a=saleMode->taxs;
        b=saleMode->pays;
        c=saleMode->cnt;
        d=saleMode->i;
        *a=0;
        *b=0;
        *c=0;
        *d=0;
        saleMode->watchui->clear();
        saleMode->watchui->setRowCount(0);
        saleMode->salecnt->setText(QString("销售合计: RMB %1").arg("0"));
        saleMode->saletax->setText(QString("零售税: RMB %1").arg("0"));
        saleMode->paycnt->setText(QString("应付金额: RMB %1").arg("0"));
    });


    //报表模块ui
    reportForm = new ReportForm(this);
    reportForm->setFixedSize(900,575);
    reportForm->move(65,115);
    //默认隐藏ui界面
    reportForm->hide();
    //报表模块按钮
    ModeButton *button_3 = new ModeButton(":/image/button3.png");
    button_3->setParent(this);
    button_3->move(685,38);
    connect(button_3,&ModeButton::clicked,[=](){
        //按钮弹跳效果
        button_3->Zoom1();
        button_3->Zoom2();
        //显示报表模块ui,隐藏其他ui
        reportForm->show();
        bookManage->hide();
        saleMode->hide();
        double *a,*b,*c;
        int *d;
        a=saleMode->taxs;
        b=saleMode->pays;
        c=saleMode->cnt;
        d=saleMode->i;
        *a=0;
        *b=0;
        *c=0;
        *d=0;
        saleMode->watchui->clear();
        saleMode->watchui->setRowCount(0);
        saleMode->salecnt->setText(QString("销售合计: RMB %1").arg("0"));
        saleMode->saletax->setText(QString("零售税: RMB %1").arg("0"));
        saleMode->paycnt->setText(QString("应付金额: RMB %1").arg("0"));
    });

    //时间显示的实现
    //启用计时器 1s更新1次
    QTimer *timer = new QTimer(this);
    timer->start(1000);
    connect(timer,&QTimer::timeout,[=](){
        QDateTime time = QDateTime::currentDateTime();
        QString timeStr = time.toString("yyyy-MM-dd hh:mm:ss");
        ui->time_label->setText(timeStr);
    });


//    缓冲区无法存放大型音频，舍去
//    //循环播放
//    QSound *backgourndSound = new QSound(":/music/background.mp3",this);
//    backgourndSound->setLoops(QSound::Infinite);
//    //音乐播放按钮的实现
//    backgourndSound->play();
//    connect(ui->checkBox,&QCheckBox::click,[=](){
//        if( ui->checkBox->isChecked()==true)
//            backgourndSound->play();
//        else {
//            backgourndSound->stop();
//        }
//    });



    //退出按钮功能实现
    connect(ui->endButton,&QToolButton::clicked,[=](){
        //弹出对话框判断确定是否退出
        if(QMessageBox::Yes == QMessageBox::question(this,"确认","确定退出吗?",QMessageBox::Yes | QMessageBox::No,QMessageBox::No))
        {
            //db.removeDatabase("desperado");
            this->close();
        }
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    //背景
    QPainter painter(this);
    QPixmap pix(":/image/bgd.jpg");
    painter.drawPixmap(0,0,1024,768,pix);
    //logo显示
    QPainter painter_logo(this);
    painter_logo.drawPixmap(20,708,QPixmap(":/image/logo.png"));

}
