#include "main.h"

unsigned __int32 GetBase(void) {
	return (unsigned __int32)GetModuleHandleA("samp.dll");
}

bool IsListBoxActive(void) {
	if (GetBase() == 0)
		return false;
	if(*(int*)(GetBase() + 0x21A0B8) == 0)
		return false;
	if (*(int*)(*(int*)(GetBase() + 0x21A0B8) + 0x20) == 0)
		return false;
	return true;
}

int* ListBoxIndex;

void ListBoxSetScrollIndex(int index) { // ??
	*(int*)(*(int*)(*(int*)(GetBase() + 0x21A0B8) + 0x20) + 0xEB) = index;
}

unsigned __int32 ListBoxGetItemsCount(void) {
	return *(int*)(*(int*)(*(int*)(GetBase() + 0x21A0B8) + 0x20) + 0x150);
}

int __stdcall WndProc(int* uMsg, int* wParam, int* lParam) {

	if (*uMsg == WM_MOUSEWHEEL) {
		if (!IsListBoxActive())
			return 0;

		if (*wParam > 0) {
			if (((*ListBoxIndex) - 1) < 0)
				return 0;
			(*ListBoxIndex)--;
			ListBoxSetScrollIndex(*ListBoxIndex);
		}
		else if (*wParam < 0)
		{
			if (((*ListBoxIndex) + 1) > (ListBoxGetItemsCount() - 1))
				return 0;
			(*ListBoxIndex)++;
			ListBoxSetScrollIndex((*ListBoxIndex) - 6);
		}
	}

	return 0;
}

void __stdcall GameLoop(void) {
	if (!IsListBoxActive())
		return;

	ListBoxIndex = (int*)(*(int*)(*(int*)(GetBase() + 0x21A0B8) + 0x20) + 0x143);
	unsigned __int32 ItemsCount = ListBoxGetItemsCount();

	if (pKeyHook->isKeyPressed('1') && ItemsCount >= 1) {
		*ListBoxIndex = 0;
		ListBoxSetScrollIndex(0);
	}

	if (pKeyHook->isKeyPressed('2') && ItemsCount >= 2) {
		*ListBoxIndex = 1;
		ListBoxSetScrollIndex(0);
	}

	if (pKeyHook->isKeyPressed('3') && ItemsCount >= 3) {
		*ListBoxIndex = 2;
		ListBoxSetScrollIndex(0);
	}

	if (pKeyHook->isKeyPressed('4') && ItemsCount >= 4) {
		*ListBoxIndex = 3;
		ListBoxSetScrollIndex(0);
	}

	if (pKeyHook->isKeyPressed('5') && ItemsCount >= 5) {
		*ListBoxIndex = 4;
		ListBoxSetScrollIndex(0);
	}

	if (pKeyHook->isKeyPressed('6') && ItemsCount >= 6) {
		*ListBoxIndex = 5;
		ListBoxSetScrollIndex(0);
	}

	if (pKeyHook->isKeyPressed('7') && ItemsCount >= 7) {
		*ListBoxIndex = 6;
		ListBoxSetScrollIndex(0);
	}

	if (pKeyHook->isKeyPressed('8') && ItemsCount >= 8) {
		*ListBoxIndex = 7;
		ListBoxSetScrollIndex(0);
	}

	if (pKeyHook->isKeyPressed('9') && ItemsCount >= 9) {
		*ListBoxIndex = 8;
		ListBoxSetScrollIndex(0);
	}
}

int __stdcall DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
		case DLL_PROCESS_ATTACH: {
			pKeyHook = new CKeyHook();
			pGameLoopHook = new CGameLoopHook();

			pGameLoopHook->RegisterGameLoopCallback(GameLoop);
			pKeyHook->RegisterWndProcCallback(WndProc);

#ifdef _DEBUG
#pragma warning(disable:28159)
			printf("\n -> Plugin loaded (%d)\n", GetTickCount());
#pragma warning(default:28159)
#endif
			break;
		}
		case DLL_PROCESS_DETACH: {
			delete pGameLoopHook;
			delete pKeyHook;

			pGameLoopHook = nullptr;
			pKeyHook = nullptr;

#ifdef  _DEBUG
#pragma warning(disable:28159)
			printf("\n -> Plugin unloaded (%d)\n", GetTickCount());
#pragma warning(default:28159)
#endif
			break;
		}
	}
	return true;
}