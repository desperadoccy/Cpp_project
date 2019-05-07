#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "bookmanage.h"
#include "reportform.h"
#include "salesmode.h"
#include <QMainWindow>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //背景绘制事件
    void paintEvent(QPaintEvent *);
    //销售模块ui
    SalesMode *saleMode;
    //图书管理模块ui
    BookManage *bookManage;
    //报表模块ui
    ReportForm *reportForm;
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
