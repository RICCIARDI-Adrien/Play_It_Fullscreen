/** @file Main.c
 * Allow to play some old games in fullscreen mode on Windows 10.
 * @author Adrien RICCIARDI
 */
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

//-------------------------------------------------------------------------------------------------
// Private constants and macros
//-------------------------------------------------------------------------------------------------
/** Tell how many games are handled by the program. */
#define GAMES_COUNT (sizeof(Games) / sizeof(TGame))

/** Convert the provided ASCII string to a widechar string (concatenate the widechar attribute to the previously expanded macro value).
 * @param String The ASCII string to convert.
 * @return The string converted to widechar.
 * @warning This macro must not be called directly, use CONVERT_ASCII_STRING_TO_WIDECHAR_STRING() instead.
 */
#define CONVERT_ASCII_STRING_TO_WIDECHAR_STRING_SECOND_PREPROCESSOR_EXPANSION(String) L##String
/** Convert the provided ASCII string to a widechar string.
 * @param String The ASCII string to convert.
 * @return The string converted to widechar.
 */
#define CONVERT_ASCII_STRING_TO_WIDECHAR_STRING(String) CONVERT_ASCII_STRING_TO_WIDECHAR_STRING_SECOND_PREPROCESSOR_EXPANSION(String)

//-------------------------------------------------------------------------------------------------
// Private types
//-------------------------------------------------------------------------------------------------
/** A game main window identification and all possible "operations" (i.e. what's needed to make the game works fullscreen). */
typedef struct
{
	LPCTSTR String_Window_Title; //! The game main window title.
	LPCTSTR String_Window_Class; //! The game main window class (used to differenciate the game window from a directory using the same name), set to NULL to match only the window title.
	int Is_Window_Border_Removed; //! Set to '1' to remove the game window border.
	int Is_Window_Maximized; //! Set to '1' to force the game window to be maximized.
} TGame;

//-------------------------------------------------------------------------------------------------
// Private variables
//-------------------------------------------------------------------------------------------------
/** All handled games. */
static TGame Games[] =
{
	// Morrowind
	{
		L"Morrowind",
		L"Morrowind",
		1,
		1
	},
	// Tomb Raider IV - The Last Revelation
	{
		L"Tomb Raider - The Last Revelation",
		L"MainGameWindow",
		1,
		0
	}
};

//-------------------------------------------------------------------------------------------------
// Private functions
//-------------------------------------------------------------------------------------------------
/** Wait for a game to be launched.
 * @param Pointer_Handle On output, contain the found game main window handle.
 * @param Pointer_Game_Index On output, contain the index in the Games array of the found game.
*/
static void WaitForGame(HWND *Pointer_Handle, unsigned int *Pointer_Game_Index)
{
	HWND Handle;
	unsigned int i;

	while (1)
	{
		// Search for all available games
		for (i = 0; i < GAMES_COUNT; i++)
		{
			Handle = FindWindow(Games[i].String_Window_Class, Games[i].String_Window_Title);
			if (Handle != NULL) // A game was found
			{
				// Display the game main window class name only in debug mode
				#ifdef _DEBUG
				{
					TCHAR String_Class_Name[1024];

					if (GetClassName(Handle, String_Class_Name, sizeof(String_Class_Name) / sizeof(TCHAR)) == 0) wprintf(L"Failed to retrieve the window class name.\n");
					else wprintf(L"Window class : %s.\n", String_Class_Name);
				}
				#endif
				*Pointer_Handle = Handle;
				*Pointer_Game_Index = i;
				return;
			}
		}
	
		// Avoid using 100% CPU
		Sleep(1000);
	}
}

//-------------------------------------------------------------------------------------------------
// Entry point
//-------------------------------------------------------------------------------------------------
int main(void)
{
	HWND Handle;
	unsigned int Game_Index, Error_Code;
	LONG_PTR Window_Style;

	// Display the banner
	wprintf(L"+------------------------------------------+\n");
	wprintf(L"| Play It Fullscreen, (C) Adrien RICCIARDI |\n");
	wprintf(L"| Build date : " CONVERT_ASCII_STRING_TO_WIDECHAR_STRING(__DATE__) L", " CONVERT_ASCII_STRING_TO_WIDECHAR_STRING(__TIME__) L"       |\n");
	wprintf(L"+------------------------------------------+\n");

	// Wait for a game to be launched
	wprintf(L"Waiting for a game to be launched...\n");
	WaitForGame(&Handle, &Game_Index);
	wprintf(L"Game found : %s.\n", Games[Game_Index].String_Window_Title); // TODO : use a new field in the struct for the game real name in case of some game not having a presentable window title
	
	// Remove the window border if required to
	if (Games[Game_Index].Is_Window_Border_Removed)
	{
		wprintf(L"Removing the window border...\n");

		// Get the current window style
		Window_Style = GetWindowLongPtr(Handle, GWL_STYLE);
		if (Window_Style == 0)
		{
			wprintf(L"Error : failed to get the window style (error %u).\n", GetLastError());
			goto Exit_Error;
		}

		// Remove the border and the title bar
		Window_Style &= ~(WS_BORDER | WS_CAPTION);

		// Set the new window style
		if (SetWindowLongPtr(Handle, GWL_STYLE, Window_Style) == 0)
		{
			Error_Code = GetLastError();
			wprintf(L"Error : failed to set the window style (error %u).\n", Error_Code);
			if (Error_Code == ERROR_ACCESS_DENIED) wprintf(L"Try to run the program as administrator.\n");
			goto Exit_Error;
		}
		wprintf(L"The window border was successfully removed.\n");
	}

	// Maximize the game window if required to
	if (Games[Game_Index].Is_Window_Maximized)
	{
		ShowWindow(Handle, SW_MAXIMIZE);
		wprintf(L"The window was successfully maximized.\n");
	}

	return EXIT_SUCCESS;

Exit_Error:
	getchar();
	return EXIT_FAILURE;
}