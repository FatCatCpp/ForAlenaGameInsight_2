#ifndef NOEDITCOLUMN_H
#define NOEDITCOLUMN_H

#include <QObject>
#include <QSqlTableModel>
#include <QDebug>

extern bool _canEdit; // возможность редактировать (по галочке)

class NoEditColumn : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit NoEditColumn(QObject *parent = 0, QSqlDatabase db = QSqlDatabase());
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:

public slots:
    void mySetCheck(bool);
};

#endif // NOEDITCOLUMN_H
