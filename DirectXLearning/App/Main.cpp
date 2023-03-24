#include "AppWindow.h"

int main()
{
	AppWindow app;
	if(app.Init())
	{
		while(app.IsRunning())
		{
			app.BroadCast();
		}
		
	}

	return 0;
}