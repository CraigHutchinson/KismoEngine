/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\_ARCHIVE_\Kismo Utility\branches\11-10-2010\KismoMaxExporterOpts.cpp
 * Status: current (newest copy of this file name in the archive)
 */
#include "KismoMaxExporterOpts.h"

//////////////////////////////////////////////////////////////////////////
// ADNDemoExportOpts functions

KismoMaxExporterOpts::KismoMaxExporterOpts()
{
}

KismoMaxExporterOpts::~KismoMaxExporterOpts()
{
	// cleanup
}

bool KismoMaxExporterOpts::ShowDialog()
{
	// Pop a modal dialog.  If we return 0,
	// the user hit cancel
	return 0 != DialogBoxParam(hInstance, 
					MAKEINTRESOURCE(IDD_OPTIONS), 
					GetActiveWindow(), 
					DialogProc, (LPARAM)this); 
}

//////////////////////////////////////////////////////////////////////////
// Dialog Proc

INT_PTR CALLBACK KismoMaxExporterOpts::DialogProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) {
	static KismoMaxExporterOpts *imp = NULL;

	switch(message) {
		case WM_INITDIALOG:
			imp = (KismoMaxExporterOpts *)lParam;
			CenterWindow(hWnd,GetParent(hWnd));

			SetCheckBox(hWnd, IDC_EXP_MATERIALS, imp->ExportMaterials());
			return TRUE;

		case WM_COMMAND:
			switch(wParam)
			{
			case IDC_OK:
				imp->m_bExportMaterials = IsDlgButtonChecked(hWnd, IDC_EXP_MATERIALS) == BST_CHECKED;
				EndDialog(hWnd, IDOK);

			case IDC_CANCEL:
				EndDialog(hWnd, IDCANCEL);
				return 1;
			}
			return TRUE;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return 1;
	}
	return 0;
}