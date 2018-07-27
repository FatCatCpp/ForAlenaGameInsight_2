#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    ui->textEdit_help->setReadOnly(true);

    ui->graphicsView->resize(501+4, 114+4);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    locPic = "d:\\new_zip\\icons\\HELP.png";
    itemPic = new QGraphicsPixmapItem(QPixmap(locPic));
    scene->addItem(itemPic);

    enfiledlg();
    procUtilPdf = new QProcess();
    pocName = "c:\\windows\\system32\\calc.exe";
    startProcess(pocName);
}

Form::~Form()
{
    delete ui;
}

void Form::on_pushButton_clicked()
{
    close();
}

void Form::enfiledlg()
{
    QString fname = "d:\\new_zip\\code.txt";
    QFile file(fname);

    file.open(QFile::ReadOnly | QFile::Text);

    QTextStream ReadFile(&file);
    ui->textEdit_help->setText(ReadFile.readAll());
}

void Form::startProcess(QString nameUtil)
{
    procUtilPdf->start(nameUtil);
}
