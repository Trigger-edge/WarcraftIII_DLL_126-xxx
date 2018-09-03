#include "Main.h"
#include <DreamUI.h>
#pragma comment(lib, "DreamUI.lib")

// Thanks @thewisp and @fluxxu for DreamDota https://github.com/DreamHacks/dreamdota

void InitializeDreamDotaAPI(BOOL config, HMODULE _GameDll)
{
	if (config)
		RegisterConfigWindow();
	else
		InitializeDreamUI(_GameDll);
}

void UninitializeDreamDotaAPI()
{
	UnitializeDreamUI();
}


