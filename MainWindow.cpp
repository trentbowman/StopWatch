#include "MainWindow.h"
#include <Application.h>
#include <Button.h>
#include <StringView.h>
#include <View.h>
#include <String.h>
#include <be/kernel/OS.h>
#include <unistd.h>

bool clockStart = false;
bool clockReset = false;
BString displayClock;
BString allZeros = "00:00:00.0";
BStringView *clockStringView;
BButton *startButton;
BButton *stopButton;
BButton *resetButton;

enum
{
	M_BUTTON_START = 'btn1',
	M_BUTTON_STOP = 'btn2',
	M_BUTTON_RESET = 'btn3',
	M_UPDATE_CLOCK = 'uclk'
};

MainWindow::MainWindow(void)
	:	BWindow(BRect(100,100,324,200), "StopWatch", B_TITLED_WINDOW,
				B_NOT_ZOOMABLE | B_NOT_RESIZABLE |
				B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE)
{
	BView *mainView = new BView(Bounds(), "mainView", B_FOLLOW_ALL, B_WILL_DRAW);
	
	clockStringView = new BStringView(BRect(10,10,254,55), "clockStringView", NULL);
	
	startButton = new BButton(BRect(9,65,87,90), "startButton", "Start",
							  new BMessage(M_BUTTON_START));
								  
	stopButton = new BButton(BRect(93,65,171,90), "stopButton", "Stop",
							 new BMessage(M_BUTTON_STOP));
								   	  
	resetButton = new BButton(BRect(177,65,255,90), "resetButton", "Reset",
							  new BMessage(M_BUTTON_RESET));
	
	mainView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	
	AddChild(mainView);
	mainView->AddChild(clockStringView);
	mainView->AddChild(startButton);
	mainView->AddChild(stopButton);
	mainView->AddChild(resetButton);
	
	BFont font(be_plain_font);
    font.SetSize(40.0);
    font.SetFamilyAndStyle("DejaVu Sans Mono", "Book");
	
	clockStringView->SetFont(&font);
	clockStringView->SetAlignment(B_ALIGN_CENTER);
	clockStringView->SetViewColor(0,0,0);
	clockStringView->SetHighColor(255,255,255);
	clockStringView->SetText(allZeros.String());
	
	startButton->MakeFocus(true);
	stopButton->SetEnabled(false);
	
	this->stopwatch = new BStopWatch("stopwatch", true);
	this->stopwatch->Reset();
	this->stopwatch->Suspend();
}

void
MainWindow::MessageReceived(BMessage *msg)
{	
	switch (msg->what)
	{
		case M_BUTTON_START:
		{
			this->ResumeTimer();
			break;	
		}
		case M_BUTTON_STOP:
		{
			this->PauseTimer();
			break;
		}
		case M_BUTTON_RESET:
		{
			this->ResetTimer();
			break;
		}
		case M_UPDATE_CLOCK:
		{
			this->UpdateClock();
			break;
		}
		default:
		{
			BWindow::MessageReceived(msg);
			break;
		}
	}
}


bool
MainWindow::QuitRequested(void)
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}


int
MainWindow::DefaultButtonState(void)
{
	startButton->SetEnabled(true);
	startButton->MakeFocus(true);
	stopButton->SetEnabled(false);
	return(0);	
}

//int
//MainWindow::UpdateClock(void)
//{
//	PostMessage(M_UPDATE_CLOCK);
//	usleep(9750);
//	return(0);	
//}

void
MainWindow::ResumeTimer(void) {
	this->message_runner = new BMessageRunner(this, M_UPDATE_CLOCK, 100000, -1);
	this->stopwatch->Resume();
	clockStart = true;
	clockReset = false;
	startButton->SetEnabled(false);
	stopButton->SetEnabled(true);
	stopButton->MakeFocus(true);
}

void
MainWindow::PauseTimer(void) {
	if (this->message_runner) {
				delete this->message_runner;
				this->message_runner = NULL;
	}
	this->stopwatch->Suspend();
	clockStart = false;
	clockReset = false;
	DefaultButtonState();
}

void
MainWindow::ResetTimer() {
	if (this->message_runner) {
		delete this->message_runner;
		this->message_runner = NULL;
	}
	this->stopwatch->Reset();
	clockStart = false;
	clockReset = true;
	UpdateClock();
	DefaultButtonState();
}
int
MainWindow::UpdateClock(void)
{
	int totalSeconds = 0;
	int totalMinutes = 0;
	int totalHours = 0;
	
	bigtime_t elapsedMicroseconds = this->stopwatch->ElapsedTime();
	
	totalSeconds = elapsedMicroseconds / 1000000;
	totalMinutes = totalSeconds / 60;
	totalHours = totalMinutes / 60;
	totalHours %= 24;

	int H1, H2, M1, M2, S1, S2, TS;
	H1 = ((totalHours % 60) / 10);
	H2 = ((totalHours % 60) % 10);
	M1 = ((totalMinutes % 60) / 10);
	M2 = ((totalMinutes % 60) % 10);
	S1 = ((totalSeconds % 60) / 10);
	S2 = ((totalSeconds % 60) % 10);
	TS = ((elapsedMicroseconds % 1000000) / 100000);
	displayClock = "";
	displayClock << H1 << H2 << ":" << M1 << M2 << ":" << S1 << S2 << "." << TS;
	
	clockStringView->SetText(displayClock.String());
	if(clockReset) { 
		clockStringView->SetText(allZeros.String());
	}

	return(0);
}
