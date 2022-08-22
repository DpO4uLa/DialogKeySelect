#pragma once
#include "main.h"

class CGameLoopHook {
private:
	using GameLoop_t = void(__cdecl*)();
	using CallbackGameLoop_t = void(__stdcall*)(void);
	std::unique_ptr<memwrapper::memhook<GameLoop_t>> pObjGameLoopHook = nullptr;
	static void __cdecl GameLoop_HOOKED();
	CallbackGameLoop_t GameLoopCallback;
public:
	inline void RegisterGameLoopCallback(CallbackGameLoop_t func) {
		GameLoopCallback = func;
	};

	CGameLoopHook() {
		pObjGameLoopHook = std::make_unique<memwrapper::memhook<GameLoop_t>>(0x00748DA3U, GameLoop_HOOKED);
		pObjGameLoopHook->install();
	};

	~CGameLoopHook() {
		pObjGameLoopHook->remove();
	};
};

CGameLoopHook* pGameLoopHook = nullptr;

void __cdecl CGameLoopHook::GameLoop_HOOKED() {
	if (pGameLoopHook->GameLoopCallback != 0) {
		pGameLoopHook->GameLoopCallback();
	}
	return pGameLoopHook->pObjGameLoopHook->call<>();
}