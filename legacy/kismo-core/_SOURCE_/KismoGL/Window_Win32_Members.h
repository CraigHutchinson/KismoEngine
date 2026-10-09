/*
 * Legacy source: verbatim copy from the Kismo archive, imported 2026-10-09.
 * Origin: \\crogserver\Development\C++\KismoCore\_SOURCE_\KismoGL\Window_Win32_Members.h
 * Status: unique (no other copy of this file name in the archive)
 */

private:
	static LRESULT CALLBACK staticProcessWin32Message(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool processWin32Message( UInt32 msg, UInt32 wParam, UInt32 lParam);