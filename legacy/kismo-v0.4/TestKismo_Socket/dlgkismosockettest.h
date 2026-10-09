/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\TestKismo_Socket\dlgkismosockettest.h
 * Status: unique (no other copy of this file name in the archive)
 */
#ifndef DLGKISMOSOCKETTEST_H
#define DLGKISMOSOCKETTEST_H

#include <QtGui/QMainWindow>
#include "ui_dlgkismosockettest.h"

class DlgKismoSocketTest : public QMainWindow
{
	Q_OBJECT

public:
	DlgKismoSocketTest(QWidget *parent = 0, Qt::WFlags flags = 0);
	~DlgKismoSocketTest();

private:
	Ui::DlgKismoSocketTestClass ui;
};

#endif // DLGKISMOSOCKETTEST_H
