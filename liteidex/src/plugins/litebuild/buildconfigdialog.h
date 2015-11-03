/**************************************************************************
** This file is part of LiteIDE
**
** Copyright (c) 2011-2015 LiteIDE Team. All rights reserved.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** In addition, as a special exception,  that plugins developed for LiteIDE,
** are allowed to remain closed sourced and can be distributed under any license .
** These rights are included in the file LGPL_EXCEPTION.txt in this package.
**
**************************************************************************/
// Module: buildconfigdialog.h
// Creator: visualfc <visualfc@gmail.com>

#ifndef BUILDCONFIGDIALOG_H
#define BUILDCONFIGDIALOG_H

#include <QDialog>
#include <QModelIndex>

namespace Ui {
    class BuildConfigDialog;
}

class QAbstractItemModel;
class QTableView;
class BuildConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BuildConfigDialog(QWidget *parent = 0);
    virtual ~BuildConfigDialog();
    void setBuildRoot(const QString &root);
    void setBuild(const QString &buildId, const QString &buildFile);
    void setModel(QAbstractItemModel * liteide,QAbstractItemModel * config, QAbstractItemModel * custom);
    void setGopath(const QStringList &global, const QStringList &custom, bool inheritGlobal);
    bool isInheritGlobalGopath() const;
    QStringList customGopathList() const;
public slots:
    void editCustomeTabView(QModelIndex);
protected:
    void resizeTableView(QTableView *tableView);
private slots:
    void on_btnClearCustomGopath_clicked();

    void on_btnBrowserCustomGopath_clicked();

    void on_btnDirectGopath_clicked();

private:
    Ui::BuildConfigDialog *ui;
    QString     m_buildRoot;
};

#endif // BUILDCONFIGDIALOG_H
