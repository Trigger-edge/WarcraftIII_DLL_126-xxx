#include "stdafx.h"
#include "Foundation.h"
#include <process.h>
#include "SystemTools.h"
#include "DreamUI.h"

static HFONT LoadingFont = NULL;
HMODULE hMdl;
bool Initialized = false;

//DWORD WINAPI INITTHREAD(LPVOID)
//{
//	Init();
//	Initialized = true;
//	return 0;
//}

void InitializeDreamUI(HMODULE _GameDll)
{
	if (!Initialized)
	{
		if (!SystemTools::Init()) {
			return;
		}
		Init(_GameDll);
		Initialized = true;
		//CreateThread(0, 0, INITTHREAD, 0, 0, 0);
	}
}

void UnitializeDreamUI()
{
	Cleanup();
	if (LoadingFont) {
		DeleteObject(LoadingFont);
		LoadingFont = NULL;
	}
	SystemTools::Cleanup();
	Initialized = false;
}
