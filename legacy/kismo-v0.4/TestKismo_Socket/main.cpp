/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo v0.4\TestKismo_Socket\main.cpp
 * Status: superseded by kismo-v0.4/KismoAI/main.cpp (same name, content differs)
 */
#include "dlgkismosockettest.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	DlgKismoSocketTest w;
	w.show();
	return a.exec();
}
