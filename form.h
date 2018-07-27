#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QFile>
#include <QTextStream>
#include <QProcess>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();
    void enfiledlg();
    void startProcess(QString); // запуск процесса

private slots:
    void on_pushButton_clicked();

private:
    Ui::Form *ui;

    QGraphicsScene* scene;      // сцена для вывода картинки
    QGraphicsView* view;
    QString locPic;             // адрес локации картинок шкафа
    QGraphicsPixmapItem*itemPic;

    QProcess* procUtilPdf;      // процесс утилиты просмотра .pdf-файлов
    QString pocName;            // адрес .exe утилиты
};

#endif // FORM_H
