#ifndef BOOKMANAGE_H
#define BOOKMANAGE_H

#include <QWidget>

namespace Ui {
class BookManage;
}

class BookManage : public QWidget
{
    Q_OBJECT

public:
    explicit BookManage(QWidget *parent = nullptr);
    ~BookManage();
private:
    Ui::BookManage *ui;
};

#endif // BOOKMANAGE_H
