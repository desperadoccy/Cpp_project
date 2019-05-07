#ifndef SALESMODE_H
#define SALESMODE_H

#include <QWidget>
#include <QLabel>
#include <QTableWidget>

namespace Ui {
class SalesMode;
}

class SalesMode : public QWidget
{
    Q_OBJECT

public:
    explicit SalesMode(QWidget *parent = nullptr);
    ~SalesMode();
    double *cnt,*taxs,*pays;
    int *i;
    QLabel *salecnt;
    QLabel *saletax;
    QLabel *paycnt;
    QTableWidget *watchui;
private:
    Ui::SalesMode *ui;
};

#endif // SALESMODE_H
