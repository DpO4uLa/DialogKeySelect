#include "main.h"

unsigned __int32 GetBase(void) {
	return (unsigned __int32)GetModuleHandleA("samp.dll");
}

bool IsListBoxActive(void) {
	return *(int*)(*(int*)(GetBase() + 0x21A0B8) + 0x20) != 0 ? true : false;
}

void ListBoxSetScrollIndex(int index) { // ??
	*(int*)(*(int*)(*(int*)(GetBase() + 0x21A0B8) + 0x20) + 0xEB) = index;
}

void ListBoxSetIndex(int index) {
	ListBoxSetScrollIndex(0);
	*(int*)(*(int*)(*(int*)(GetBase() + 0x21A0B8) + 0x20) + 0x143) = index;
}

unsigned __int32 ListBoxGetItemsCount(void) {
	return *(int*)(*(int*)(*(int*)(GetBase() + 0x21A0B8) + 0x20) + 0x150);
}

void __stdcall GameLoop(void) {
	if (!IsListBoxActive())
		return;

	unsigned __int32 ItemsCount = ListBoxGetItemsCount();

	if (pKeyHook->isKeyPressed('1') && ItemsCount >= 1)
		ListBoxSetIndex(0);

	if (pKeyHook->isKeyPressed('2') && ItemsCount >= 2)
		ListBoxSetIndex(1);

	if (pKeyHook->isKeyPressed('3') && ItemsCount >= 3)
		ListBoxSetIndex(2);

	if (pKeyHook->isKeyPressed('4') && ItemsCount >= 4)
		ListBoxSetIndex(3);

	if (pKeyHook->isKeyPressed('5') && ItemsCount >= 5)
		ListBoxSetIndex(4);

	if (pKeyHook->isKeyPressed('6') && ItemsCount >= 6)
		ListBoxSetIndex(5);

	if (pKeyHook->isKeyPressed('7') && ItemsCount >= 7)
		ListBoxSetIndex(6);

	if (pKeyHook->isKeyPressed('8') && ItemsCount >= 8)
		ListBoxSetIndex(7);

	if (pKeyHook->isKeyPressed('9') && ItemsCount >= 9)
		ListBoxSetIndex(8);
}

int __stdcall DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
		case DLL_PROCESS_ATTACH: {
			pKeyHook = new CKeyHook();
			pGameLoopHook = new CGameLoopHook();

			pGameLoopHook->RegisterGameLoopCallback(GameLoop);

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