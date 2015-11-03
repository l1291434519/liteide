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
// Module: buildconfigdialog.cpp
// Creator: visualfc <visualfc@gmail.com>

#include "buildconfigdialog.h"
#include "ui_buildconfigdialog.h"

#include <QAbstractItemModel>
#include <QDir>
#include <QFileDialog>
#include <QDebug>
//lite_memory_check_begin
#if defined(WIN32) && defined(_MSC_VER) &&  defined(_DEBUG)
     #define _CRTDBG_MAP_ALLOC
     #include <stdlib.h>
     #include <crtdbg.h>
     #define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
     #define new DEBUG_NEW
#endif
//lite_memory_check_end

BuildConfigDialog::BuildConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BuildConfigDialog)
{
    ui->setupUi(this);
    ui->liteideTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->liteideTableView->resizeColumnsToContents();
    ui->liteideTableView->verticalHeader()->hide();

    ui->configTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->configTableView->resizeColumnsToContents();
    ui->configTableView->verticalHeader()->hide();

    ui->customTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->customTableView->resizeColumnsToContents();
    ui->customTableView->verticalHeader()->hide();

    ui->editGlobalGopath->setEnabled(false);

    connect(ui->customTableView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(editCustomeTabView(QModelIndex)));
}

BuildConfigDialog::~BuildConfigDialog()
{
    delete ui;
}

void BuildConfigDialog::setBuildRoot(const QString &root)
{
    m_buildRoot = root;
}

void BuildConfigDialog::editCustomeTabView(QModelIndex index)
{
    if (!index.isValid()) {
        return;
    }
    if (index.column() == 1) {
        ui->customTableView->edit(index);
    }
}

void BuildConfigDialog::resizeTableView(QTableView *tableView)
{
    QAbstractItemModel *model = tableView->model();
    if (model && model->columnCount() >= 2) {
        tableView->resizeColumnToContents(0);
#if QT_VERSION >= 0x050000
        tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Interactive);
        tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
#else
        tableView->horizontalHeader()->setResizeMode(0,QHeaderView::Interactive);
        tableView->horizontalHeader()->setResizeMode(1,QHeaderView::Stretch);
#endif
    }
}

void BuildConfigDialog::setBuild(const QString &buildId, const QString &buildFile)
{
    ui->buildIdLabel->setText(buildId);
    ui->buildFileLabel->setText(buildFile);
}

void BuildConfigDialog::setModel(QAbstractItemModel * liteide,QAbstractItemModel * config, QAbstractItemModel * custom)
{
    ui->liteideTableView->setModel(liteide);
    ui->configTableView->setModel(config);
    ui->customTableView->setModel(custom);
    resizeTableView(ui->liteideTableView);
    resizeTableView(ui->configTableView);
    resizeTableView(ui->customTableView);
}

void BuildConfigDialog::setGopath(const QStringList &global, const QStringList &custom, bool inheritGlobal)
{
    ui->chkInheritGlobalGopath->setChecked(inheritGlobal);
    ui->editGlobalGopath->clear();
    foreach (QString path, global) {
        ui->editGlobalGopath->appendPlainText(path);
    }
    ui->editCustomGopath->clear();
    foreach (QString path, custom) {
        ui->editCustomGopath->appendPlainText(path);
    }
}

bool BuildConfigDialog::isInheritGlobalGopath() const
{
    return ui->chkInheritGlobalGopath->isChecked();
}

QStringList BuildConfigDialog::customGopathList() const
{
    QString text = ui->editCustomGopath->toPlainText().trimmed();
    return text.split("\n",QString::SkipEmptyParts);
}

void BuildConfigDialog::on_btnClearCustomGopath_clicked()
{
    ui->editCustomGopath->clear();
}

void BuildConfigDialog::on_btnBrowserCustomGopath_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Choose directory to add to GOPATH:"),
                                                    m_buildRoot,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty()) {
        ui->editCustomGopath->appendPlainText(dir);
    }
}

void BuildConfigDialog::on_btnDirectGopath_clicked()
{
    QString path = QDir::fromNativeSeparators(m_buildRoot);
    QStringList pathList = path.split("/",QString::KeepEmptyParts);
    int pos = pathList.lastIndexOf("src");
    // /proj/src c:/src
    if (pos < 1) {
        return;
    }
    int sum = pathList.length()-pos;

    QString gopath;
    for (int i = 0; i < sum; i++) {
        gopath.append("../");
    }
    ui->editCustomGopath->setPlainText(gopath);
}
