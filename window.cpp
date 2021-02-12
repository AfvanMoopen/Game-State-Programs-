// INCLUDES /////////////////////////////////////////////////////////////////////////////

// bell0bytes core
#include "window.h"
#include "app.h"

// bell0bytes util
#include "serviceLocator.h"

// METHODS //////////////////////////////////////////////////////////////////////////////

namespace
{
	core::Window* window = NULL;
}

namespace core
{

	// the window procedure
	LRESULT CALLBACK mainWndProc(HWND hWnd, unsigned int msg, WPARAM wParam, LPARAM lParam)
	{
		// forward messages from a global window procedure to the member window procedure until a valid window handle is available 
		// this is needed because we can't assign a member function to WINDCLASS:lpfnWndProc
		return window->msgProc(hWnd, msg, wParam, lParam);
	}

	// CLASS METHODS ////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// Constructors /////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	Window::Window(DirectXApp* dxApp) : mainWindow(NULL), dxApp(dxApp)
	{
		// this is necessary to forward messages
		window = this;

		// initialize the window
		util::Expected<void> rv = this->init();
		if (!rv.isValid())
		{
			// log the error
			try { rv.get(); }
			catch (std::exception& e)
			{
				// create and print error message string
				std::stringstream errorMessage;
				errorMessage << "Creation of the game window failed with: " << e.what();
				util::ServiceLocator::getFileLogger()->print<util::SeverityType::error>(std::stringstream(errorMessage.str()));

				// throw an error
				throw std::runtime_error("Window creation failed!");
			}
		}
	}

	Window::~Window()
	{
		if (mainWindow)
			mainWindow = NULL;

		if (dxApp)
			dxApp = NULL;

		// log
		util::ServiceLocator::getFileLogger()->print<util::SeverityType::info>("Main window class destruction was successful.");
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// Window Creation //////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	util::Expected<void> Window::init()
	{
		// specify the window class description
		WNDCLASSEX wc;

		// white window
		wc.cbClsExtra = 0;										// no extra bytes needed
		wc.cbSize = sizeof(WNDCLASSEX);							// size of the window description structure
		wc.cbWndExtra = 0;										// no extra bytes needed
		wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// brush to repaint the background with
		wc.hCursor = LoadCursor(0, IDC_ARROW);					// load the standard arrow cursor
		wc.hIcon = LoadIcon(0, IDI_APPLICATION);				// load the standard application icon
		wc.hIconSm = LoadIcon(0, IDI_APPLICATION);				// load the standard small application icon
		wc.hInstance = dxApp->appInstance;						// handle to the core application instance
		wc.lpfnWndProc = mainWndProc;							// window procedure function
		wc.lpszClassName = L"bell0window";						// class name
		wc.lpszMenuName = 0;									// no menu
		wc.style = CS_HREDRAW | CS_VREDRAW;						// send WM_SIZE message when either the height or the width of the client area are changed

		// register the window
		if (!RegisterClassEx(&wc))
			return std::invalid_argument("The window class could not be registered; most probably due to invalid arguments!");

		// create the window
		mainWindow = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, wc.lpszClassName, L"bell0window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, dxApp->appInstance, NULL);
		if (!mainWindow)
			return std::invalid_argument("The window could not be created; most probably due to invalid arguments!");

		// show and update the windows
		ShowWindow(mainWindow, SW_SHOW);
		UpdateWindow(mainWindow);

		// log and return success
		util::ServiceLocator::getFileLogger()->print<util::SeverityType::info>("The main window was successfully created.");
		return {};
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////// Message Procedure ////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	LRESULT CALLBACK Window::msgProc(HWND hWnd, unsigned int msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{

		case WM_DESTROY:
			// window is flagged to be destroyed: send a quit message
			util::ServiceLocator::getFileLogger()->print<util::SeverityType::info>("The main window was flagged for destruction.");
			PostQuitMessage(0);
			return 0;
		}

		// let Windows handle other messages
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}