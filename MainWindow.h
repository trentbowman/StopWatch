#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Window.h>
#include <StopWatch.h>
#include <MessageRunner.h>

class MainWindow : public BWindow
{
public:
	MainWindow(void);
	void AllAttached();
	void MessageReceived(BMessage *msg);
	bool QuitRequested(void);

	BMessageRunner 	*message_runner;
	BStopWatch		*stopwatch;

private:
	int TickTock(void);
	int UpdateClock(void);
	int DefaultButtonState(void);
	void ResumeTimer();
	void PauseTimer();
	void ResetTimer();
};

#endif
