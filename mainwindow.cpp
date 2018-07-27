
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updTime()));
    timer->start(0);
    ui->lineEdit_date->setText(currDate.currentDate().toString("dd.MM.yyyy"));
    ui->lineEdit_date->setReadOnly(true);
    ui->lineEdit_time->setReadOnly(true);

    pmin.box1 = 122;
    pmin.box2 = 143;
    pmin.box3 = 163;
    pmin.box4 = 205;
    pmin.box5 = 76;

    pmax.box1 = 142;
    pmax.box2 = 162;
    pmax.box3 = 204;
    pmax.box4 = 246;
    pmax.box5 = 121;


    /*     загрузка картинки шкафа по умолчанию     */
    ui->graphicsView->resize(142+4, 315+4);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    itemPic = new QGraphicsPixmapItem(QPixmap(":boxes/icons/zip_box.png"));
    scene->addItem(itemPic);

    /*     радиокнопки по умолчанию     */
    ui->radioBut_all->setChecked(true);
    ui->radioBut_shortage->setChecked(false);

    name_driver = "QSQLITE";     // драйвер БД
    db.load_driver(name_driver); // загружаем драйвер

    nameDB = "D:\\projects\\new_zip\\DataBaseZip.db"; // имя БД
    db.load_DataBase(nameDB);               // загружаем БД

    /*     Модели-наследники от QSqlTableModel    */
    mod_spare   = new NoEditColumn(this, DataBase_Users); // Запчасти
    mod_zip     = new NoEditColumn(this, DataBase_Users); // ЗИП к покупным
    mod_fasten  = new NoEditColumn(this, DataBase_Users); // крепежные детали
    mod_instrum = new NoEditColumn(this, DataBase_Users); // инструменты
    mod_access  = new NoEditColumn(this, DataBase_Users); // принадлежности
    mod_mater   = new NoEditColumn(this, DataBase_Users); // материалы


    /*     стратегия редактирования - при переходе к другой строке     */
    mod_spare->setEditStrategy(QSqlTableModel::OnFieldChange);
    mod_zip->setEditStrategy(QSqlTableModel::OnFieldChange);
    mod_fasten->setEditStrategy(QSqlTableModel::OnFieldChange);
    mod_instrum->setEditStrategy(QSqlTableModel::OnFieldChange);
    mod_access->setEditStrategy(QSqlTableModel::OnFieldChange);
    mod_mater->setEditStrategy(QSqlTableModel::OnFieldChange);

    /*     вьюшки     */
    tabViewSpare   = new QTableView(ui->tableView);   // Запчасти
    tabViewZip     = new QTableView(ui->tableView_2); // ЗИП к покупным
    tabViewFasten  = new QTableView(ui->tableView_3); // крепежные детали
    tabViewInstrum = new QTableView(ui->tableView_4); // инструменты
    tabViewAccess  = new QTableView(ui->tableView_5); // принадлежности
    tabViewMater   = new QTableView(ui->tableView_6); // материалы

    /*     Установка таблиц баз данных на модели     */
    setTables(mod_spare,   "Spare_parts"); // Запчасти
    setTables(mod_zip,     "Zip_bougth");  // ЗИП к покупным
    setTables(mod_fasten,  "sheet");       // крепежные детали
    setTables(mod_instrum, "Instruments"); // инструменты
    setTables(mod_access,  "Acess");       // принадлежности
    setTables(mod_mater,   "Materials");   // материалы

    /*     Переименование заголовков таблиц на русский язык     */
    renameColumn(mod_spare);
    renameColumn(mod_zip);
    renameColumn(mod_fasten);
    renameColumn(mod_instrum);
    renameColumn(mod_access);
    renameColumn(mod_mater);

    /*     Отображение данных     */
    DisplayTab(mod_spare,   tabViewSpare);
    DisplayTab(mod_zip,     tabViewZip);
    DisplayTab(mod_fasten,  tabViewFasten);
    DisplayTab(mod_instrum, tabViewInstrum);
    DisplayTab(mod_access,  tabViewAccess);
    DisplayTab(mod_mater,   tabViewMater);

    /*     галочка "Разрешить редактирование наличия"     */
    ui->checkBox_red->setCheckable(true);
    ui->checkBox_red->setChecked(true);

    /*     редактирование колонки     */
    NoEditColumn* nec = new NoEditColumn;
    connect(ui->checkBox_red, SIGNAL(toggled(bool)), nec, SLOT(mySetCheck(bool)));
    connect(ui->lineEdit_search, SIGNAL(textEdited(QString)), this, SLOT(LiveSearch(QString)));
    connect(ui->radioBut_shortage, SIGNAL(clicked(bool)), SLOT(FilterShortage()));
    connect(ui->radioBut_all, SIGNAL(clicked(bool)), SLOT(FilterAll()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_exit_button_clicked()
{
    /*     Применение обновлений в базе данных     */
    mod_spare->submitAll();
    mod_zip->submitAll();
    mod_instrum->submitAll();
    mod_fasten->submitAll();
    mod_access->submitAll();
    mod_mater->submitAll();

    exit(0);
}

/*     Подгоняет размеры таблиц под размеры соответствующих виджетов     */
void MainWindow::resizeEvent(QResizeEvent *e)
{
    QMainWindow::resizeEvent(e);

    tabViewSpare->resize(e->size().width(), e->size().height());
    tabViewZip->resize(e->size().width(), e->size().height());
    tabViewFasten->resize(e->size().width(), e->size().height());
    tabViewInstrum->resize(e->size().width(), e->size().height());
    tabViewAccess->resize(e->size().width(), e->size().height());
    tabViewMater->resize(e->size().width(), e->size().height());
}

/*     Загрузка нужной картинки ящика при нажатии левой кнопки      */
void MainWindow::mousePressEvent(QMouseEvent* e)
{
    e->pos();
    QString enterStr;
    QPoint p = ui->graphicsView->mapFromGlobal(QCursor::pos());



    if ((p.y() > 0) && (p.y() < 315) && (p.x() > 0) && (p.x() < 142)) { // если попали на форму
        if (p.x() > 20 && p.x() < 110) {                                // ширина ящиков

            if (p.y() >= pmin.box5 && p.y() <= pmax.box5) {             // ящик 5
                itemPic = new QGraphicsPixmapItem(QPixmap(":boxes/icons/zip_box_5.png"));
                scene->addItem(itemPic);

                enterStr = "ящ5";
                CallMakeSelect(enterStr);
                //ui->lineEdit_search->setText("");
            }
            if (p.y() >= pmin.box1 && p.y() <= pmax.box1) {             // ящик 1
                itemPic = new QGraphicsPixmapItem(QPixmap(":boxes/icons/zip_box_1.png"));
                scene->addItem(itemPic);

                enterStr = "ящ1";
                CallMakeSelect(enterStr);
                //ui->lineEdit_search->setText("");
            }
            if (p.y() > pmin.box2 && p.y() <= pmax.box2) {             // ящик 2
                itemPic = new QGraphicsPixmapItem(QPixmap(":boxes/icons/zip_box_2.png"));
                scene->addItem(itemPic);

                enterStr = "ящ2";
                CallMakeSelect(enterStr);
                //ui->lineEdit_search->setText("");
            }
            if (p.y() >= pmin.box3 && p.y() <= pmax.box3) {            // ящик 3
                itemPic = new QGraphicsPixmapItem(QPixmap(":boxes/icons/zip_box_3.png"));
                scene->addItem(itemPic);

                enterStr = "ящ3";
                CallMakeSelect(enterStr);
                //ui->lineEdit_search->setText("");
            }
            if (p.y() >= pmin.box4 && p.y() <= pmax.box4) {            // ящик 4
                itemPic = new QGraphicsPixmapItem(QPixmap(":boxes/icons/zip_box_4.png"));
                scene->addItem(itemPic);

                enterStr = "ящ4";
                CallMakeSelect(enterStr);
                //ui->lineEdit_search->setText("");
            }
        }
    }
    else {                                                              // если не попали на форму
        itemPic = new QGraphicsPixmapItem(QPixmap(":boxes/icons/zip_box.png"));
        scene->addItem(itemPic);

        mod_access->setFilter("");
        mod_fasten->setFilter("");
        mod_instrum->setFilter("");
        mod_mater->setFilter("");
        mod_spare->setFilter("");
        mod_zip->setFilter("");

        LiveSearch(enterStr);
    }


    /*if(e->button() == Qt::RightButton) {    // обработка сообщения нажатия правой кнопки мыши
        qDebug() << "Right Button pressed!";
    }*/
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)    // фильтр событий
{
    if ((object->objectName() == "tableView") || (object->objectName() == "tableView_2"))
    {
        if( event->type() == QEvent::KeyPress )
            qDebug() << "Press Enter!";
    }
    return QWidget::eventFilter(object, event);
}

/*     Получение и вывод времени машины в эдит     */
void MainWindow::updTime()
{
    ui->lineEdit_time->setText(currTime.currentTime().toString("hh:mm:ss"));
}

/*     Живой поиск      */
void MainWindow::LiveSearch(QString str)
{
    MakeSearch(str, mod_spare);
    MakeSearch(str, mod_zip);
    MakeSearch(str, mod_fasten);
    MakeSearch(str, mod_instrum);
    MakeSearch(str, mod_access);
    MakeSearch(str, mod_mater);

    itemPic = new QGraphicsPixmapItem(QPixmap(":boxes/icons/zip_box.png"));
    scene->addItem(itemPic);
}

/*     Переименование столбцов на русский язык     */
void MainWindow::renameColumn(QSqlTableModel* mod)
{
    mod->setHeaderData(0, Qt::Horizontal, "Обозначение");
    mod->setHeaderData(1, Qt::Horizontal, "Наименование");
    mod->setHeaderData(2, Qt::Horizontal, "Место укладки");
    mod->setHeaderData(3, Qt::Horizontal, "Наличие");
    mod->setHeaderData(4, Qt::Horizontal, "Штатное");
    mod->setHeaderData(5, Qt::Horizontal, "Примечания");
}

/*     Отображение таблиц     */
void MainWindow::DisplayTab(QSqlTableModel* mod, QTableView* tab)
{
    tab->setModel(mod);

    tab->setSelectionMode(QAbstractItemView::SingleSelection);
    tab->setSelectionBehavior(QAbstractItemView::SelectColumns);
    tab->setSelectionMode(QAbstractItemView::NoSelection);

    tab->show();
}

/*     Скрыть столбцы     */
void MainWindow::HideColumn(QVector<int> col, QTableView* tab, QSqlTableModel* mod)
{
    tab->setModel(mod);
    for (int i = 0; i < col.size(); i++) {
        tab->setColumnHidden(col[i], true);
    }
}

void MainWindow::MakeSearch(QString str, NoEditColumn* mod)
{
    QString sstr, firstB, lastB, bus, testS;

    bus = str;

    firstB = bus.remove(1, bus.length()-1);
    firstB = firstB.toUpper();

    lastB = str;
    lastB = lastB.remove(0, 1);
    firstB += lastB;

    testS = str.toUpper();

    if (!str.isEmpty()) {
        sstr = "UPPER(name) LIKE UPPER('%" + str + "%')";
        sstr += " OR UPPER(design) LIKE UPPER('%" + str + "%')";
        sstr += " OR UPPER(app) LIKE UPPER('%" + str + "%')";
        sstr += " OR UPPER(staff) LIKE UPPER('%" + str + "%')";
        sstr += " OR UPPER(place) LIKE UPPER('%" + str + "%')";
        sstr += " OR UPPER(appling) LIKE UPPER('%" + str + "%')";

        sstr += " OR UPPER(name) LIKE UPPER('%" + firstB + "%')";
        sstr += " OR UPPER(design) LIKE UPPER('%" + firstB + "%')";
        sstr += " OR UPPER(app) LIKE UPPER('%" + firstB + "%')";
        sstr += " OR UPPER(staff) LIKE UPPER('%" + firstB + "%')";
        sstr += " OR UPPER(place) LIKE UPPER('%" + firstB + "%')";
        sstr += " OR UPPER(appling) LIKE UPPER('%" + firstB + "%')";

        sstr += " OR UPPER(name) LIKE UPPER('%" + testS + "%')";
        sstr += " OR UPPER(design) LIKE UPPER('%" + testS + "%')";
        sstr += " OR UPPER(app) LIKE UPPER('%" + testS + "%')";
        sstr += " OR UPPER(staff) LIKE UPPER('%" + testS + "%')";
        sstr += " OR UPPER(place) LIKE UPPER('%" + testS + "%')";
        sstr += " OR UPPER(appling) LIKE UPPER('%" + testS + "%')";

        mod->setFilter(sstr);
    }
    else {
        mod->setFilter("");
    }

    mod->select();
}

/*     загрузка картинки выдвинутого ящика по соотв. значению в таблице     */
void MainWindow::MakeSelect(QString str, QSqlTableModel* mod)
{
    QString res_str =  "UPPER(place) LIKE UPPER('%" + str + "%')";
    mod->setFilter(res_str);
}

/*     Запуск pdf-файла со справкой     */
void MainWindow::startProcess(QString name)
{
    QString linux = "/usr/bin/kde/bin/kpdf"; // вписать нужную строку с адресом утилиты-просмотрщика Linux/МСВС
    QString cmd = "cmd /Q /C \"start"; //
    QString screen = "\"";
    QString strName = cmd + " " + name + screen;
    QProcess::startDetached(strName);
}

/*     вызов MakeSelect для всех таблиц    */
void MainWindow::CallMakeSelect(QString str)
{
    MakeSelect(str, mod_spare);
    MakeSelect(str, mod_zip);
    MakeSelect(str, mod_fasten);
    MakeSelect(str, mod_instrum);
    MakeSelect(str, mod_access);
    MakeSelect(str, mod_mater);
}

/*      Установка таблицы и фильтрация по радиокнопкам     */
void MainWindow::setTables(QSqlTableModel* mod, QString nameTab)
{
    mod->setTable(nameTab);
    mod->setEditStrategy(QSqlTableModel::OnManualSubmit);
    mod->select();
}

/*     Фильтрация по радиокнопке "Некомплект"     */
void MainWindow::FilterShortage()
{
    mod_access->setFilter("app < staff");
    mod_fasten->setFilter("app < staff");
    mod_instrum->setFilter("app < staff");
    mod_mater->setFilter("app < staff");
    mod_spare->setFilter("app < staff");
    mod_zip->setFilter("app < staff");

    searchStr = "";
    ui->lineEdit_search->setText("");
}

/*     Сброс фильтра на радиокнопке "Все"     */
void MainWindow::FilterAll()
{
    mod_access->setFilter("");
    mod_fasten->setFilter("");
    mod_instrum->setFilter("");
    mod_mater->setFilter("");
    mod_spare->setFilter("");
    mod_zip->setFilter("");

    searchStr = "";
    ui->lineEdit_search->setText("");
}







