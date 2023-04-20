#pragma once

#include <Windows.h>
#include <stdexcept>

class Window
{
public:
	Window();
	~Window();

	bool isRunning();

	RECT getClientWindowRect();
	RECT getScreenSize();
	// void setHWND(HWND hwnd);

	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();
	virtual void onSize();

protected:
	HWND m_hwnd;
	bool m_isRunning;
	bool m_isInitialized = false;

private:
	bool broadcast();
};
