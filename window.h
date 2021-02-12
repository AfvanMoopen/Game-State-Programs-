#pragma once

/****************************************************************************************
* Author:	Gilles Bellot
* Date:		08/03/2016 - Dortmund - Germany
*
* Desc:		the Window class handles Windows related stuff, such as window creation and event handling
*
****************************************************************************************/

// INCLUDES /////////////////////////////////////////////////////////////////////////////

// windows
#include <Windows.h>

// bell0bytes util
#include "expected.h"

namespace core
{
	// DEFINITIONS //////////////////////////////////////////////////////////////////////////
	class DirectXApp;

	// CLASSES //////////////////////////////////////////////////////////////////////////////
	class Window
	{
	private:
		HWND mainWindow;						// handle to the main window
		DirectXApp* dxApp;						// the core application class

		util::Expected<void> init();			// initializes the window

	public:
		// constructor and destructor
		Window(DirectXApp* dxApp);
		~Window();

		// getters
		inline HWND getMainWindowHandle() { return mainWindow; };

		// the call back function
		virtual LRESULT CALLBACK msgProc(HWND hWnd, unsigned int msg, WPARAM wParam, LPARAM lParam);

		friend class DirectXApp;
	};
}