/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\TestKismo_Socket\GeneratedFiles\ui_dlgkismosockettest.h
 * Status: unique (no other copy of this file name in the archive)
 */
/********************************************************************************
** Form generated from reading ui file 'dlgkismosockettest.ui'
**
** Created: Wed 29. Sep 19:47:44 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_DLGKISMOSOCKETTEST_H
#define UI_DLGKISMOSOCKETTEST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DlgKismoSocketTestClass
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *DlgKismoSocketTestClass)
    {
        if (DlgKismoSocketTestClass->objectName().isEmpty())
            DlgKismoSocketTestClass->setObjectName(QString::fromUtf8("DlgKismoSocketTestClass"));
        DlgKismoSocketTestClass->resize(563, 382);
        centralWidget = new QWidget(DlgKismoSocketTestClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        DlgKismoSocketTestClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(DlgKismoSocketTestClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 563, 22));
        DlgKismoSocketTestClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(DlgKismoSocketTestClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        DlgKismoSocketTestClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(DlgKismoSocketTestClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        DlgKismoSocketTestClass->setStatusBar(statusBar);

        retranslateUi(DlgKismoSocketTestClass);

        QMetaObject::connectSlotsByName(DlgKismoSocketTestClass);
    } // setupUi

    void retranslateUi(QMainWindow *DlgKismoSocketTestClass)
    {
        DlgKismoSocketTestClass->setWindowTitle(QApplication::translate("DlgKismoSocketTestClass", "DlgKismoSocketTest", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(DlgKismoSocketTestClass);
    } // retranslateUi

};

namespace Ui {
    class DlgKismoSocketTestClass: public Ui_DlgKismoSocketTestClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGKISMOSOCKETTEST_H
