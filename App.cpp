#include "App.h"
#include "MainWindow.h"


App::App(void)
	:	BApplication("application/x-vnd.StopWatch")
{
	MainWindow *mainwin = new MainWindow();
	mainwin->Show();
	mainwin->SetSizeLimits(264,264,100,100);
}


int
main(void)
{
	App *app = new App();
	app->Run();
	delete app;
	return 0;
}
