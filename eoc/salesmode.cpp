#if _MSC_VER >= 1400
#pragma execution_character_set("utf-8")
#endif

#include "modebutton.h"
#include "salesmode.h"
#include "ui_salesmode.h"
#include <QTimer>
#include <QMessageBox>
#include <QFrame>
#include <QInputDialog>
#include <QSqlQuery>
#include <QLabel>
#include <QLineEdit>
#include <QFont>
#include <QTableWidget>
#include <QDate>

SalesMode::SalesMode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SalesMode)
{
    ui->setupUi(this);

    //字体设置
    QFont fonttitle;
    fonttitle.setPointSize(16);
    fonttitle.setBold(true);
    QFont fonttext;
    fonttext.setPointSize(12);

    //sql设置
    double taxrate;
    QString taxrate1;
    QString sql;
    QSqlQuery sqlquery;
    sql = "select * from tax";
    sqlquery.exec(sql);
    sqlquery.next();
    taxrate = sqlquery.value(1).toDouble();
    taxrate1 = sqlquery.value(1).toString();

    //购买显示模块
    cnt = new double;
    taxs = new double;
    pays = new double;
    i = new int;
    *cnt=0;
    *taxs=0;
    *pays=0;
    *i=0;
    QDate date1 = QDate::currentDate();
    QLabel *title2 = new QLabel(this);
    QLabel *date = new QLabel(this);
    salecnt = new QLabel(this);
    saletax = new QLabel(this);
    paycnt = new QLabel(this);
    watchui = new QTableWidget(this);
    title2->setText("前台销售模块");
    watchui->setColumnCount(5);
    watchui->setHorizontalHeaderLabels(QStringList() << "数量"<< "ISBN号"<< "书名"<< "单价"<< "金额");
    date->setText(QString("日期：%1年%2月%3日").arg(date1.year()).arg(date1.month()).arg(date1.day()));
    title2->move(385,50);
    title2->setFont(fonttitle);
    date->move(392,100);
    date->setStyleSheet("color:white");
    salecnt->setStyleSheet("color:white");
    saletax->setStyleSheet("color:white");
    paycnt->setStyleSheet("color:white");
    watchui->setFixedSize(520,200);
    watchui->move(215,150);
    salecnt->setFixedWidth(200);
    saletax->setFixedWidth(200);
    paycnt->setFixedWidth(200);
    salecnt->setText(QString("销售合计: RMB %1").arg(*cnt));
    saletax->setText(QString("零售税: RMB %1").arg(*taxs));
    paycnt->setText(QString("应付金额: RMB %1").arg(*pays));

    //分割线
    QFrame *line_h1 = new QFrame(this);
    line_h1->setGeometry(QRect(125,485,735,3));
    line_h1->setFrameShape(QFrame::HLine);
    line_h1->setFrameShadow(QFrame::Sunken);
    line_h1->raise();
    salecnt->move(215,370);
    saletax->move(215,400);
    paycnt->move(215,430);

    //购书ui设置
    QDialog *buybookui = new QDialog(this);
    buybookui->setFixedSize(500,300);
    QLabel *title1 = new QLabel(buybookui);
    QLabel *title11 = new QLabel(buybookui);
    QLabel *bookname1 = new QLabel(buybookui);
    QLabel *bookisbn1 = new QLabel(buybookui);
    QLabel *booknum1 = new QLabel(buybookui);
    QLineEdit *nametext1 = new QLineEdit(buybookui);
    QLineEdit *isbntext1 = new QLineEdit(buybookui);
    QLineEdit *numtext1 = new QLineEdit(buybookui);
    QPushButton *okbutton1 = new QPushButton(buybookui);
    QPushButton *nobutton1 = new QPushButton(buybookui);
    title1->setText("购书模块");
    title1->setFont(fonttitle);
    title11->setText("isbn和图书名称至少填写一个");
    bookname1->setText("书名:");
    bookisbn1->setText("ISBN:");
    booknum1->setText("购买数量:");
    okbutton1->setText("确认");
    nobutton1->setText("取消");
    title1->move(220,20);
    title11->move(180,60);
    bookname1->move(150,100);
    nametext1->move(250,100);
    bookisbn1->move(150,150);
    isbntext1->move(250,150);
    booknum1->move(150,200);
    numtext1->move(250,200);
    okbutton1->move(150,250);
    nobutton1->move(300,250);
    okbutton1->setDefault(false);
    nobutton1->setDefault(true);


    //购书ui功能设置
    connect(nobutton1,&QPushButton::clicked,[=](){
       buybookui->hide();
    });
    connect(okbutton1,&QPushButton::clicked,[=](){
        if(QMessageBox::Yes == QMessageBox::question(this,"确认","确定购买吗?确认后无法取消购买操作。",QMessageBox::Yes | QMessageBox::No,QMessageBox::No))
        {
            QString a2,a3;
            //sql注入过滤
            if(nametext1->text().contains("' ")||nametext1->text().contains("#")||nametext1->text().contains(" -- ")||isbntext1->text().contains("'")||isbntext1->text().contains("#")||isbntext1->text().contains(" -- "))
            {
                QMessageBox::critical(this,"hack attacked","检测到可能的sql注入攻击，请停止！后果自负！");
                return;
            }
            //查询
            if(numtext1->text()=="")
            {
                QMessageBox::critical(this,"错误","请指明购买量！");
            }
            if(nametext1->text()=="")
            {
                if(isbntext1->text()=="")
                {
                    QMessageBox::critical(this,"error","请输入查询内容！");
                    return;
                }
                else{
                    a2 = QString("select * from Book where isbn = '%1'").arg(isbntext1->text());
                }
            }
            else {
                if(nametext1->text()=="")
                {
                    a2 = QString("select * from Book where name = '%1'").arg(nametext1->text());

                }
                else {
                    a2 = QString("select * from Book where name = '%1' and isbn = '%2'").arg(nametext1->text()).arg(isbntext1->text());
                }
            }
            QSqlQuery sqlquery2,sqlquery3;
            sqlquery2.exec(a2);
            if(!sqlquery2.next())
            {
                QMessageBox::information(this,"error","未找到指定图书");
                return;
            }
            else {
                if(sqlquery2.value(6).toInt()<numtext1->text().toInt())
                {
                    QMessageBox::information(buybookui,"不足",QString("库存不足。当前此书库存为%1本").arg(sqlquery2.value(6).toString()));
                }
                else {
                    a3 = QString("update Book set stock = %1,price_1cnt = %3 where id = %2").arg(sqlquery2.value(6).toInt()-numtext1->text().toInt()).arg(sqlquery2.value(0).toInt()).arg((sqlquery2.value(6).toInt()-numtext1->text().toInt())*sqlquery2.value(7).toDouble());
                    sqlquery3.exec(a3);
                    *cnt += numtext1->text().toInt()*sqlquery2.value(8).toDouble();
                    *taxs = taxrate**cnt;
                    *pays = *taxs+*cnt;
                    watchui->setRowCount(*i+1);
                    salecnt->setText(QString("销售合计: RMB %1").arg(*cnt));
                    saletax->setText(QString("零售税: RMB %1").arg(*taxs));
                    paycnt->setText(QString("应付金额: RMB %1").arg(*pays));
                    watchui->setItem(*i,0,new QTableWidgetItem(numtext1->text()));
                    watchui->setItem(*i,1,new QTableWidgetItem(sqlquery2.value(2).toString()));
                    watchui->setItem(*i,2,new QTableWidgetItem(sqlquery2.value(1).toString()));
                    watchui->setItem(*i,3,new QTableWidgetItem(sqlquery2.value(8).toString()));
                    watchui->setItem(*i,4,new QTableWidgetItem(QString("%1").arg(numtext1->text().toInt()*sqlquery2.value(8).toDouble())));
                    (*i)++;
                    QMessageBox::information(buybookui,"成功","购买成功");
                    buybookui->hide();
                }
            }
        }
    });

    //返回键设置
    ModeButton *back_button = new ModeButton(":/image/paymoney.png");
    back_button->setParent(this);
    back_button->move(756,515);
    //按钮功能实现
    connect(back_button,&QPushButton::clicked,[=](){
        back_button->Zoom1();
        back_button->Zoom2();
        //利用延迟显示弹跳
        QMessageBox::information(this,"成功","已成功完成购书要求！");
        QTimer::singleShot(500,this,[=](){
            *taxs=0;
            *pays=0;
            *cnt=0;
            *i=0;
            watchui->clear();
            salecnt->setText(QString("销售合计: RMB %1").arg("0"));
            saletax->setText(QString("零售税: RMB %1").arg("0"));
            paycnt->setText(QString("应付金额: RMB %1").arg("0"));
            this->hide();
        });
    });

    //购书按钮设置
    ModeButton *buyBookbutton = new ModeButton(":/image/buybook.png");
    buyBookbutton->setParent(this);
    buyBookbutton->move(606,515);

    //改变税率按钮设置
    ModeButton *changeTaxbutton = new ModeButton(":/image/changetax.png");
    changeTaxbutton->setParent(this);
    changeTaxbutton->move(456,515);

    //按钮功能实现
    //购书
    connect(buyBookbutton,&QPushButton::clicked,[=](){
        buybookui->setWindowModality(Qt::ApplicationModal);
        buybookui->show();
    });
    //改变税率
    connect(changeTaxbutton,&QPushButton::clicked,[=](){
        bool isok;
        QString tax= QInputDialog::getText(this,"改变税率","请输入税率",QLineEdit::Normal,taxrate1,&isok);
        if(isok)
        {
            if(tax.toDouble()>0)
            {
                QSqlQuery sqlquery1;
                QString a;
                *taxs = *cnt*tax.toDouble();
                *pays = *taxs+*taxs;
                salecnt->setText(QString("销售合计: RMB %1").arg(*cnt));
                saletax->setText(QString("零售税: RMB %1").arg(*taxs));
                paycnt->setText(QString("应付金额: RMB %1").arg(*pays));
                QMessageBox::information(this,"成功","修改税率成功");
                a = QString("update tax set rate = %1 where id = 1").arg(tax.toDouble());
                sqlquery1.exec(a);
            }
            else {
                QMessageBox::information(this,"失败","修改税率失败");
            }
        }
    });
}

SalesMode::~SalesMode()
{
    delete ui;
}
