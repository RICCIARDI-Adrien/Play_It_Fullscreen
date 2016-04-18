/** @file Morrowind_Fullscreen.c
 * Find the Morrowind window and force it to fullscreen mode.
 * @author Adrien RICCIARDI
 */
#define UNICODE
#include <stdio.h>
#include <wchar.h>
#include <windows.h>

//-------------------------------------------------------------------------------------------------
// Private constants
//-------------------------------------------------------------------------------------------------
/** The window to find title. */
#define WINDOW_TITLE L"Morrowind"

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(void)
{
	HWND Handle;
	LONG_PTR Window_Style;
	unsigned int Error_Code;

	// Find the window handle
	wprintf(L"Searching for the window handle...\n");
	while (1)
	{
		Handle = FindWindow(NULL, WINDOW_TITLE);
		if (Handle != NULL) break;
		Sleep(1000); // Avoid using 100% CPU
	}
	wprintf(L"Window handle found.\n");
	
	// Get the current window style
	wprintf(L"Removing the window border...\n");
	Window_Style = GetWindowLongPtr(Handle, GWL_STYLE);
	if (Window_Style == 0)
	{
		wprintf(L"Error : failed to get the window style (error %u).\n", GetLastError());
		return EXIT_FAILURE;
	}
	// Remove the border and the title bar
	Window_Style &= ~(WS_BORDER | WS_CAPTION);
	// Set the new window style
	if (SetWindowLongPtr(Handle, GWL_STYLE, Window_Style) == 0)
	{
		Error_Code = GetLastError();
		wprintf(L"Error : failed to set the window style (error %u).\n", Error_Code);
		if (Error_Code == ERROR_ACCESS_DENIED) wprintf(L"Try to run Cygwin or Morrowind_Fullscreen as administrator.\n");
		return EXIT_FAILURE;
	}
	wprintf(L"The window border was successfully removed.\n");
	
	// Force the window to maximize
	ShowWindow(Handle, SW_MAXIMIZE);
	wprintf(L"The window was successfully maximized.\n");

	return EXIT_SUCCESS;
}
