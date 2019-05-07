#ifndef REPORTFORM_H
#define REPORTFORM_H

#include <QWidget>

namespace Ui {
class ReportForm;
}

class ReportForm : public QWidget
{
    Q_OBJECT

public:
    explicit ReportForm(QWidget *parent = nullptr);
    ~ReportForm();

private:
    Ui::ReportForm *ui;
};

#endif // REPORTFORM_H
