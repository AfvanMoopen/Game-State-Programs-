#pragma once

/****************************************************************************************
* Author:	Gilles Bellot
* Date:		08/03/2016 - Dortmund - Germany
*
* Desc:		main class to bring together all the core components of a game
*
****************************************************************************************/

// INCLUDES /////////////////////////////////////////////////////////////////////////////

// windows
#include <Windows.h>

// bell0bytes core
#include "window.h"

// bell0bytes util
#include "expected.h"


// CLASSES //////////////////////////////////////////////////////////////////////////////
namespace core
{
	class DirectXApp
	{
	protected:
		// application window
		HINSTANCE appInstance;					// handle to an instance of the application
		Window* appWindow;						// the application window (i.e. game window)

		// constructor and destructor
		DirectXApp(HINSTANCE hInstance);
		~DirectXApp();

		util::Expected<int> run();				// enters the main event loop
		
		// virtual methods, must be overriden
		virtual util::Expected<void> init();								// initializes the DirectX application
		virtual void shutdown(util::Expected<void>* expected = NULL);		// clean up and shutdown the DirectX application

	public:
		friend class Window;
	};
}