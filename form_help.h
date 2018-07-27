#ifndef FORM_HELP_H
#define FORM_HELP_H

#include <QWidget>

namespace Ui {
class Form_help;
}

class Form_help : public QWidget
{
    Q_OBJECT

public:
    explicit Form_help(QWidget *parent = 0);
    ~Form_help();

private:
    Ui::Form_help *ui;
};

#endif // FORM_HELP_H
