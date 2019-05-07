#if _MSC_VER >= 1400
#pragma execution_character_set("utf-8")
#endif

#include "modebutton.h"
#include "reportform.h"
#include "ui_reportform.h"
#include <QFont>
#include <QFrame>
#include <QLabel>
#include <QMessageBox>
#include <QSqlQuery>
#include <QTimer>
#include <QTableWidget>

ReportForm::ReportForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReportForm)
{
    ui->setupUi(this);

    //字体设置
    QFont fonttitle;
    fonttitle.setPointSize(16);
    fonttitle.setBold(true);
    QFont fonttext;
    fonttext.setPointSize(12);

    //区分功能按钮和显示区域的分割线
    QFrame *line_v1 = new QFrame(this);
    line_v1->setGeometry(QRect(150,60,3,430));
    line_v1->setFrameShape(QFrame::VLine);
    line_v1->setFrameShadow(QFrame::Sunken);

    //提示栏
    QLabel *tip = new QLabel(this);
    tip->setText("按如下一种方式列表");
    tip->setStyleSheet("color:white");
    tip->move(5,60);

    //返回列表ui设置
    QTableWidget *tableui = new QTableWidget(this);
    tableui->setFixedSize(650,330);
    tableui->move(230,110);
    tableui->hide();

    //按钮设置
    ModeButton *btn1 = new ModeButton(":/image/all.png");
    ModeButton *btn2 = new ModeButton(":/image/price1.png");
    ModeButton *btn3 = new ModeButton(":/image/price2.png");
    ModeButton *btn4 = new ModeButton(":/image/num.png");
    ModeButton *btn5 = new ModeButton(":/image/pricecnt.png");
    ModeButton *btn6 = new ModeButton(":/image/data.png");
    btn1->setParent(this);
    btn2->setParent(this);
    btn3->setParent(this);
    btn4->setParent(this);
    btn5->setParent(this);
    btn6->setParent(this);
    btn1->move(0,80);
    btn2->move(0,150);
    btn3->move(0,220);
    btn4->move(0,290);
    btn5->move(0,360);
    btn6->move(0,430);
    //功能实现
    connect(btn1,&QPushButton::clicked,[=](){
        btn1->Zoom1();
        btn1->Zoom2();
        tableui->setColumnCount(8);
        tableui->setHorizontalHeaderLabels(QStringList() << "ISBN"<< "书名"<< "作者"<< "出版社"<< "进书日期"<< "库存量"<< "批发价"<<"零售价");
        QSqlQuery sqlquery;
        QString sql;
        sql = "select * from Book";
        sqlquery.exec(sql);
        int i=0;
        while(sqlquery.next())
        {
           tableui->setRowCount(i+1);
           tableui->setItem(i,0,new QTableWidgetItem(sqlquery.value(2).toString()));
           tableui->setItem(i,1,new QTableWidgetItem(sqlquery.value(1).toString()));
           tableui->setItem(i,2,new QTableWidgetItem(sqlquery.value(3).toString()));
           tableui->setItem(i,3,new QTableWidgetItem(sqlquery.value(4).toString()));
           tableui->setItem(i,4,new QTableWidgetItem(sqlquery.value(5).toString()));
           tableui->setItem(i,5,new QTableWidgetItem(sqlquery.value(6).toString()));
           tableui->setItem(i,6,new QTableWidgetItem(sqlquery.value(7).toString()));
           tableui->setItem(i,7,new QTableWidgetItem(sqlquery.value(8).toString()));
           i++;
        }
        tableui->show();
    });
    connect(btn2,&QPushButton::clicked,[=](){
        btn2->Zoom1();
        btn2->Zoom2();
        tableui->setColumnCount(4);
        tableui->setHorizontalHeaderLabels(QStringList() << "书名"<< "库存量"<< "批发价"<<"批发价总额");
        QSqlQuery sqlquery;
        QString sql;
        sql = "select * from Book";
        sqlquery.exec(sql);
        int i=0;
        while(sqlquery.next())
        {
            tableui->setRowCount(i+1);
            tableui->setItem(i,0,new QTableWidgetItem(sqlquery.value(1).toString()));
            tableui->setItem(i,1,new QTableWidgetItem(sqlquery.value(6).toString()));
            tableui->setItem(i,2,new QTableWidgetItem(sqlquery.value(7).toString()));
            tableui->setItem(i,3,new QTableWidgetItem(QString("%1").arg(sqlquery.value(6).toInt()*sqlquery.value(7).toDouble())));
            i++;
        }
        tableui->show();
    });
    connect(btn3,&QPushButton::clicked,[=](){
        btn3->Zoom1();
        btn3->Zoom2();
        tableui->setColumnCount(4);
        tableui->setHorizontalHeaderLabels(QStringList() << "书名"<< "库存量"<< "零售价"<<"零售价总额");
        QSqlQuery sqlquery;
        QString sql;
        sql = "select * from Book";
        sqlquery.exec(sql);
        int i=0;
        while(sqlquery.next())
        {
            tableui->setRowCount(i+1);
            tableui->setItem(i,0,new QTableWidgetItem(sqlquery.value(1).toString()));
            tableui->setItem(i,1,new QTableWidgetItem(sqlquery.value(6).toString()));
            tableui->setItem(i,2,new QTableWidgetItem(sqlquery.value(8).toString()));
            tableui->setItem(i,3,new QTableWidgetItem(QString("%1").arg(sqlquery.value(6).toInt()*sqlquery.value(8).toDouble())));
            i++;
        }
        tableui->show();
    });
    connect(btn4,&QPushButton::clicked,[=](){
        btn4->Zoom1();
        btn4->Zoom2();
        tableui->setColumnCount(3);
        tableui->setHorizontalHeaderLabels(QStringList() << "书名"<< "ISBN"<< "库存量");
        QSqlQuery sqlquery;
        QString sql;
        sql = "select * from Book order by stock desc";
        sqlquery.exec(sql);
        int i=0;
        while(sqlquery.next())
        {
            tableui->setRowCount(i+1);
            tableui->setItem(i,0,new QTableWidgetItem(sqlquery.value(1).toString()));
            tableui->setItem(i,1,new QTableWidgetItem(sqlquery.value(2).toString()));
            tableui->setItem(i,2,new QTableWidgetItem(sqlquery.value(6).toString()));
            i++;
        }
        tableui->show();
    });
    connect(btn5,&QPushButton::clicked,[=](){
        btn5->Zoom1();
        btn5->Zoom2();
        tableui->setColumnCount(4);
        tableui->setHorizontalHeaderLabels(QStringList() << "书名"<< "库存量"<< "批发价"<<"批发价总额");
        QSqlQuery sqlquery;
        QString sql;
        sql = "select * from Book order by price_1cnt desc";
        sqlquery.exec(sql);
        int i=0;
        while(sqlquery.next())
        {
            tableui->setRowCount(i+1);
            tableui->setItem(i,0,new QTableWidgetItem(sqlquery.value(1).toString()));
            tableui->setItem(i,1,new QTableWidgetItem(sqlquery.value(6).toString()));
            tableui->setItem(i,2,new QTableWidgetItem(sqlquery.value(7).toString()));
            tableui->setItem(i,3,new QTableWidgetItem(sqlquery.value(9).toString()));
            i++;
        }
        tableui->show();
    });
    connect(btn6,&QPushButton::clicked,[=](){
        btn6->Zoom1();
        btn6->Zoom2();
        tableui->setColumnCount(3);
        tableui->setHorizontalHeaderLabels(QStringList() << "书名"<< "ISBN"<< "入库日期");
        QSqlQuery sqlquery;
        QString sql;
        sql = "select * from Book order by date";
        sqlquery.exec(sql);
        int i=0;
        while(sqlquery.next())
        {
            tableui->setRowCount(i+1);
            tableui->setItem(i,0,new QTableWidgetItem(sqlquery.value(1).toString()));
            tableui->setItem(i,1,new QTableWidgetItem(sqlquery.value(2).toString()));
            tableui->setItem(i,2,new QTableWidgetItem(sqlquery.value(5).toString()));
            i++;
        }
        tableui->show();
    });

    //返回键设置
    ModeButton *back_button = new ModeButton(":/image/backbutton.png");
    back_button->setParent(this);
    back_button->move(756,515);
    //按钮功能实现
    connect(back_button,&QPushButton::clicked,[=](){
        back_button->Zoom1();
        back_button->Zoom2();
        //利用延迟显示弹跳
        QMessageBox::information(this,"成功","已成功完成购书要求！");
        QTimer::singleShot(500,this,[=](){
            this->hide();
        });
    });
}

ReportForm::~ReportForm()
{
    delete ui;
}
