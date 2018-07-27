#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "data_base.h"

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QDate>
#include <QTime>
#include <QTableView>
#include <QString>
#include <QSqlTableModel>
#include <QProcess>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QSqlQueryModel>

#include "noeditcolumn.h"

#include <form.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void MakeSearch(QString, NoEditColumn*);                     // поиск
    void renameColumn(QSqlTableModel*);                          // переименование столбцов таблиц
    void DisplayTab(QSqlTableModel*, QTableView*);               // отображение таблиц
    void startProcess(QString);                                  // запуск процесса
    void MakeSelect(QString, QSqlTableModel*);                   // загрузка картинки выдвинутого ящика по соотв. значению в таблице
    void CallMakeSelect(QString);                                // MakeSelect по всем таблицам
    void setTables(QSqlTableModel*, QString);                    // установка таблиц
    void HideColumn(QVector <int>, QTableView*, QSqlTableModel*);// скрыть столбцы в таблице                           //


protected:
    void resizeEvent(QResizeEvent *);     // подстраивание размера таблиц под размер TableView
    void mousePressEvent(QMouseEvent *);  // загрузка картинки по клику мыша на ящик
    bool eventFilter(QObject *, QEvent *);// фильтрация событий


public slots:
    void updTime();        // отображение реального времени в LineEdit
    void FilterShortage(); // фильтр радиокнопки "Некомплект"
    void FilterAll();      // фильтр радиокнопки "Все"
    void LiveSearch(QString);
    //void

private slots:
    void on_exit_button_clicked();

private:
    Ui::MainWindow *ui;
    Data_base db;               // экземпляр класса Data_base, реализующего функции соединения с базой данных
    QString name_driver;        // идентификатор менеджера базы данных
    QString nameDB;             // имя базы данных
    QTimer *timer;              // таймер для вывода реального времени
    QSqlDatabase DataBase_Users;// присоединение к базе данных
    QSqlQuery Query_DB1;        // запросы
    QGraphicsScene* scene;      // сцена для вывода картинки
    QGraphicsView* view;        // для вывода картинки
    QTime currTime;             // текущее время
    QDate currDate;             // текущая дата
    QString locPic;             // адрес локации картинок шкафа
    QGraphicsPixmapItem*itemPic;// для вывода картинки шкафа
    Form *helpW;                // форма для справки
    QProcess* procUtilPdf;      // процесс утилиты просмотра .pdf-файлов
    QString pocName;            // адрес .exe утилиты
    QString searchStr;          // строка поиска
    struct posYmin {
        int box1;
        int box2;
        int box3;
        int box4;
        int box5;
    };
    posYmin pmin;
    struct posYmax {
        int box1;
        int box2;
        int box3;
        int box4;
        int box5;
    };
    posYmax pmax;

    /* экземпляры QTableView виджетов отображения*/
    QTableView  *tabViewSpare,   // запасные части
                *tabViewZip,     // ЗИП к покупным
                *tabViewFasten,  // крепежные детали
                *tabViewInstrum, // инструмент
                *tabViewAccess,  // принадлежности
                *tabViewMater;   // материалы

    /* Экземпляры NoEditColumn для создания модели*/
    NoEditColumn   *mod_spare,  // запасные части
                   *mod_zip,    // ЗИП к покупным
                   *mod_fasten, // крепежные детали
                   *mod_instrum,// инструмент
                   *mod_access, // принадлежности
                   *mod_mater;  // материалы
};

#endif // MAINWINDOW_H
