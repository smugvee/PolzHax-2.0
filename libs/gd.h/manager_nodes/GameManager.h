#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__

#include <gd.h>

namespace gd {

	class PlayLayer;
	class GManager;

	class GameManager : public GManager {
	public:
		

	public:
		static GameManager* sharedState() {
			return reinterpret_cast<GameManager* (__stdcall*)()>(
				base + 0x9ac40
				)();
		}
		void reloadAll(bool bSwitch, bool bFullscreen, bool bReloadedInSession) {
			return reinterpret_cast<void(__thiscall*)(GameManager*, bool, bool, bool)>(
				base + 0xCE950
				)(this, bSwitch, bFullscreen, bReloadedInSession);
		}
		bool getGameVariable(const char* key) {
			return reinterpret_cast<bool(__thiscall*)(GameManager*, const char*)>(
				base + 0x9f520
				)(this, key);
		}
		void setGameVariable(const char* key, bool value) {
			return reinterpret_cast<void(__thiscall*)(GameManager*, const char*, bool)>(
				base + 0x9f340
				)(this, key, value);
		}
		int getIntGameVariable(const char* key) {
			return reinterpret_cast<int(__thiscall*)(GameManager*, const char*)>(
				base + 0x9f860
				)(this, key);
		}
		void setIntGameVariable(const char* key, int value) {
			reinterpret_cast<void(__thiscall*)(GameManager*, const char*, int)>(base + 0x9f780)(this, key, value);
		}
		static void toggleGameVariable(const char* key) {
			reinterpret_cast<void(__stdcall*)(const char*)>(base + 0x9f680)(key);
		}
		bool getUGV(const char* key) {
			return reinterpret_cast<bool(__thiscall*)(GameManager*, const char*)>(
				base + 0xCA0D0
				)(this, key);
		}
		void fadeInMusic(const char* filename) {
			return reinterpret_cast<void(__thiscall*)(GameManager*, const char*)>(
				base + 0xC4BD0
				)(this, filename);
		}
		PlayLayer* getPlayLayer() {
			return from<PlayLayer*>(this, 0x14c);
		}
		LevelEditorLayer* getLevelEditorLayer() {
			return from<LevelEditorLayer*>(this, 0x150);
		}
		bool getProgressBar() {
			return from<bool>(this, 0x238);
		}
	};
}

#endif