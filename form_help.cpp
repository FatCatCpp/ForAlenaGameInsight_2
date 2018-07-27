#include "form_help.h"
#include "ui_form_help.h"
#include <QDialog>
#include <QPushButton>

Form_help::Form_help(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_help)
{
    ui->setupUi(this);

    /*QDialog *subDialog = new QDialog;
    subDialog->setWindowTitle("Sub Dialog");
    QPushButton *button = new QPushButton("Push to open new dialog", this);
    connect(button, SIGNAL(clicked()), subDialog, SLOT(show()));*/
}

Form_help::~Form_help()
{
    delete ui;
}
