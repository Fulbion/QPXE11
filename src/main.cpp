#include "App/AppWindow.hpp"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
	AppWindow app;
	app.run();

	return EXIT_SUCCESS;
}