/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo Utility\trunk\3Ds Max Exporter\KismoMaxExporterOpts.h
 * Status: duplicate of kismo-utility/branches/11-10-2010/KismoMaxExporterOpts.h
 */
#include "3dsmaxsdk_preinclude.h"
#include "Max.h"
#include "resource.h"

class KismoMaxExporterOpts
{
private:
	// Options
	bool m_bExportMaterials;

public:
	KismoMaxExporterOpts();
	~KismoMaxExporterOpts();

	// This function will show the options dialog. Canceling will cancel the export
	bool ShowDialog();

	bool ExportMaterials() { return m_bExportMaterials; };

private:

	// Our WndProc callback for the options dlg
	static INT_PTR CALLBACK DialogProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};