#include "noeditcolumn.h"
#include "mainwindow.h"

bool _canEdit = true;

NoEditColumn::NoEditColumn(QObject *parent, QSqlDatabase db)
    : QSqlTableModel(parent, db)
{

}

Qt::ItemFlags NoEditColumn::flags(const QModelIndex &index) const
{
    Qt::ItemFlags fl = QSqlTableModel::flags(index) & ~(Qt::ItemIsEditable);
    if(_canEdit)
    {
        if (index.column() == 3)
        {
            fl |= Qt::ItemIsEditable;
        }
    }
    return fl;
}

void NoEditColumn::mySetCheck(bool ed)
{
    _canEdit = ed;
}
