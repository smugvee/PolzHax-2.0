#ifndef __FMODAUDIOENGINE_H__
#define __FMODAUDIOENGINE_H__

#include <gd.h>
#include <fmod.hpp>

//namespace FMOD {
//	static auto base = GetModuleHandleA("fmod.dll");
//	class Channel {
//		void setPitch(float pitch) {
//			static const auto addr = GetProcAddress(base, "?setPitch@ChannelControl@FMOD@@QAG?AW4FMOD_RESULT@@M@Z");
//			reinterpret_cast<void* (__stdcall*)(void*, float)>(addr)(this, pitch);
//		}
//		void setReverbProperties(int idk, float idk2) {
//			static const auto addr = GetProcAddress(base, "?setReverbProperties@ChannelControl@FMOD@@QAG?AW4FMOD_RESULT@@HM@Z");
//			reinterpret_cast<void* (__stdcall*)(void*, int, float)>(addr)(this, idk, idk2);
//		}
//	};
//}

namespace gd {
	class FMODAudioEngine : public cocos2d::CCNode {
	public:
		cocos2d::CCDictionary* m_dictionary; // 0xe8
		std::string m_filePath; // 0xec
		float m_backgroundMusicVolume; // 0x104
		float m_effectsVolume; // 0x108
		float m_pulse1; // 0x10c
		float m_pulse2; // 0x110
		float m_pulse3; // 0x114
		int m_pulseCounter; // 0x118
		bool m_metering; // 0x11c
		bool m_fading; // 0x11d
		bool m_fadeIn; // 0x11e
		float m_fadeInDuration; // 0x120
		FMOD::System* m_system; // 0x124
		FMOD::Sound* m_sound; // 0x128
		FMOD::Channel* m_currentSoundChannel; // 0x12c
		FMOD::Channel* m_globalChannel; // 0x130
		FMOD::DSP* m_DSP; // 0x134
		FMOD_RESULT m_lastResult; // 0x138
		int m_version; // 0x13c
		void* m_extraDriverData; // 0x140
		int m_musicOffset; // 0x144

	public:
		static FMODAudioEngine* sharedEngine() {
			return reinterpret_cast<FMODAudioEngine * (__stdcall*)()>(
				base + 0x20430
				)();
		}
		void preloadEffect(std::string filename) {
			return reinterpret_cast<void(__thiscall*)(FMODAudioEngine*, std::string)>(
				base + 0x24240
				)(this, filename);
		}
		//my own function
		//void reloadEffects() {
		//	using namespace std::filesystem;

		//	std::vector<std::string> oggs = {};
		//	path defaults = current_path() / "Resources";
		//	for (auto file : directory_iterator{ defaults }) {
		//		if (file.path().extension() == ".ogg")
		//			oggs.push_back(file.path().filename().string());
		//	}
		//	for (auto ogg : oggs) {
		//		m_pDictionary->removeObjectForKey(ogg);
		//		this->preloadEffect(ogg);
		//	}
		//}
		auto currentSound() {
			return from<FMOD::Channel*>(this, 0x130);
		}

		void playBackgroundMusic(std::string const& path, bool idk0, bool idk1) {
			reinterpret_cast<void(__thiscall*)(FMODAudioEngine*, bool, bool, std::string)>(
				base + 0x207c0
				)(this, idk0, idk1, path);
		}
	};
}

#endif