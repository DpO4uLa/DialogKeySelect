#pragma once
#include "main.h"

class CKeyHook {
private:
	using SampWndProc_t = int(__thiscall*)(int, int, int);
	using CallbackWndProc_t = int(__stdcall*)(int*, int*, int*);
	std::unique_ptr<memwrapper::memhook<SampWndProc_t>> pObjSampWndProcHook = nullptr;
	static int __fastcall WndProc_HOOKED(void* _this, void* edx86, int uMsg, int wParam, int lParam);
	CallbackWndProc_t WndProcCallback;
	bool bKeyTable[256];
public:
	bool isKeyDown(uint8_t key) {
		return bKeyTable[key];
	}
	bool isKeyPressed(uint8_t key) {
		static bool bPressed[0xFF] = { 0 };
		if (bKeyTable[key]) {
			if (!bPressed[key])
				return bPressed[key] = true;
		}
		else bPressed[key] = false;

		return false;
	}
	bool isKeyReleased(uint8_t vkey) {
		static bool bPressed[0xFF] = { 0 };
		if (!bKeyTable[vkey]) {
			if (bPressed[vkey])
				return !(bPressed[vkey] = false);
		}
		else bPressed[vkey] = true;

		return false;
	}

	inline void RegisterWndProcCallback(CallbackWndProc_t func) {
		WndProcCallback = func;
	};

	CKeyHook() {
		pObjSampWndProcHook = std::make_unique<memwrapper::memhook<SampWndProc_t>>((DWORD)GetModuleHandleA("samp.dll") + 0x65B30, WndProc_HOOKED);
		pObjSampWndProcHook->install();
	};

	~CKeyHook() {
		pObjSampWndProcHook->remove();
	};
};

CKeyHook* pKeyHook = nullptr;

int __fastcall CKeyHook::WndProc_HOOKED(void* _this, void* edx86, int uMsg, int wParam, int lParam) {
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
		pKeyHook->bKeyTable[VK_LBUTTON] = (uMsg == WM_LBUTTONDOWN);
		break;

	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
		pKeyHook->bKeyTable[VK_RBUTTON] = (uMsg == WM_RBUTTONDOWN);
		break;

	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
		pKeyHook->bKeyTable[VK_MBUTTON] = (uMsg == WM_MBUTTONDOWN);
		break;

	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_KEYDOWN:
	case WM_KEYUP:
	{
		bool bDown = (uMsg == WM_SYSKEYDOWN || uMsg == WM_KEYDOWN);
		int	iKey = (int)wParam;
		uint32_t ScanCode = LOBYTE(HIWORD(lParam));

		pKeyHook->bKeyTable[iKey] = bDown;

		switch (iKey)
		{
		case VK_SHIFT:
			if (ScanCode == MapVirtualKey(VK_LSHIFT, 0)) pKeyHook->bKeyTable[VK_LSHIFT] = bDown;
			if (ScanCode == MapVirtualKey(VK_RSHIFT, 0)) pKeyHook->bKeyTable[VK_RSHIFT] = bDown;
			break;

		case VK_CONTROL:
			if (ScanCode == MapVirtualKey(VK_LCONTROL, 0)) pKeyHook->bKeyTable[VK_LCONTROL] = bDown;
			if (ScanCode == MapVirtualKey(VK_RCONTROL, 0)) pKeyHook->bKeyTable[VK_RCONTROL] = bDown;
			break;

		case VK_MENU:
			if (ScanCode == MapVirtualKey(VK_LMENU, 0)) pKeyHook->bKeyTable[VK_LMENU] = bDown;
			if (ScanCode == MapVirtualKey(VK_RMENU, 0)) pKeyHook->bKeyTable[VK_RMENU] = bDown;
			break;
		}
		break;
	}
	}

	if (pKeyHook->WndProcCallback != 0) {
		LRESULT res = pKeyHook->WndProcCallback(&uMsg, &wParam, &lParam);
		if (res != 0)
			return res;
	}

	return pKeyHook->pObjSampWndProcHook->call<void*, int, int, int>(_this, uMsg, wParam, lParam);
}