#pragma once
#include "main.h"

class CKeyHook {
private:
	using WndProc_t = LRESULT(__stdcall*)(HWND, UINT, WPARAM, LPARAM);
	std::unique_ptr<memwrapper::memhook<WndProc_t>> pObjWndProcHook = nullptr;
	static LRESULT __stdcall WndProc_HOOKED(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
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

	CKeyHook() {
		pObjWndProcHook = std::make_unique<memwrapper::memhook<WndProc_t>>(0x00747EB0U, WndProc_HOOKED);
		pObjWndProcHook->install();
	};

	~CKeyHook() {
		pObjWndProcHook->remove();
	};
};

CKeyHook* pKeyHook = nullptr;

LRESULT __stdcall CKeyHook::WndProc_HOOKED(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
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

	return pKeyHook->pObjWndProcHook->call<HWND, UINT, WPARAM, LPARAM>(hWnd, uMsg, wParam, lParam);
}