#if _MSC_VER >= 1400
#pragma execution_character_set("utf-8")
#endif

#include "bookmanage.h"
#include "ui_bookmanage.h"
#include "modebutton.h"
#include <QPushButton>
#include <QTimer>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QFrame>
#include <QLabel>
#include <QFont>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QTableView>
#include <QStandardItem>
#include <QTableWidget>
#include <QSqlRecord>
#include <QDateEdit>

BookManage::BookManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookManage)
{
    ui->setupUi(this);

    //区分功能按钮和显示区域的分割线
    QFrame *line_v1 = new QFrame(this);
    line_v1->setGeometry(QRect(125,60,3,430));
    line_v1->setFrameShape(QFrame::VLine);
    line_v1->setFrameShadow(QFrame::Sunken);
    line_v1->raise();

    //字体设置
    QFont fonttitle;
    fonttitle.setPointSize(16);
    fonttitle.setBold(true);
    QFont fonttext;
    fonttext.setPointSize(12);

    //返回键的ui设置
    ModeButton *back_button = new ModeButton(":/image/backbutton.png");
    back_button->setParent(this);
    back_button->move(756,515);
    //按钮功能实现
    connect(back_button,&QPushButton::clicked,[=](){
        back_button->Zoom1();
        back_button->Zoom2();
        //利用延迟显示弹跳
        QTimer::singleShot(500,this,[=](){
            this->hide();
        });

    });

    //功能ui初始化
    //查找
    //查找框ui
    QWidget *queryui = new QWidget(this);
    queryui->setFixedSize(746,430);
    queryui->move(150,60);
    queryui->hide();
    QLabel *title = new QLabel(queryui);
    title->setText("查询模块（至少填写一个查询信息）");
    QLabel *bookname = new QLabel(queryui);
    bookname->setText("图书名称:");
    QLabel *bookisbn = new QLabel(queryui);
    bookisbn->setText("isbn:");
    QLabel *bookauthor = new QLabel(queryui);
    bookauthor->setText("作者：");
    QLabel *bookpress = new QLabel(queryui);
    bookpress->setText("出版社：");
    QPushButton *btnquery = new QPushButton(queryui);
    btnquery->setText("查询");
    QLineEdit *booktext = new QLineEdit(queryui);
    QLineEdit *isbntext = new QLineEdit(queryui);
    QLineEdit *authortext = new QLineEdit(queryui);
    QLineEdit *presstext = new QLineEdit(queryui);
    title->setFont(fonttitle);
    bookauthor->setStyleSheet("color:white");
    bookpress->setStyleSheet("color:white");
    bookname->setStyleSheet("color:white");
    bookisbn->setStyleSheet("color:white");
    bookname->setFont(fonttext);
    bookisbn->setFont(fonttext);
    bookpress->setFont(fonttext);
    bookauthor->setFont(fonttext);
    booktext->move(350,100);
    isbntext->move(350,280);
    authortext->move(350,160);
    presstext->move(350,220);
    title->move(155,20);
    bookname->move(190,100);
    bookauthor->move(190,160);
    bookpress->move(190,220);
    bookisbn->move(190,280);
    btnquery->move(470,350);
    //返回框ui
    QTableWidget *response = new QTableWidget(this);
    response->setFixedSize(740,400);
    response->move(175,60);
    response->setColumnCount(8);
    response->setHorizontalHeaderLabels(QStringList() << "ISBN"<< "书名"<< "作者"<< "出版社"<< "进书日期"<< "库存量"<< "批发价"<<"零售价");
    response->hide();
    //返回键的ui重置
    ModeButton *back_button2 = new ModeButton(":/image/backbutton.png");
    back_button2->setParent(this);
    back_button2->move(756,515);
    back_button2->hide();
    //按钮功能实现
    connect(back_button2,&QPushButton::clicked,[=](){
        back_button2->Zoom1();
        back_button2->Zoom2();
        //利用延迟显示弹跳
        QTimer::singleShot(500,this,[=](){
            response->hide();
            back_button2->hide();
            back_button->show();
        });

    });
    //查询实现
    connect(btnquery,&QPushButton::clicked,[=](){
        QString a;
        //sql注入过滤
        if(booktext->text().contains("' ")||booktext->text().contains("#")||booktext->text().contains(" -- ")||isbntext->text().contains("'")||isbntext->text().contains("#")||isbntext->text().contains(" -- "))
        {
            QMessageBox::critical(this,"hack attacked","检测到可能的sql注入攻击，请停止！后果自负！");
            return;
        }
        if(presstext->text().contains("' ")||presstext->text().contains("#")||presstext->text().contains(" -- ")||authortext->text().contains("' ")||authortext->text().contains("#")||authortext->text().contains(" -- "))
        {
            QMessageBox::critical(this,"hack attacked","检测到可能的sql注入攻击，请停止！后果自负！");
            return;
        }
        //查询
        if(booktext->text()=="")
        {
            if(authortext->text()=="")
            {
                if(presstext->text()=="")
                {
                    if(isbntext->text()=="")
                    {
                        QMessageBox::critical(this,"error","请输入查询内容！");
                        return;
                    }
                    else {
                        a = QString("select * from Book where isbn like '%%1%'").arg(isbntext->text());
                    }
                }
                else {
                    if(isbntext->text()=="")
                    {
                        a = QString("select * from Book where press like '%%1%'").arg(presstext->text());
                    }
                    else {
                        a = QString("select * from Book where isbn like '%%1%' and press like '%%2%'").arg(isbntext->text()).arg(presstext->text());
                    }
                }
            }
            else{
                if(presstext->text()=="")
                {
                    if(isbntext->text()=="")
                    {
                        a = QString("select * from Book where author like '%%1%'").arg(authortext->text());
                    }
                    else {
                        a = QString("select * from Book where isbn like '%%1%' and author like '%%2%'").arg(isbntext->text()).arg(authortext->text());
                    }
                }
                else {
                    if(isbntext->text()=="")
                    {
                        a = QString("select * from Book where press like '%%1%' and author like '%%2%'").arg(presstext->text()).arg(authortext->text());
                    }
                    else {
                        a = QString("select * from Book where isbn like '%%1%' and press like '%%2%' and author like '%%3%'").arg(isbntext->text()).arg(presstext->text()).arg(authortext->text());
                    }
                }
            }
        }
        else {
            if(authortext->text()=="")
            {
                if(presstext->text()=="")
                {
                    if(isbntext->text()=="")
                    {
                        a = QString("select * from Book where name like '%%1%'").arg(booktext->text());
                    }
                    else {
                        a = QString("select * from Book where name like '%%1%' and isbn like '%%2%'").arg(booktext->text()).arg(isbntext->text());
                    }
                }
                else {
                    if(isbntext->text()=="")
                    {
                        a = QString("select * from Book where name like '%%1%' and press like '%%2%' ").arg(booktext->text()).arg(presstext->text());
                    }
                    else {
                        a = QString("select * from Book where name like '%%1%' and isbn like '%%2%' and press like '%%3%'").arg(booktext->text()).arg(isbntext->text()).arg(presstext->text());
                    }
                }
            }
            else{
                if(presstext->text()=="")
                {
                    if(isbntext->text()=="")
                    {
                        a = QString("select * from Book where name like '%%1%' and author like '%%2%'").arg(booktext->text()).arg(authortext->text());
                    }
                    else {
                        a = QString("select * from Book where name like '%%1%' and isbn like '%%2%' and author like '%%3%'").arg(booktext->text()).arg(isbntext->text()).arg(authortext->text());
                    }
                }
                else {
                    if(isbntext->text()=="")
                    {
                        a = QString("select * from Book where name like '%%1%' and press like '%%2%' and author like '%%3%'").arg(booktext->text()).arg(presstext->text()).arg(authortext->text());
                    }
                    else {
                        a = QString("select * from Book where isbn like '%%1%' and press like '%%2%' and author like '%%3%'").arg(isbntext->text()).arg(presstext->text()).arg(authortext->text());
                    }
                }
            }

        }
        QSqlQuery sqlquery;
        sqlquery.exec(a);
        int i=0;
        if(!sqlquery.next())
        {
            QMessageBox::information(this,"error","未找到指定图书");
            return;
        }
        sqlquery.previous();
        while(sqlquery.next())
        {
            response->setRowCount(i+1);
            response->setItem(i,0,new QTableWidgetItem(sqlquery.value(2).toString()));
            response->setItem(i,1,new QTableWidgetItem(sqlquery.value(1).toString()));
            response->setItem(i,2,new QTableWidgetItem(sqlquery.value(3).toString()));
            response->setItem(i,3,new QTableWidgetItem(sqlquery.value(4).toString()));
            response->setItem(i,4,new QTableWidgetItem(sqlquery.value(5).toString()));
            response->setItem(i,5,new QTableWidgetItem(sqlquery.value(6).toString()));
            response->setItem(i,6,new QTableWidgetItem(sqlquery.value(7).toString()));
            response->setItem(i,7,new QTableWidgetItem(sqlquery.value(8).toString()));
            i++;
        }
        response->show();
        back_button->hide();
        back_button2->show();
    });

    //添加
    //添加框ui
    QWidget *insertui = new QWidget(this);
    insertui->setFixedSize(756,430);
    insertui->move(150,60);
    insertui->hide();
    QLabel *title1 = new QLabel(insertui);
    title1->setText("添加模块");
    QLabel *bookname1 = new QLabel(insertui);
    bookname1->setText("图书名称:");
    QLabel *bookisbn1 = new QLabel(insertui);
    bookisbn1->setText("isbn:");
    QLabel *bookauthor1 = new QLabel(insertui);
    bookauthor1->setText("作者:");
    QLabel *bookpress1 = new QLabel(insertui);
    bookpress1->setText("出版社:");
    QLabel *bookdate1 = new QLabel(insertui);
    bookdate1->setText("入库时间:");
    QLabel *bookstock1 = new QLabel(insertui);
    bookstock1->setText("库存量:");
    QLabel *bookprice11 = new QLabel(insertui);
    bookprice11->setText("批发价:");
    QLabel *bookprice21 = new QLabel(insertui);
    bookprice21->setText("零售价:");
    QPushButton *btninsert = new QPushButton(insertui);
    btninsert->setText("添加");
    QLineEdit *name1text = new QLineEdit(insertui);
    QLineEdit *isbn1text = new QLineEdit(insertui);
    QLineEdit *author1text = new QLineEdit(insertui);
    QLineEdit *press1text = new QLineEdit(insertui);
    QDateEdit *date1text = new QDateEdit(insertui);
    date1text->setCalendarPopup(true);
    QLineEdit *stock1text = new QLineEdit(insertui);
    QLineEdit *price11text = new QLineEdit(insertui);
    QLineEdit *price21text = new QLineEdit(insertui);
    title1->setFont(fonttitle);
    bookname1->setStyleSheet("color:white");
    bookisbn1->setStyleSheet("color:white");
    bookauthor1->setStyleSheet("color:white");
    bookpress1->setStyleSheet("color:white");
    bookdate1->setStyleSheet("color:white");
    bookstock1->setStyleSheet("color:white");
    bookprice11->setStyleSheet("color:white");
    bookprice21->setStyleSheet("color:white");
    bookname1->setFont(fonttext);
    bookisbn1->setFont(fonttext);
    bookauthor1->setFont(fonttext);
    bookpress1->setFont(fonttext);
    bookdate1->setFont(fonttext);
    bookstock1->setFont(fonttext);
    bookprice11->setFont(fonttext);
    bookprice21->setFont(fonttext);
    name1text->move(350,70);
    isbn1text->move(350,100);
    author1text->move(350,130);
    press1text->move(350,160);
    date1text->move(350,190);
    stock1text->move(350,220);
    price11text->move(350,250);
    price21text->move(350,280);
    title1->move(285,0);
    bookname1->move(190,70);
    bookisbn1->move(190,100);
    bookauthor1->move(190,130);
    bookpress1->move(190,160);
    bookdate1->move(190,190);
    bookstock1->move(190,220);
    bookprice11->move(190,250);
    bookprice21->move(190,280);
    btninsert->move(470,350);
    //添加实现
    connect(btninsert,&QPushButton::clicked,[=](){
        QString a;
        //sql注入过滤
        if(name1text->text().contains("' ")||name1text->text().contains("#")||name1text->text().contains(" -- ")||isbn1text->text().contains("'")||isbn1text->text().contains("#")||isbn1text->text().contains(" -- ")||author1text->text().contains("'")||author1text->text().contains("#")||author1text->text().contains(" -- "))
        {
            QMessageBox::critical(this,"hack attacked","检测到可能的sql注入攻击，请停止！后果自负！");
            return;
        }
        if(press1text->text().contains("' ")||press1text->text().contains("#")||press1text->text().contains(" -- ")||date1text->text().contains("' ")||date1text->text().contains("#")||date1text->text().contains(" -- ")||stock1text->text().contains("' ")||stock1text->text().contains("#")||stock1text->text().contains(" -- "))
        {
            QMessageBox::critical(this,"hack attacked","检测到可能的sql注入攻击，请停止！后果自负！");
            return;
        }
        if(price11text->text().contains("' ")||price11text->text().contains("#")||price11text->text().contains(" -- ")||price21text->text().contains("' ")||price21text->text().contains("#")||price21text->text().contains(" -- "))
        {
            QMessageBox::critical(this,"hack attacked","检测到可能的sql注入攻击，请停止！后果自负！");
            return;
        }
        //一些输入判断
        if(name1text->text()==""||isbn1text->text()==""||stock1text->text()==""||price11text->text()==""||price21text->text()=="")
        {
            QMessageBox::critical(this,"错误","书名，ISBN，库存，批发价，销售价请勿为空！");
            return;
        }
        if(price11text->text().toDouble()<0||price21text->text().toDouble()<0||stock1text->text().toInt()<0)
        {
            QMessageBox::critical(this,"错误","价格或库存量不能为负");
            return;
        }
        if(price11text->text().toDouble()>price21text->text().toDouble())
        {
            QMessageBox::critical(this,"错误","批发价不能小于零售价");
            return;
        }
        if(isbn1text->text().toULongLong()==0)
        {
            QMessageBox::critical(this,"错误","isbn填写出错");
            return;
        }
        QString d=date1text->text();
        for(int i=0;i<d.length();i++)
        {
            if(d[i]=="/")
                d.replace(i,1,"-");
        }
        //判断是否已有此图书，即ISBN是否相同
        QString b;
        b = QString("select * from Book where isbn = '%1' or name = '%2'").arg(isbn1text->text()).arg(name1text->text());
        QSqlQuery temp;
        temp.exec(b);
        if(temp.next())
        {
            QMessageBox::critical(this,"错误","已有此图书");
            return;
        }
        //执行sql
        a = QString("insert into Book values(0,'%1','%2','%3','%4','%5',%6,%7,%8,%9)").arg(name1text->text()).arg(isbn1text->text()).arg(author1text->text()).arg(press1text->text()).arg(d).arg(stock1text->text()).arg(price11text->text()).arg(price21text->text()).arg(price11text->text().toDouble()*stock1text->text().toInt());
        QSqlQuery sqlquery;
        if(sqlquery.exec(a))
        {
            QMessageBox::information(this,"成功","添加成功！");
        }
        else{
            QMessageBox::information(this,"失败","添加失败！");
        }

    });


    //修改
    //修改框ui
    QWidget *updateui = new QWidget(this);
    updateui->setFixedSize(756,430);
    updateui->move(150,60);
    updateui->hide();
    QLabel *title3 = new QLabel(updateui);
    title3->setText("修改模块（至少填写一个修改信息） \n     查找你要修改的图书信息");
    QLabel *bookname3 = new QLabel(updateui);
    bookname3->setText("图书名称:");
    QLabel *bookisbn3 = new QLabel(updateui);
    bookisbn3->setText("isbn:");
    QPushButton *btnupdate1 = new QPushButton(updateui);
    btnupdate1->setText("查询");
    QLineEdit *nametext3 = new QLineEdit(updateui);
    QLineEdit *isbntext3 = new QLineEdit(updateui);
    title3->setFont(fonttitle);
    bookname3->setStyleSheet("color:white");
    bookisbn3->setStyleSheet("color:white");
    bookname3->setFont(fonttext);
    bookisbn3->setFont(fonttext);
    nametext3->move(350,170);
    isbntext3->move(350,270);
    title3->move(155,90);
    bookname3->move(190,170);
    bookisbn3->move(190,270);
    btnupdate1->move(470,350);
    //返回框ui
    QWidget *response2 = new QWidget(this);
    response2->setFixedSize(756,430);
    response2->move(150,60);
    response2->hide();
    QLabel *title4 = new QLabel(response2);
    title4->setText("修改模块");
    QLabel *bookname4 = new QLabel(response2);
    bookname4->setText("图书名称:");
    QLabel *bookisbn4 = new QLabel(response2);
    bookisbn4->setText("isbn:");
    QLabel *bookauthor4 = new QLabel(response2);
    bookauthor4->setText("作者:");
    QLabel *bookpress4 = new QLabel(response2);
    bookpress4->setText("出版社:");
    QLabel *bookdate4 = new QLabel(response2);
    bookdate4->setText("入库时间:");
    QLabel *bookstock4 = new QLabel(response2);
    bookstock4->setText("库存量:");
    QLabel *bookprice14 = new QLabel(response2);
    bookprice14->setText("批发价:");
    QLabel *bookprice24 = new QLabel(response2);
    bookprice24->setText("零售价:");
    QPushButton *btnupdate2 = new QPushButton(response2);
    btnupdate2->setText("修改");
    QLineEdit *booktext4 = new QLineEdit(response2);
    QLineEdit *isbntext4 = new QLineEdit(response2);
    QLineEdit *authortext4 = new QLineEdit(response2);
    QLineEdit *presstext4 = new QLineEdit(response2);
    QDateEdit *datetext4 = new QDateEdit(response2);
    datetext4->setCalendarPopup(true);
    QLineEdit *stocktext4 = new QLineEdit(response2);
    QLineEdit *price1text4 = new QLineEdit(response2);
    QLineEdit *price2text4 = new QLineEdit(response2);
    title4->setFont(fonttitle);
    bookname4->setStyleSheet("color:white");
    bookisbn4->setStyleSheet("color:white");
    bookauthor4->setStyleSheet("color:white");
    bookpress4->setStyleSheet("color:white");
    bookdate4->setStyleSheet("color:white");
    bookstock4->setStyleSheet("color:white");
    bookprice14->setStyleSheet("color:white");
    bookprice24->setStyleSheet("color:white");
    bookname4->setFont(fonttext);
    bookisbn4->setFont(fonttext);
    bookauthor4->setFont(fonttext);
    bookpress4->setFont(fonttext);
    bookdate4->setFont(fonttext);
    bookstock4->setFont(fonttext);
    bookprice14->setFont(fonttext);
    bookprice24->setFont(fonttext);
    booktext4->move(350,70);
    isbntext4->move(350,100);
    authortext4->move(350,130);
    presstext4->move(350,160);
    datetext4->move(350,190);
    stocktext4->move(350,220);
    price1text4->move(350,250);
    price2text4->move(350,280);
    title4->move(285,0);
    bookname4->move(190,70);
    bookisbn4->move(190,100);
    bookauthor4->move(190,130);
    bookpress4->move(190,160);
    bookdate4->move(190,190);
    bookstock4->move(190,220);
    bookprice14->move(190,250);
    bookprice24->move(190,280);
    btnupdate2->move(470,350);
    response2->hide();

    //返回键的ui重置
    ModeButton *back_button3 = new ModeButton(":/image/backbutton.png");
    back_button3->setParent(this);
    back_button3->move(756,515);
    back_button3->hide();
    //按钮功能实现
    connect(back_button3,&QPushButton::clicked,[=](){
        back_button3->Zoom1();
        back_button3->Zoom2();
        //利用延迟显示弹跳
        QTimer::singleShot(500,this,[=](){
            response2->hide();
            updateui->show();
            back_button3->hide();
            back_button->show();
        });

    });
    //修改实现
    //第一步查找修改图书
    connect(btnupdate1,&QPushButton::clicked,[=](){
        QString a;
        //sql注入过滤
        if(nametext3->text().contains("' ")||nametext3->text().contains("#")||nametext3->text().contains(" -- ")||isbntext3->text().contains("'")||isbntext3->text().contains("#")||isbntext3->text().contains(" -- "))
        {
            QMessageBox::critical(this,"hack attacked","检测到可能的sql注入攻击，请停止！后果自负！");
            return;
        }
        //查询
        if(nametext3->text()=="")
        {
            if(isbntext3->text()=="")
            {
                QMessageBox::critical(this,"error","请输入查询内容！");
                return;
            }
            else{
                a = QString("select * from Book where isbn = '%1'").arg(isbntext3->text());
            }
        }
        else {
            if(isbntext3->text()=="")
            {
                a = QString("select * from Book where name = '%1'").arg(nametext3->text());

            }
            else {
                a = QString("select * from Book where name = '%1' and isbn = '%2'").arg(nametext3->text()).arg(isbntext3->text());
            }
        }
        QSqlQuery sqlquery;
        sqlquery.exec(a);
        if(!sqlquery.next())
        {
            QMessageBox::information(this,"error","未找到指定图书");
            return;
        }
        else{
            booktext4->setText(sqlquery.value(1).toString());
            isbntext4->setText(sqlquery.value(2).toString());
            authortext4->setText(sqlquery.value(3).toString());
            presstext4->setText(sqlquery.value(4).toString());
            datetext4->setDate(sqlquery.value(5).toDate());
            stocktext4->setText(sqlquery.value(6).toString());
            price1text4->setText(sqlquery.value(7).toString());
            price2text4->setText(sqlquery.value(8).toString());
            updateui->hide();
            response2->show();
            back_button3->show();
        }
    });
    //第二步修改操作
    connect(btnupdate2,&QPushButton::clicked,[=](){
        if(QMessageBox::Yes == QMessageBox::question(this,"确认","确定修改吗?",QMessageBox::Yes | QMessageBox::No,QMessageBox::No))
        {
            QString a;
            //sql注入过滤
            if(booktext4->text().contains("' ")||booktext4->text().contains("#")||booktext4->text().contains(" -- ")||isbntext4->text().contains("'")||isbntext4->text().contains("#")||isbntext4->text().contains(" -- ")||authortext4->text().contains("'")||authortext4->text().contains("#")||authortext4->text().contains(" -- "))
            {
                QMessageBox::critical(this,"hack attacked","检测到可能的sql注入攻击，请停止！后果自负！");
                return;
            }
            if(presstext4->text().contains("' ")||presstext4->text().contains("#")||presstext4->text().contains(" -- ")||datetext4->text().contains("' ")||datetext4->text().contains("#")||datetext4->text().contains(" -- ")||stocktext4->text().contains("' ")||stocktext4->text().contains("#")||stocktext4->text().contains(" -- "))
            {
                QMessageBox::critical(this,"hack attacked","检测到可能的sql注入攻击，请停止！后果自负！");
                return;
            }
            if(price1text4->text().contains("' ")||price1text4->text().contains("#")||price1text4->text().contains(" -- ")||price2text4->text().contains("' ")||price2text4->text().contains("#")||price2text4->text().contains(" -- "))
            {
                QMessageBox::critical(this,"hack attacked","检测到可能的sql注入攻击，请停止！后果自负！");
                return;
            }
            //一些输入判断
            if(booktext4->text()==""||isbntext4->text()==""||stocktext4->text()==""||price1text4->text()==""||price2text4->text()=="")
            {
                QMessageBox::critical(this,"错误","书名，ISBN，库存，批发价，销售价请勿为空！");
                return;
            }
            if(price1text4->text().toDouble()<0||price2text4->text().toDouble()<0||stocktext4->text().toInt()<0)
            {
                QMessageBox::critical(this,"错误","价格或库存量不能为负");
                return;
            }
            if(price1text4->text().toDouble()>price2text4->text().toDouble())
            {
                QMessageBox::critical(this,"错误","批发价不能小于零售价");
                return;
            }
            if(isbntext4->text().toULongLong()==0)
            {
                QMessageBox::critical(this,"错误","isbn填写出错");
                return;
            }
            QString d=datetext4->text();
            for(int i=0;i<d.length();i++)
            {
                if(d[i]=="/")
                    d.replace(i,1,"-");
            }
            //执行sql
            if(nametext3->text()=="")
            {
                if(isbntext3->text()=="")
                {
                    QMessageBox::critical(this,"error","请输入查询内容！");
                    return;
                }
                else{
                    a = QString("update Book set name = '%2',isbn = '%3',author = '%4',press = '%5',date = '%6',stock = '%7',price_1 = '%8',price_2 = '%9',price_1cnt = '%10' where isbn = '%1'").arg(isbntext3->text()).arg(booktext4->text()).arg(isbntext4->text()).arg(authortext4->text()).arg(presstext4->text()).arg(d).arg(stocktext4->text()).arg(price1text4->text()).arg(price2text4->text()).arg(price1text4->text().toDouble()*stocktext4->text().toInt());
                }
            }
            else {
                if(isbntext3->text()=="")
                {
                    a = QString("update Book set name = '%2',isbn = '%3',author = '%4',press = '%5',date = '%6',stock = '%7',price_1 = '%8',price_2 = '%9',price_1cnt = '%10' where name = '%1'").arg(nametext3->text()).arg(booktext4->text()).arg(isbntext4->text()).arg(authortext4->text()).arg(presstext4->text()).arg(d).arg(stocktext4->text()).arg(price1text4->text()).arg(price2text4->text()).arg(price1text4->text().toDouble()*stocktext4->text().toInt());

                }
                else {
                    a = QString("update Book set name = '%3',isbn = '%4',author = '%5',press = '%6',date = '%7',stock = '%8',price_1 = '%9',price_2 = '%10',price_1cnt = %11 where name = '%1' and isbn = '%2'").arg(nametext3->text()).arg(isbntext3->text()).arg(booktext4->text()).arg(isbntext4->text()).arg(authortext4->text()).arg(presstext4->text()).arg(d).arg(stocktext4->text()).arg(price1text4->text()).arg(price2text4->text()).arg(price1text4->text().toDouble()*stocktext4->text().toInt());
                }
            }
            QSqlQuery sqlquery;
            if(sqlquery.exec(a))
            {
                QMessageBox::information(this,"成功","修改成功！");
            }
            else{
                QMessageBox::information(this,"失败","修改失败！");
            }
        }
    });

    //删除
    //删除框ui
    QWidget *deleteui = new QWidget(this);
    deleteui->setFixedSize(756,430);
    deleteui->move(150,60);
    deleteui->hide();
    QLabel *title2 = new QLabel(deleteui);
    title2->setText("删除模块（至少填写一个删除信息）");
    QLabel *bookname2 = new QLabel(deleteui);
    bookname2->setText("图书名称:");
    QLabel *bookisbn2 = new QLabel(deleteui);
    bookisbn2->setText("isbn:");
    QPushButton *btndelete = new QPushButton(deleteui);
    btndelete->setText("删除");
    QLineEdit *nametext2 = new QLineEdit(deleteui);
    QLineEdit *isbntext2 = new QLineEdit(deleteui);
    title2->setFont(fonttitle);
    bookname2->setStyleSheet("color:white");
    bookisbn2->setStyleSheet("color:white");
    bookname2->setFont(fonttext);
    bookisbn2->setFont(fonttext);
    nametext2->move(350,170);
    isbntext2->move(350,270);
    title2->move(155,90);
    bookname2->move(190,170);
    bookisbn2->move(190,270);
    btndelete->move(470,350);
    //删除实现
    connect(btndelete,&QPushButton::clicked,[=](){
        if(QMessageBox::Yes == QMessageBox::question(this,"确认","确定删除吗?",QMessageBox::Yes | QMessageBox::No,QMessageBox::No))
        {
            QString a;
            //sql注入过滤
            if(nametext2->text().contains("' ")||nametext2->text().contains("#")||nametext2->text().contains(" -- ")||isbntext2->text().contains("'")||isbntext2->text().contains("#")||isbntext2->text().contains(" -- "))
            {
                QMessageBox::critical(this,"hack attacked","检测到可能的sql注入攻击，请停止！后果自负！");
                return;
            }
            //删除
            if(nametext2->text()=="")
            {
                if(isbntext2->text()=="")
                {
                    QMessageBox::critical(this,"error","请输入删除内容！");
                    return;
                }
                else{
                    a = QString("delete from Book where isbn = '%1'").arg(isbntext2->text());
                }
            }
            else {
                if(isbntext2->text()=="")
                {
                    a = QString("delete from Book where name = '%1'").arg(nametext2->text());

                }
                else {
                    a = QString("delete from Book where name = '%1' and isbn = '%2'").arg(nametext2->text()).arg(isbntext2->text());
                }
            }
            QSqlQuery sqlquery;
            if(sqlquery.exec(a))
            {
                QMessageBox::information(this,"成功","删除完成");
            }
            else{
                QMessageBox::information(this,"失败","无此图书");
            }
        }
    });


    //四个功能按钮ui设置
    //查找
    ModeButton *search_button = new ModeButton(":/image/querybutton.png");
    search_button->setParent(this);
    search_button->move(0,85);
    //按钮功能实现
    connect(search_button,&QPushButton::clicked,[=](){
        search_button->Zoom1();
        search_button->Zoom2();
        queryui->show();
        insertui->hide();
        updateui->hide();
        deleteui->hide();
        response->hide();
        response2->hide();
        back_button2->hide();
        back_button->show();
        back_button3->hide();
    });
    //添加
    ModeButton *insert_button = new ModeButton(":/image/insertbutton.png");
    insert_button->setParent(this);
    insert_button->move(0,195);
    //按钮功能实现
    connect(insert_button,&QPushButton::clicked,[=](){
        insert_button->Zoom1();
        insert_button->Zoom2();
        queryui->hide();
        insertui->show();
        updateui->hide();
        deleteui->hide();
        response->hide();
        response2->hide();
        back_button2->hide();
        back_button->show();
        back_button3->hide();
    });
    //修改
    ModeButton *update_button = new ModeButton(":/image/updatebutton.png");
    update_button->setParent(this);
    update_button->move(0,305);
    //按钮功能实现
    connect(update_button,&QPushButton::clicked,[=](){
        update_button->Zoom1();
        update_button->Zoom2();
        queryui->hide();
        insertui->hide();
        updateui->show();
        deleteui->hide();
        response->hide();
        response2->hide();
        back_button2->hide();
        back_button->show();
        back_button3->hide();
    });
    //删除
    ModeButton *delete_button = new ModeButton(":/image/deletebutton.png");
    delete_button->setParent(this);
    delete_button->move(0,415);
    //按钮功能实现
    connect(delete_button,&QPushButton::clicked,[=](){
        delete_button->Zoom1();
        delete_button->Zoom2();
        queryui->hide();
        insertui->hide();
        updateui->hide();
        deleteui->show();
        response->hide();
        response2->hide();
        back_button2->hide();
        back_button->show();
        back_button3->hide();
    });



}

BookManage::~BookManage()
{
    delete ui;
}
