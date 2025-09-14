#include "pch.h"
#include "menu.h"

#include <imgui-hook.hpp>
#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>
#include <filesystem>
#include <fstream>
#include <chrono>

#include "state.h"

#include "explorer.h"
#include "utils.hpp"
#include "patching.h"

#include "SpeedHack.h"
#include "fmod.hpp"
#include "fmod_dsp_effects.h"
#include "shellapi.h"

#include "portable-file-dialogs.h"

auto libcocosbase = GetModuleHandleA("libcocos2d.dll");

ImVec4 color1;
ImVec4 color2;
ImVec4 color3;
ImVec4 color4;
ImVec4 color5;
ImVec4 color6;

bool oneX = true;

void update_fps_bypass() {
	const auto value = setting().onFPSBypass ? setting().fps : 60.f;
	static const auto addr = cocos_symbol<&CCApplication::setAnimationInterval>("?setAnimationInterval@CCApplication@cocos2d@@UAEXN@Z");
	addr(CCApplication::sharedApplication(), 1.0 / value);
	CCDirector::sharedDirector()->setAnimationInterval(1.0 / value);
}

void cheatDec()
{
	setting().cheatsCount--;
}

void cheatAdd()
{
	setting().cheatsCount++;
	setting().beforeRestartCheatsCount++;
}

void update_speed_hack() {
	const auto value = setting().onSpeedhack ? setting().speedhack : 1.f;
	if (auto fme = gd::FMODAudioEngine::sharedEngine())
		if (auto sound = fme->currentSound())
			if (setting().onSpeedhackMusic) sound->setPitch(value);
			else sound->setPitch(1.f);
	if (setting().speedhack == 0)
		return;
	if (!setting().onClassicMode) {
		CCDirector::sharedDirector()->m_pScheduler->setTimeScale(value);
		SpeedHack::SetSpeed(1.f);
	}
	else {
		CCDirector::sharedDirector()->m_pScheduler->setTimeScale(1.f);
		SpeedHack::SetSpeed(value);
	}
}

std::vector<std::string> dllNames;

float solidsColor[3];
float hazardsColor[3];
float specialsColor[3];

float levelBGColor[3];
float levelGColor[3];

void imgui_render() {
	if (oneX) {
		setting().loadState();

		setting().cheatsCount = 0;
		setting().beforeRestartCheatsCount = 0;

		solidsColor[0] = setting().solidHitboxesR / 255.f;
		solidsColor[1] = setting().solidHitboxesG / 255.f;
		solidsColor[2] = setting().solidHitboxesB / 255.f;

		hazardsColor[0] = setting().hazardHitboxesR / 255.f;
		hazardsColor[1] = setting().hazardHitboxesG / 255.f;
		hazardsColor[2] = setting().hazardHitboxesB / 255.f;

		specialsColor[0] = setting().specialHitboxesR / 255.f;
		specialsColor[1] = setting().specialHitboxesG / 255.f;
		specialsColor[2] = setting().specialHitboxesB / 255.f;

		levelBGColor[0] = setting().levelBGColorR / 255.f;
		levelBGColor[1] = setting().levelBGColorG / 255.f;
		levelBGColor[2] = setting().levelBGColorB / 255.f;

		levelGColor[0] = setting().levelGColorR / 255.f;
		levelGColor[1] = setting().levelGColorG / 255.f;
		levelGColor[2] = setting().levelGColorB / 255.f;

		float polzhax_xPos;
		float bypass_xPos;
		float cosmetic_xPos;
		float creator_xPos;
		float level_xPos;
		float universal_xPos;
		float addingSpeedhackY;

		if (ImGui::Begin("PolzHax", nullptr)) {
			ImGui::SetWindowPos({ 5,5 });
			polzhax_xPos = ImGui::GetWindowWidth() + 10;
		}

		if (ImGui::Begin("Bypass", nullptr)) {
			ImGui::SetWindowPos({ polzhax_xPos, 5 });
			bypass_xPos = polzhax_xPos + ImGui::GetWindowWidth() + 5;
		}

		if (ImGui::Begin("Cosmetic", nullptr)) {
			ImGui::SetWindowPos({ bypass_xPos, 5 });
			cosmetic_xPos = bypass_xPos + ImGui::GetWindowWidth() + 5;
		}

		if (ImGui::Begin("Creator", nullptr)) {
			ImGui::SetWindowPos({ cosmetic_xPos, 5 });
			creator_xPos = cosmetic_xPos + ImGui::GetWindowWidth() + 5;
		}

		if (ImGui::Begin("Level", nullptr)) {
			ImGui::SetWindowPos({ creator_xPos, 5 });
			level_xPos = creator_xPos + ImGui::GetWindowWidth() + 5;
		}

		if (ImGui::Begin("Universal", nullptr)) {
			ImGui::SetWindowPos({ level_xPos, 5 });
			universal_xPos = level_xPos + ImGui::GetWindowWidth() + 5;
			addingSpeedhackY = ImGui::GetWindowHeight() + 10;
		}

		if (ImGui::Begin("Speedhack", nullptr)) {
			ImGui::SetWindowPos({ level_xPos, addingSpeedhackY });
		}

		// Bypass

		if (setting().onTheVault) {
			sequence_patch((uint32_t)gd::base + 0x1567fb, { 0xe9, 0x72, 0x01, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x1567fb, { 0x0f, 0x85, 0x71, 0x01, 0x00, 0x00 });
		}

		if (setting().onLoadFailed) {
			sequence_patch((uint32_t)gd::base + 0x117908, { 0xe9, 0x13, 0x01, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x117908, { 0x0f, 0x87, 0x12, 0x01, 0x00, 0x00 });
		}

		if (setting().onMainLevels) {
			sequence_patch((uint32_t)gd::base + 0x11510b, { 0xe9, 0xa7, 0x02, 0x00, 0x00, 0x90 });
			sequence_patch((uint32_t)gd::base + 0x115b82, { 0xe9, 0x98, 0x00, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x11510b, { 0x0f, 0x84, 0xa6, 0x02, 0x00, 0x00 });
			sequence_patch((uint32_t)gd::base + 0x115b82, { 0x0f, 0x8e, 0x97, 0x00, 0x00, 0x00 });
		}

		if (setting().onSliderLimit) {
			sequence_patch((uint32_t)gd::base + 0x2a567, { 0xeb, 0x07 });
			sequence_patch((uint32_t)gd::base + 0x2a57b, { 0xeb, 0x07 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x2a567, { 0x76, 0x07 });
			sequence_patch((uint32_t)gd::base + 0x2a57b, { 0x76, 0x07 });
		}

		// Cosmetic

		if (setting().onForceDontEnter) {
			sequence_patch((uint32_t)gd::base + 0x177926, { 0xe9, 0xe4, 0x01, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x177926, { 0x0f, 0x85, 0xe3, 0x01, 0x00, 0x00 });
		}

		if (setting().onForceDontFade) {
			sequence_patch((uint32_t)gd::base + 0x1770de, { 0xe9, 0x83, 0x00, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x1770de, { 0x0f, 0x85, 0x82, 0x00, 0x00, 0x00 });
		}

		if (setting().onInstantMirror) {
			sequence_patch((uint32_t)gd::base + 0x17bdf6, { 0x00, 0x00, 0x00, 0x00 });
			cheatAdd();
		}
		else
			sequence_patch((uint32_t)gd::base + 0x17bdf6, { 0x00, 0x00, 0x00, 0x3f });

		if (setting().onInversedTrail) {
			sequence_patch((uint32_t)libcocosbase + 0xad5b6, { 0x0f, 0x85, 0x55, 0x02, 0x00, 0x00 });
		}
		else {
			sequence_patch((uint32_t)libcocosbase + 0xad5b6, { 0x0f, 0x84, 0x55, 0x02, 0x00, 0x00 });
		}

		if (setting().onNoBackgroundFlash) {
			sequence_patch((uint32_t)gd::base + 0x16e331, { 0x6a, 0x00 });
			sequence_patch((uint32_t)gd::base + 0x16e34b, { 0xc7, 0x04, 0x24, 0x00, 0x00, 0x00, 0x00 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x16e331, { 0x6a, 0x01 });
			sequence_patch((uint32_t)gd::base + 0x16e34b, { 0xc7, 0x04, 0x24, 0x8f, 0xc2, 0xf5, 0x3d });
		}

		if (setting().onNoDeathEffect) {
			sequence_patch((uint32_t)gd::base + 0x164c2d, { 0xe9, 0xe7, 0x01, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x164c2d, { 0x0f, 0x84, 0xd5, 0x01, 0x00, 0x00 });
		}

		if (setting().onNoGhostEffect) {
			sequence_patch((uint32_t)gd::base + 0x17d664, { 0x6a, 0x00, 0x90 });
			sequence_patch((uint32_t)gd::base + 0x17d67d, { 0x6a, 0x00, 0x90 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x17d664, { 0xff, 0x75, 0x08 });
			sequence_patch((uint32_t)gd::base + 0x17d67d, { 0xff, 0x75, 0x08 });
		}

		if (setting().onNoGlow)
			sequence_patch((uint32_t)gd::base + 0xa4442, { 0xe9, 0x91, 0x01, 0x00, 0x00, 0x90 });
		else
			sequence_patch((uint32_t)gd::base + 0xa4442, { 0x0f, 0x85, 0x90, 0x01, 0x00, 0x00 });

		if (setting().onNoGravityEffect) {
			sequence_patch((uint32_t)gd::base + 0x17e6f6, { 0x90, 0x90 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x17e6f6, { 0x75, 0x07 });
		}

		if (setting().onNoLightning) {
			sequence_patch((uint32_t)gd::base + 0x16e28f, { 0xe9, 0x97, 0x00, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x16e28f, { 0x0f, 0x85, 0x96, 0x00, 0x00, 0x00 });
		}

		if (setting().onNoMirror) {
			sequence_patch((uint32_t)gd::base + 0x17bcb2, { 0xe9, 0x9b, 0x01, 0x00, 0x00, 0x90 });
			cheatAdd();
		}
		else
			sequence_patch((uint32_t)gd::base + 0x17bcb2, { 0x0f, 0x84, 0x9a, 0x01, 0x00, 0x00 });

		if (setting().onNoRespawnFlash) {
			sequence_patch((uint32_t)gd::base + 0x1643a1, { 0xe9, 0x9f, 0x00, 0x00, 0x00, 0x00 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x1643a1, { 0x0f, 0x85, 0x9e, 0x00, 0x00, 0x00 });
		}

		if (setting().onPracticePulse) {
			sequence_patch((uint32_t)gd::base + 0x39805, { 0x90, 0x90 });
			sequence_patch((uint32_t)gd::base + 0x17655a, { 0xeb, 0x16 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x39805, { 0x75, 0x0c });
			sequence_patch((uint32_t)gd::base + 0x17655a, { 0x74, 0x16 });
		}

		if (setting().onSolidWaveTrail) {
			sequence_patch((uint32_t)gd::base + 0x15f46b, { 0x90, 0x90 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x15f46b, { 0x75, 0x0c });
		}

		if (setting().onTrailAlwaysOff) {
			sequence_patch((uint32_t)libcocosbase + 0xad5b6, { 0xe9, 0x56, 0x02, 0x00, 0x00, 0x90 });
		}
		else if (setting().onTrailAlwaysOn) {
			sequence_patch((uint32_t)libcocosbase + 0xad5b6, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch((uint32_t)libcocosbase + 0xad5b6, { 0x0f, 0x84, 0x55, 0x02, 0x00, 0x00 });
		}

		if (setting().onTrailBugFix) {
			sequence_patch((uint32_t)libcocosbase + 0xad1ad, { 0xbb, 0xff, 0x00, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch((uint32_t)libcocosbase + 0xad1ad, { 0xf3, 0x0f, 0x2c, 0xc1, 0x2b, 0xd8 });
		}

		// Creator

		if (setting().onAbsolutePosition) {
			sequence_patch((uint32_t)gd::base + 0x6e3bd, { 0x90, 0x8b, 0xce, 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x6e3bd, { 0x51, 0x8b, 0xce, 0xff, 0x50, 0x5c });
		}

		if (setting().onCopyHack) {
			sequence_patch((uint32_t)gd::base + 0x10841f, { 0x8b, 0xce, 0x90 });
			sequence_patch((uint32_t)gd::base + 0x1084b4, { 0xb0, 0x01, 0x90 });
			sequence_patch((uint32_t)gd::base + 0x109620, { 0xeb, 0x18 });
			sequence_patch((uint32_t)gd::base + 0x10a079, { 0x90, 0x90 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x10841f, { 0x0f, 0x44, 0xce });
			sequence_patch((uint32_t)gd::base + 0x1084b4, { 0x0f, 0x95, 0xc0 });
			sequence_patch((uint32_t)gd::base + 0x109620, { 0x74, 0x18 });
			sequence_patch((uint32_t)gd::base + 0x10a079, { 0x75, 0x11 });
		}

		if (setting().onCustomObjectBypass) {
			sequence_patch((uint32_t)gd::base + 0x607c1, { 0xeb, 0x7f });
			sequence_patch((uint32_t)gd::base + 0x608a3, { 0xe9, 0xaf, 0x00, 0x00, 0x00, 0x90 });
			sequence_patch((uint32_t)gd::base + 0x609cf, { 0x90, 0x90 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x607c1, { 0x76, 0x7f });
			sequence_patch((uint32_t)gd::base + 0x608a3, { 0x0f, 0x82, 0xae, 0x00, 0x00, 0x00 });
			sequence_patch((uint32_t)gd::base + 0x609cf, { 0x74, 0x4f });
		}

		if (setting().onDefaultSongBypass) {
			sequence_patch((uint32_t)gd::base + 0x10560f, { 0x90, 0x90 });
			sequence_patch((uint32_t)gd::base + 0x105621, { 0x90, 0x90, 0x90 });
			sequence_patch((uint32_t)gd::base + 0x10568e, { 0x90, 0x90 });
			sequence_patch((uint32_t)gd::base + 0x1056a0, { 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x10560f, { 0x74, 0x4e });
			sequence_patch((uint32_t)gd::base + 0x105621, { 0x0f, 0x4f, 0xf0 });
			sequence_patch((uint32_t)gd::base + 0x10568e, { 0x74, 0x4e });
			sequence_patch((uint32_t)gd::base + 0x1056a0, { 0x0f, 0x4f, 0xf0 });
		}

		if (setting().onEditorExtension) {
			sequence_patch((uint32_t)gd::base + 0x20cf68, { 0x00, 0xc8, 0x2f, 0x4b });
			sequence_patch((uint32_t)gd::base + 0x6fdb9, { 0x00, 0xd7, 0x2f, 0x4b });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x20cf68, { 0x00, 0xc8, 0xaf, 0x47 });
			sequence_patch((uint32_t)gd::base + 0x6fdb9, { 0x00, 0xd7, 0xaf, 0x47 });
		}

		if (setting().onFreeScroll) {
			sequence_patch((uint32_t)gd::base + 0x6fe05, { 0xeb, 0x08 });
			sequence_patch((uint32_t)gd::base + 0x6fe1c, { 0xeb, 0x05 });
			sequence_patch((uint32_t)gd::base + 0x6fe35, { 0xeb, 0x08 });
			sequence_patch((uint32_t)gd::base + 0x6fe4c, { 0xeb, 0x05 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x6fe05, { 0x77, 0x08 });
			sequence_patch((uint32_t)gd::base + 0x6fe1c, { 0x77, 0x05 });
			sequence_patch((uint32_t)gd::base + 0x6fe35, { 0x77, 0x08 });
			sequence_patch((uint32_t)gd::base + 0x6fe4c, { 0x77, 0x05 });
		}

		if (setting().onHidePreviewLine) {
			sequence_patch((uint32_t)gd::base + 0xff890, { 0xe9, 0x04, 0x01, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0xff890, { 0x0f, 0x84, 0x03, 0x01, 0x00, 0x00 });
		}

		if (setting().onLevelEdit) {
			sequence_patch((uint32_t)gd::base + 0x15baaf, { 0x90, 0x90 });
			sequence_patch((uint32_t)gd::base + 0x75757, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x15baaf, { 0x75, 0x62 });
			sequence_patch((uint32_t)gd::base + 0x75757, { 0x0f, 0x85, 0xca, 0x00, 0x00, 0x00 });
		}

		if (setting().onNoCMark) {
			sequence_patch((uint32_t)gd::base + 0x7d0e0, { 0xb8, 0x00, 0x00, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x7d0e0, { 0x8b, 0x81, 0x18, 0x02, 0x00, 0x00 });
		}

		if (setting().onNoDeathX) {
			sequence_patch((uint32_t)gd::base + 0xee33a, { 0x00, 0x00, 0x00, 0x00 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0xee33a, { 0x33, 0x33, 0x33, 0x3f });
		}

		if (setting().onObjectBypass) {
			sequence_patch((uint32_t)gd::base + 0x5a727, { 0x68, 0xff, 0xff, 0xff, 0x7f });
			sequence_patch((uint32_t)gd::base + 0x69902, { 0x3d, 0xff, 0xff, 0xff, 0x7f });
			sequence_patch((uint32_t)gd::base + 0x6b895, { 0x3d, 0xff, 0xff, 0xff, 0x7f });
			sequence_patch((uint32_t)gd::base + 0x6bbf9, { 0x3d, 0xff, 0xff, 0xff, 0x7f });
			sequence_patch((uint32_t)gd::base + 0xef1af, { 0xff, 0xff, 0xff, 0x7f });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x5a727, { 0x68, 0x30, 0x75, 0x00, 0x00 });
			sequence_patch((uint32_t)gd::base + 0x69902, { 0x3d, 0x30, 0x75, 0x00, 0x00 });
			sequence_patch((uint32_t)gd::base + 0x6b895, { 0x3d, 0x30, 0x75, 0x00, 0x00 });
			sequence_patch((uint32_t)gd::base + 0x6bbf9, { 0x3d, 0x30, 0x75, 0x00, 0x00 });
			sequence_patch((uint32_t)gd::base + 0xef1af, { 0x30, 0x75, 0x00, 0x00 });
		}

		if (setting().onPlaceOver) {
			sequence_patch((uint32_t)gd::base + 0xef5e1, { 0x8b, 0xc1, 0x90 });
			sequence_patch((uint32_t)gd::base + 0xef5f0, { 0xe9, 0x45, 0x02, 0x00, 0x00, 0x90 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0xef5e1, { 0x0f, 0x48, 0xc1 });
			sequence_patch((uint32_t)gd::base + 0xef5f0, { 0x0f, 0x8f, 0x44, 0x02, 0x00, 0x00 });
		}

		if (setting().onScaleSnapBypass) {
			sequence_patch((uint32_t)gd::base + 0x74869, { 0xeb, 0x26 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x74869, { 0x76, 0x26 });
		}

		if (setting().onSmoothTrail) {
			sequence_patch((uint32_t)gd::base + 0xfbd31, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0xfbd31, { 0x0f, 0x82, 0x7f, 0x00, 0x00, 0x00 });
		}

		if (setting().onToolboxBypass) {
			sequence_patch((uint32_t)gd::base + 0xd9f3b, { 0x83, 0xf9, 0x01 });
			sequence_patch((uint32_t)gd::base + 0xd9f40, { 0x83, 0xf9, 0x7f });
			sequence_patch((uint32_t)gd::base + 0xd9f45, { 0xb9, 0x7f, 0x00, 0x00, 0x00 });
			sequence_patch((uint32_t)gd::base + 0xd9f4c, { 0xb8, 0x01, 0x00, 0x00, 0x00 });
			sequence_patch((uint32_t)gd::base + 0xd9fcb, { 0x83, 0xf9, 0x01 });
			sequence_patch((uint32_t)gd::base + 0xd9fd0, { 0x83, 0xf9, 0x7f });
			sequence_patch((uint32_t)gd::base + 0xd9fd5, { 0xb9, 0x7f, 0x00, 0x00, 0x00 });
			sequence_patch((uint32_t)gd::base + 0xd9fdc, { 0xb8, 0x01, 0x00, 0x00, 0x00 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0xd9f3b, { 0x83, 0xf9, 0x06 });
			sequence_patch((uint32_t)gd::base + 0xd9f40, { 0x83, 0xf9, 0x0c });
			sequence_patch((uint32_t)gd::base + 0xd9f45, { 0xb9, 0x0c, 0x00, 0x00, 0x00 });
			sequence_patch((uint32_t)gd::base + 0xd9f4c, { 0xb8, 0x06, 0x00, 0x00, 0x00 });
			sequence_patch((uint32_t)gd::base + 0xd9fcb, { 0x83, 0xf9, 0x02 });
			sequence_patch((uint32_t)gd::base + 0xd9fd0, { 0x83, 0xf9, 0x03 });
			sequence_patch((uint32_t)gd::base + 0xd9fd5, { 0xb9, 0x03, 0x00, 0x00, 0x00 });
			sequence_patch((uint32_t)gd::base + 0xd9fdc, { 0xb8, 0x02, 0x00, 0x00, 0x00 });
		}

		if (setting().onVerifyHack) {
			sequence_patch((uint32_t)gd::base + 0x594e5, { 0xeb, 0x49 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x594e5, { 0x75, 0x49 });
		}

		if (setting().onZOrderBypass) {
			sequence_patch((uint32_t)gd::base + 0x18d9c1, { 0x90, 0x90, 0x90 });
			sequence_patch((uint32_t)gd::base + 0x18d9cb, { 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x18d9c1, { 0x0f, 0x4c, 0xc1 });
			sequence_patch((uint32_t)gd::base + 0x18d9cb, { 0x0f, 0x4f, 0xc1 });
		}

		if (setting().onZoomBypass) {
			sequence_patch((uint32_t)gd::base + 0x6b428, { 0x90, 0x90, 0x90 });
			sequence_patch((uint32_t)gd::base + 0x6b42d, { 0x90, 0x90, 0x90 });
			sequence_patch((uint32_t)gd::base + 0x6b4a8, { 0x90, 0x90, 0x90 });
			sequence_patch((uint32_t)gd::base + 0x6b4ad, { 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x6b428, { 0x0f, 0x2f, 0xc8 });
			sequence_patch((uint32_t)gd::base + 0x6b42d, { 0x0f, 0x28, 0xc8 });
			sequence_patch((uint32_t)gd::base + 0x6b4a8, { 0x0f, 0x2f, 0xc8 });
			sequence_patch((uint32_t)gd::base + 0x6b4ad, { 0x0f, 0x28, 0xc8 });
		}

		// Level

		if (setting().onConfirmExit) {
			sequence_patch((uint32_t)gd::base + 0x15d670, { 0x90, 0x90, 0x90, 0x90, 0x90 });
			sequence_patch((uint32_t)gd::base + 0x15d67d, { 0x90, 0x90 });
			sequence_patch((uint32_t)gd::base + 0x15d68d, { 0x90, 0x90, 0x90, 0x90, 0x90 });
			sequence_patch((uint32_t)gd::base + 0x15d695, { 0x90, 0x90, 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x15d670, { 0xe8, 0x0b, 0x08, 0x02, 0x00 });
			sequence_patch((uint32_t)gd::base + 0x15d67d, { 0x6a, 0x10 });
			sequence_patch((uint32_t)gd::base + 0x15d68d, { 0x68, 0x4c, 0x58, 0x5c, 0x00 });
			sequence_patch((uint32_t)gd::base + 0x15d695, { 0xe8, 0x86, 0xe7, 0xea, 0xff });
		}

		if (setting().onCorrectiveMusicSync) {
			sequence_patch((uint32_t)gd::base + 0x179cbe, { 0xeb, 0x08 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x179cbe, { 0x75, 0x08 });
		}

		if (setting().onHighFPSRotationFix)
			sequence_patch((uint32_t)gd::base + 0x16286b, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
		else
			sequence_patch((uint32_t)gd::base + 0x16286b, { 0x0f, 0x82, 0x21, 0x01, 0x00, 0x00 });

		if (setting().onPauseDuringComplete) {
			sequence_patch((uint32_t)gd::base + 0x16a146, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
			sequence_patch((uint32_t)gd::base + 0x16c87a, { 0x00 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x16a146, { 0x88, 0x81, 0x42, 0x03, 0x00, 0x00 });
			sequence_patch((uint32_t)gd::base + 0x16c87a, { 0x01 });
		}

		if (setting().onPracticeMusic) {
			sequence_patch((uint32_t)gd::base + 0x17b765, { 0x90, 0x90 });
			sequence_patch((uint32_t)gd::base + 0x17b797, { 0x90, 0x90 });
			sequence_patch((uint32_t)gd::base + 0x17d0f5, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
			sequence_patch((uint32_t)gd::base + 0x17d953, { 0x90, 0x90 });
			sequence_patch((uint32_t)gd::base + 0x17d981, { 0x90, 0x90, 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x17b765, { 0x75, 0x3e });
			sequence_patch((uint32_t)gd::base + 0x17b797, { 0x75, 0x0c });
			sequence_patch((uint32_t)gd::base + 0x17d0f5, { 0x0f, 0x85, 0x53, 0x02, 0x00, 0x00 });
			sequence_patch((uint32_t)gd::base + 0x17d953, { 0x75, 0x41 });
			sequence_patch((uint32_t)gd::base + 0x17d981, { 0xe8, 0x2a, 0x3f, 0xea, 0xff });
		}

		if (setting().onNoclip) cheatAdd();

		if (setting().onHitboxes) cheatAdd();

		if (setting().onShowLayout) cheatAdd();

		// Universal

		if (setting().onAllowLowVolume) {
			sequence_patch((uint32_t)gd::base + 0x15665e, { 0xeb, 0x08 });
			sequence_patch((uint32_t)gd::base + 0x15cebe, { 0xeb, 0x08 });
			sequence_patch((uint32_t)gd::base + 0x156500, { 0xeb, 0x08 });
			sequence_patch((uint32_t)gd::base + 0x15665e, { 0xeb, 0x08 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x15665e, { 0x76, 0x08 });
			sequence_patch((uint32_t)gd::base + 0x15cebe, { 0x76, 0x08 });
			sequence_patch((uint32_t)gd::base + 0x156500, { 0x76, 0x08 });
			sequence_patch((uint32_t)gd::base + 0x15665e, { 0x76, 0x08 });
		}

		if (setting().onAntiCheatBypass) {
			sequence_patch((uint32_t)gd::base + 0x170fc2, { 0x90, 0x90, 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x170fc2, { 0xe8, 0xb9, 0xce, 0x00, 0x00 });
		}

		if (setting().onFastAltTab)
			sequence_patch((uint32_t)gd::base + 0x38cae, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
		else
			sequence_patch((uint32_t)gd::base + 0x38cae, { 0x8b, 0x03, 0x8b, 0xcb, 0xff, 0x50, 0x18 });

		if (setting().onForceVisibility) {
			sequence_patch((uint32_t)libcocosbase + 0x60813, { 0xb0, 0x01, 0x90 });
			sequence_patch((uint32_t)libcocosbase + 0x60d2a, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
		}
		else {
			sequence_patch((uint32_t)libcocosbase + 0x60813, { 0x8a, 0x45, 0x08 });
			sequence_patch((uint32_t)libcocosbase + 0x60d2a, { 0x0f, 0x84, 0xcb, 0x00, 0x00, 0x00 });
		}

		if (setting().onFreeWindowResize) {
			sequence_patch((uint32_t)libcocosbase + 0x1110db, { 0x90, 0x90, 0x90, 0x90, 0x90 });
			sequence_patch((uint32_t)libcocosbase + 0x10fd93, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
			sequence_patch((uint32_t)libcocosbase + 0x110ad1, { 0xe9, 0x2f, 0xff, 0xff, 0xff, 0x90 });
		}
		else {
			sequence_patch((uint32_t)libcocosbase + 0x1110db, { 0xe8, 0xb0, 0xf3, 0xff, 0xff });
			//sequence_patch((uint32_t)libcocosbase + 0x10fd93, { 0xff, 0x15, 0x90, 0xa5, 0x8e, 0x55 });
			sequence_patch((uint32_t)libcocosbase + 0x110ad1, { 0x0f, 0x85, 0x2e, 0xff, 0xff, 0xff });
		}

		if (setting().onSafeMode) safeModeON();
		else safeModeOFF();

		if (setting().onShowRestartButton) {
			sequence_patch((uint32_t)gd::base + 0x15bc99, { 0x90, 0x90 });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x15bc99, { 0x75, 0x29 });
		}

		if (setting().onTransparentBG) {
			sequence_patch((uint32_t)gd::base + 0x40f17, { 0x90, 0xb1, 0xff }); // CreatorLayer
			sequence_patch((uint32_t)gd::base + 0x40f1f, { 0xff, 0xff });
			sequence_patch((uint32_t)gd::base + 0xea5fb, { 0x90, 0xb1, 0xff }); // LevelBrowserLayer
			sequence_patch((uint32_t)gd::base + 0xea602, { 0xff, 0xff });
			sequence_patch((uint32_t)gd::base + 0x10c1fd, { 0x90, 0xb1, 0xff }); // LevelSearchLayer
			sequence_patch((uint32_t)gd::base + 0x10c205, { 0xff, 0xff });
			sequence_patch((uint32_t)gd::base + 0x107933, { 0x90, 0xb1, 0xff }); // LevelInfoLayer
			sequence_patch((uint32_t)gd::base + 0x10793a, { 0xff, 0xff });
			sequence_patch((uint32_t)gd::base + 0x571a1, { 0x90, 0xb1, 0xff }); // EditLevelLayer
			sequence_patch((uint32_t)gd::base + 0x571a8, { 0xff, 0xff });
			sequence_patch((uint32_t)gd::base + 0xe8da2, { 0x90, 0xb1, 0xff }); // LeaderboardsLayer
			sequence_patch((uint32_t)gd::base + 0xe8daa, { 0xff, 0xff });
		}
		else {
			sequence_patch((uint32_t)gd::base + 0x40f17, { 0x80, 0xc9, 0xff });
			sequence_patch((uint32_t)gd::base + 0x40f1f, { 0x00, 0x66 });
			sequence_patch((uint32_t)gd::base + 0xea5fb, { 0x80, 0xc9, 0xff });
			sequence_patch((uint32_t)gd::base + 0xea602, { 0x00, 0x66 });
			sequence_patch((uint32_t)gd::base + 0x10c1fd, { 0x80, 0xc9, 0xff });
			sequence_patch((uint32_t)gd::base + 0x10c205, { 0x00, 0x66 });
			sequence_patch((uint32_t)gd::base + 0x107933, { 0x80, 0xc9, 0xff });
			sequence_patch((uint32_t)gd::base + 0x10793a, { 0x00, 0x66 });
			sequence_patch((uint32_t)gd::base + 0x571a1, { 0x80, 0xc9, 0xff });
			sequence_patch((uint32_t)gd::base + 0x571a8, { 0x00, 0x66 });
			sequence_patch((uint32_t)gd::base + 0xe8da2, { 0x80, 0xc9, 0xff });
			sequence_patch((uint32_t)gd::base + 0xe8daa, { 0x00, 0x66 });
		}

		auto* colors = ImGui::GetStyle().Colors;

		color1.x = setting().Overlaycolor[0];
		color1.y = setting().Overlaycolor[1];
		color1.z = setting().Overlaycolor[2];
		color1.w = setting().Overlaycolor[3] - 0.4f;

		color2.x = setting().Overlaycolor[0];
		color2.y = setting().Overlaycolor[1];
		color2.z = setting().Overlaycolor[2];
		color2.w = 1;

		color3.x = setting().Overlaycolor[0] + 0.3f;
		color3.y = setting().Overlaycolor[1] + 0.3f;
		color3.z = setting().Overlaycolor[2] + 0.3f;
		color3.w = setting().Overlaycolor[3] + 0.3f;

		color4.x = setting().Overlaycolor[0] - 0.1f;
		color4.y = setting().Overlaycolor[1] - 0.1f;
		color4.z = setting().Overlaycolor[2] - 0.1f;
		color4.w = setting().Overlaycolor[3] - 0.1f;

		color5.x = setting().Overlaycolor[0] + 0.1f;
		color5.y = setting().Overlaycolor[1] + 0.1f;
		color5.z = setting().Overlaycolor[2] + 0.1f;
		color5.w = setting().Overlaycolor[3] + 0.1f;

		color6.x = setting().BGcolor[0];
		color6.y = setting().BGcolor[1];
		color6.z = setting().BGcolor[2];
		color6.w = setting().BGcolor[3];

		//colors[ImGuiCol_Text] = ImVec4(1, 0, 0, 1);
		colors[ImGuiCol_WindowBg] = color6;
		colors[ImGuiCol_Border] = color2;
		colors[ImGuiCol_BorderShadow] = color2;
		colors[ImGuiCol_Button] = color3;
		colors[ImGuiCol_ButtonHovered] = color1;
		colors[ImGuiCol_ButtonActive] = color1;
		colors[ImGuiCol_CheckMark] = ImVec4(1, 1, 1, 1);
		colors[ImGuiCol_FrameBg] = color1;
		colors[ImGuiCol_FrameBgHovered] = color3;
		colors[ImGuiCol_FrameBgActive] = color3;
		colors[ImGuiCol_Tab] = ImVec4(color5.x, color5.y, color5.z, color5.w - 0.2f);
		colors[ImGuiCol_TabHovered] = color3;
		colors[ImGuiCol_TabActive] = color3;
		colors[ImGuiCol_TitleBg] = color3;
		colors[ImGuiCol_TitleBgActive] = color3;
		colors[ImGuiCol_TitleBgCollapsed] = color3;
		colors[ImGuiCol_SliderGrab] = color4;
		colors[ImGuiCol_SliderGrabActive] = color4;
		colors[ImGuiCol_TextSelectedBg] = color2;
		colors[ImGuiCol_Header] = color1;
		colors[ImGuiCol_HeaderHovered] = color1;
		colors[ImGuiCol_HeaderActive] = color2;
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0, 0, 0, 0);

		oneX = false;
	}

	if (setting().show) {
		auto playLayer = gd::GameManager::sharedState()->getPlayLayer();

		if (setting().explorer) {
			render_explorer_window(setting().explorer);
		}

		if (ImGui::Begin("PolzHax", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)); {
			ImGui::SetWindowFontScale(setting().UISize);

			ImGui::Text("2.011 - v1.0.2");

			ImGui::Checkbox("Auto Save", &setting().onAutoSave);
			ImGui::SameLine();
			if (ImGui::Button("Save")) {
				setting().saveState();
				gd::FLAlertLayer::create(nullptr, "Saved", "OK", nullptr, 300.f, "Your hack state is saved.")->show();
			}

			if (GetAsyncKeyState(0x31) && GetAsyncKeyState(0x37) && GetAsyncKeyState(0x30) && GetAsyncKeyState(0x33))
				ImGui::Checkbox("Developer Mode", &setting().onDeveloperMode);

			if (ImGui::Button("Cocos Explorer")) {
				setting().explorer = !setting().explorer;
			}

			if (ImGui::Button("AppData")) {
				ShellExecute(0, NULL, CCFileUtils::sharedFileUtils()->getWritablePath().c_str(), NULL, NULL, SW_SHOW);
			}

			char buffer[256];
			sprintf(buffer, "Extensions: %d", dllNames.size());
			if (ImGui::TreeNode(buffer)) {
				for (const auto& name : dllNames) {
					ImGui::Text(name.c_str());
				}
				if (ImGui::Button("Extensions Folder")) {
					ShellExecute(NULL, "open", "PolzHax\\extensions", NULL, NULL, SW_SHOWNORMAL);
				}
				ImGui::TreePop();
			}

			if (ImGui::Button("Inject DLL")) {
				auto selection = pfd::open_file("Select a file", CCFileUtils::sharedFileUtils()->getWritablePath2(), { "DLL File", "*.dll" }, pfd::opt::multiselect).result();
				for (auto const& filename : selection) {
					LoadLibrary(filename.c_str());
					std::filesystem::path path = filename;
					dllNames.push_back(path.filename().string());
				}
			}

			if (ImGui::Button("Sort Tabs")) {
				float polzhax_xPos;
				float bypass_xPos;
				float cosmetic_xPos;
				float creator_xPos;
				float level_xPos;
				float universal_xPos;
				float addingSpeedhackY;

				if (ImGui::Begin("PolzHax", nullptr)) {
					ImGui::SetWindowPos({ 5,5 });
					polzhax_xPos = ImGui::GetWindowWidth() + 10;
				}

				if (ImGui::Begin("Bypass", nullptr)) {
					ImGui::SetWindowPos({ polzhax_xPos, 5 });
					bypass_xPos = polzhax_xPos + ImGui::GetWindowWidth() + 5;
				}

				if (ImGui::Begin("Cosmetic", nullptr)) {
					ImGui::SetWindowPos({ bypass_xPos, 5 });
					cosmetic_xPos = bypass_xPos + ImGui::GetWindowWidth() + 5;
				}

				if (ImGui::Begin("Creator", nullptr)) {
					ImGui::SetWindowPos({ cosmetic_xPos, 5 });
					creator_xPos = cosmetic_xPos + ImGui::GetWindowWidth() + 5;
				}

				if (ImGui::Begin("Level", nullptr)) {
					ImGui::SetWindowPos({ creator_xPos, 5 });
					level_xPos = creator_xPos + ImGui::GetWindowWidth() + 5;
				}

				if (ImGui::Begin("Universal", nullptr)) {
					ImGui::SetWindowPos({ level_xPos, 5 });
					universal_xPos = level_xPos + ImGui::GetWindowWidth() + 5;
					addingSpeedhackY = ImGui::GetWindowHeight() + 10;
				}

				if (ImGui::Begin("Speedhack", nullptr)) {
					ImGui::SetWindowPos({ level_xPos, addingSpeedhackY });
				}
			}
		}

		if (ImGui::Begin("Bypass", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)); {
			ImGui::SetWindowFontScale(setting().UISize);

			ImGui::Checkbox("Character Filter", &setting().onCharFilter);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Lets you input any character in text fields.");

			if (ImGui::Checkbox("Guard Vault", &setting().onTheVault)) {
				if (setting().onTheVault) {
					sequence_patch((uint32_t)gd::base + 0x1567fb, { 0xe9, 0x72, 0x01, 0x00, 0x00, 0x90 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x1567fb, { 0x0f, 0x85, 0x71, 0x01, 0x00, 0x00 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Unlocks the guard's vault.");

			ImGui::Checkbox("Icons", &setting().onIcons);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Unlocks all icons and colors.");

			if (ImGui::Checkbox("Load Failed", &setting().onLoadFailed)) {
				if (setting().onLoadFailed) {
					sequence_patch((uint32_t)gd::base + 0x117908, { 0xe9, 0x13, 0x01, 0x00, 0x00, 0x90 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x117908, { 0x0f, 0x87, 0x12, 0x01, 0x00, 0x00 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Bypasses load failed for modified main levels.");

			if (ImGui::Checkbox("Main Levels", &setting().onMainLevels)) {
				if (setting().onMainLevels) {
					sequence_patch((uint32_t)gd::base + 0x11510b, { 0xe9, 0xa7, 0x02, 0x00, 0x00, 0x90 });
					sequence_patch((uint32_t)gd::base + 0x115b82, { 0xe9, 0x98, 0x00, 0x00, 0x00, 0x90 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x11510b, { 0x0f, 0x84, 0xa6, 0x02, 0x00, 0x00 });
					sequence_patch((uint32_t)gd::base + 0x115b82, { 0x0f, 0x8e, 0x97, 0x00, 0x00, 0x00 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Unlocks main demon levels.");

			if (ImGui::Checkbox("Slider Limit", &setting().onSliderLimit)) {
				if (setting().onSliderLimit) {
					sequence_patch((uint32_t)gd::base + 0x2a567, { 0xeb, 0x07 });
					sequence_patch((uint32_t)gd::base + 0x2a57b, { 0xeb, 0x07 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x2a567, { 0x76, 0x07 });
					sequence_patch((uint32_t)gd::base + 0x2a57b, { 0x76, 0x07 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Lets sliders be dragged beyond the visible limit.");

			ImGui::Checkbox("Text Length", &setting().onTextLength);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Allows for unlimited text length in text inputs.");
		}

		if (ImGui::Begin("Cosmetic", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)); {
			ImGui::SetWindowFontScale(setting().UISize);

			if (ImGui::Checkbox("Force Don't Enter", &setting().onForceDontEnter)) {
				if (setting().onForceDontEnter) {
					sequence_patch((uint32_t)gd::base + 0x177926, { 0xe9, 0xe4, 0x01, 0x00, 0x00, 0x90 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x177926, { 0x0f, 0x85, 0xe3, 0x01, 0x00, 0x00 });
				}
			}

			if (ImGui::Checkbox("Force Don't Fade", &setting().onForceDontFade)) {
				if (setting().onForceDontFade) {
					sequence_patch((uint32_t)gd::base + 0x1770de, { 0xe9, 0x83, 0x00, 0x00, 0x00, 0x90 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x1770de, { 0x0f, 0x85, 0x82, 0x00, 0x00, 0x00 });
				}
			}

			if (ImGui::Checkbox("Hide Attempts", &setting().onHideAttempts)) {
				if (playLayer)
					playLayer->m_attemptLabel->setVisible(!setting().onHideAttempts);
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Hides the attempts counter in-game.");

			if (ImGui::Checkbox("Hide Pause Button", &setting().onHidePauseButton)) {
				if (playLayer) {
					if (gd::GameManager::sharedState()->getGameVariable("0024")) {
						from<gd::CCMenuItemSpriteExtra*>(playLayer->m_uiLayer, 0x1a0)->setVisible(!setting().onHidePauseButton);
					}
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Hides the pause button when the in-game cursor is enabled.");

			if (ImGui::Checkbox("Hide Pause Menu", &setting().onHidePauseMenu)) {
				if (layers().PauseLayerObject && playLayer)
					layers().PauseLayerObject->setVisible(!setting().onHidePauseMenu);
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Hides the pause menu.");

			if (ImGui::Checkbox("Hide Player", &setting().onHidePlayer)) {
				if (playLayer) {
					playLayer->m_player->setVisible(!setting().onHidePlayer);
					playLayer->m_player2->setVisible(!setting().onHidePlayer);
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Hides the player.");

			if (ImGui::Checkbox("Hide Practice Buttons", &setting().onHidePracticeButtons)) {
				if (playLayer && playLayer->m_uiLayer && playLayer->m_uiLayer->m_checkpointMenu) playLayer->m_uiLayer->m_checkpointMenu->setVisible(!setting().onHidePracticeButtons);
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Hides practice buttons.");

			if (ImGui::Checkbox("Instant Mirror", &setting().onInstantMirror)) {
				if (setting().onInstantMirror) {
					sequence_patch((uint32_t)gd::base + 0x17bdf6, { 0x00, 0x00, 0x00, 0x00 });
					cheatAdd();
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x17bdf6, { 0x00, 0x00, 0x00, 0x3f });
					cheatDec();
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables the mirror portal animation.");

			if (ImGui::Checkbox("Inversed Trail", &setting().onInversedTrail)) {
				if (setting().onInversedTrail) {
					sequence_patch((uint32_t)libcocosbase + 0xad5b6, { 0x0f, 0x85, 0x55, 0x02, 0x00, 0x00 });
				}
				else {
					sequence_patch((uint32_t)libcocosbase + 0xad5b6, { 0x0f, 0x84, 0x55, 0x02, 0x00, 0x00 });
				}
			}

			if (ImGui::Checkbox("No Background Flash", &setting().onNoBackgroundFlash)) {
				if (setting().onNoBackgroundFlash) {
					sequence_patch((uint32_t)gd::base + 0x16e331, { 0x6a, 0x00 });
					sequence_patch((uint32_t)gd::base + 0x16e34b, { 0xc7, 0x04, 0x24, 0x00, 0x00, 0x00, 0x00 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x16e331, { 0x6a, 0x01 });
					sequence_patch((uint32_t)gd::base + 0x16e34b, { 0xc7, 0x04, 0x24, 0x8f, 0xc2, 0xf5, 0x3d });
				}
			}

			if (ImGui::Checkbox("No Death Effect", &setting().onNoDeathEffect)) {
				if (setting().onNoDeathEffect) {
					sequence_patch((uint32_t)gd::base + 0x164c2d, { 0xe9, 0xe7, 0x01, 0x00, 0x00, 0x90 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x164c2d, { 0x0f, 0x84, 0xd5, 0x01, 0x00, 0x00 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("No visual effects on death.");

			ImGui::Checkbox("No Effect Circle", &setting().onNoEffectCircle);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Removes effect circles from orb, portal & pad activations.");

			if (ImGui::Checkbox("No Ghost Trail", &setting().onNoGhostEffect)) {
				if (setting().onNoGhostEffect) {
					sequence_patch((uint32_t)gd::base + 0x17d664, { 0x6a, 0x00, 0x90 });
					sequence_patch((uint32_t)gd::base + 0x17d67d, { 0x6a, 0x00, 0x90 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x17d664, { 0xff, 0x75, 0x08 });
					sequence_patch((uint32_t)gd::base + 0x17d67d, { 0xff, 0x75, 0x08 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables player ghost trail triggers.");

			if (ImGui::Checkbox("No Glow", &setting().onNoGlow)) {
				if (setting().onNoGlow)
					sequence_patch((uint32_t)gd::base + 0xa4442, { 0xe9, 0x91, 0x01, 0x00, 0x00, 0x90 });
				else
					sequence_patch((uint32_t)gd::base + 0xa4442, { 0x0f, 0x85, 0x90, 0x01, 0x00, 0x00 });
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables objects glow.");

			if (ImGui::Checkbox("No Gravity Effect", &setting().onNoGravityEffect)) {
				if (setting().onNoGravityEffect) {
					sequence_patch((uint32_t)gd::base + 0x17e6f6, { 0x90, 0x90 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x17e6f6, { 0x75, 0x07 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables gravity effect.");

			ImGui::Checkbox("No Mini Icon", &setting().onNoMiniIcon);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Replaces default mini icon with normal one.");

			if (ImGui::Checkbox("No Mirror", &setting().onNoMirror)) {
				if (setting().onNoMirror) {
					sequence_patch((uint32_t)gd::base + 0x17bcb2, { 0xe9, 0x9b, 0x01, 0x00, 0x00, 0x90 });
					cheatAdd();
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x17bcb2, { 0x0f, 0x84, 0x9a, 0x01, 0x00, 0x00 });
					cheatDec();
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables mirror portals.");

			ImGui::Checkbox("No Orb Ring", &setting().onNoOrbRing);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables orb ring effect when touching it.");

			ImGui::Checkbox("No Particles", &setting().onNoParticles);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables the particle system.");

			if (ImGui::Checkbox("No Portal Lightning", &setting().onNoLightning)) {
				if (setting().onNoLightning) {
					sequence_patch((uint32_t)gd::base + 0x16e28f, { 0xe9, 0x97, 0x00, 0x00, 0x00, 0x90 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x16e28f, { 0x0f, 0x85, 0x96, 0x00, 0x00, 0x00 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables portal lightning from size-changing portals.");

			ImGui::Checkbox("No Pulse", &setting().onNoPulse);

			if (ImGui::Checkbox("No Respawn Flash", &setting().onNoRespawnFlash)) {
				if (setting().onNoRespawnFlash) {
					sequence_patch((uint32_t)gd::base + 0x1643a1, { 0xe9, 0x9f, 0x00, 0x00, 0x00, 0x00 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x1643a1, { 0x0f, 0x85, 0x9e, 0x00, 0x00, 0x00 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables the respawn flash & circle effect.");

			ImGui::Checkbox("No Wave Pulse", &setting().onNoWavePulse);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables wave trail pulsing.");

			ImGui::Checkbox("No Wave Trail Behind", &setting().onNoWaveTrailBehind);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables default player trail behind the wave trail.");

			if (ImGui::Checkbox("Practice Pulse", &setting().onPracticePulse)) {
				if (setting().onPracticePulse) {
					sequence_patch((uint32_t)gd::base + 0x39805, { 0x90, 0x90 });
					sequence_patch((uint32_t)gd::base + 0x17655a, { 0xeb, 0x16 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x39805, { 0x75, 0x0c });
					sequence_patch((uint32_t)gd::base + 0x17655a, { 0x74, 0x16 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Pulses orbs & wave in practice mode.");

			if (ImGui::Checkbox("Solid Wave Trail", &setting().onSolidWaveTrail)) {
				if (setting().onSolidWaveTrail) {
					sequence_patch((uint32_t)gd::base + 0x15f46b, { 0x90, 0x90 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x15f46b, { 0x75, 0x0c });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("No blending on the wave trail.");

			if (ImGui::Checkbox("Trail Always Off", &setting().onTrailAlwaysOff)) {
				if (setting().onTrailAlwaysOff) {
					sequence_patch((uint32_t)libcocosbase + 0xad5b6, { 0xe9, 0x56, 0x02, 0x00, 0x00, 0x90 });
				}
				else {
					sequence_patch((uint32_t)libcocosbase + 0xad5b6, { 0x0f, 0x84, 0x55, 0x02, 0x00, 0x00 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Forces player trail to be always off.");

			if (ImGui::Checkbox("Trail Always On", &setting().onTrailAlwaysOn)) {
				if (setting().onTrailAlwaysOn) {
					sequence_patch((uint32_t)libcocosbase + 0xad5b6, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
				}
				else {
					sequence_patch((uint32_t)libcocosbase + 0xad5b6, { 0x0f, 0x84, 0x55, 0x02, 0x00, 0x00 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Forces player trail to be always on.");

			if (ImGui::Checkbox("Trail Bug Fix", &setting().onTrailBugFix)) {
				if (setting().onTrailBugFix) {
					sequence_patch((uint32_t)libcocosbase + 0xad1ad, { 0xbb, 0xff, 0x00, 0x00, 0x00, 0x90 });
				}
				else {
					sequence_patch((uint32_t)libcocosbase + 0xad1ad, { 0xf3, 0x0f, 0x2c, 0xc1, 0x2b, 0xd8 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Fixes trail cutting on high refresh rates.");

			ImGui::Checkbox("Wave Pulse Size", &setting().onWavePulseSize);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Changes the wave pulse size multiplier.");
			ImGui::SameLine();
			if (ImGui::TreeNode("##wavePulseSize")) {

				ImGui::SetNextItemWidth(setting().UISize * 120.f);
				ImGui::DragFloat("##waveSize", &setting().wavePulseSize, .1f, .1f, 2.3f, "%.1fx");

				ImGui::TreePop();
			}
		}

		if (ImGui::Begin("Creator", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)); {
			ImGui::SetWindowFontScale(setting().UISize);

			if (ImGui::Checkbox("Absolute Position", &setting().onAbsolutePosition)) {
				if (setting().onAbsolutePosition) {
					sequence_patch((uint32_t)gd::base + 0x6e3bd, { 0x90, 0x8b, 0xce, 0x90, 0x90, 0x90 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x6e3bd, { 0x51, 0x8b, 0xce, 0xff, 0x50, 0x5c });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Locks block position for rotation.");

			if (ImGui::Checkbox("Copy Hack", &setting().onCopyHack)) {
				if (setting().onCopyHack) {
					sequence_patch((uint32_t)gd::base + 0x10841f, { 0x8b, 0xce, 0x90 });
					sequence_patch((uint32_t)gd::base + 0x1084b4, { 0xb0, 0x01, 0x90 });
					sequence_patch((uint32_t)gd::base + 0x109620, { 0xeb, 0x18 });
					sequence_patch((uint32_t)gd::base + 0x10a079, { 0x90, 0x90 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x10841f, { 0x0f, 0x44, 0xce });
					sequence_patch((uint32_t)gd::base + 0x1084b4, { 0x0f, 0x95, 0xc0 });
					sequence_patch((uint32_t)gd::base + 0x109620, { 0x74, 0x18 });
					sequence_patch((uint32_t)gd::base + 0x10a079, { 0x75, 0x11 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Lets you copy any level, without a password.");

			if (ImGui::Checkbox("Custom Object Bypass", &setting().onCustomObjectBypass)) {
				if (setting().onCustomObjectBypass) {
					sequence_patch((uint32_t)gd::base + 0x607c1, { 0xeb, 0x7f });
					sequence_patch((uint32_t)gd::base + 0x608a3, { 0xe9, 0xaf, 0x00, 0x00, 0x00, 0x90 });
					sequence_patch((uint32_t)gd::base + 0x609cf, { 0x90, 0x90 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x607c1, { 0x76, 0x7f });
					sequence_patch((uint32_t)gd::base + 0x608a3, { 0x0f, 0x82, 0xae, 0x00, 0x00, 0x00 });
					sequence_patch((uint32_t)gd::base + 0x609cf, { 0x74, 0x4f });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Removes the object limit for custom objects & lets you save over 100.");

			if (ImGui::Checkbox("Default Song Bypass", &setting().onDefaultSongBypass)) {
				if (setting().onDefaultSongBypass) {
					sequence_patch((uint32_t)gd::base + 0x10560f, { 0x90, 0x90 });
					sequence_patch((uint32_t)gd::base + 0x105621, { 0x90, 0x90, 0x90 });
					sequence_patch((uint32_t)gd::base + 0x10568e, { 0x90, 0x90 });
					sequence_patch((uint32_t)gd::base + 0x1056a0, { 0x90, 0x90, 0x90 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x10560f, { 0x74, 0x4e });
					sequence_patch((uint32_t)gd::base + 0x105621, { 0x0f, 0x4f, 0xf0 });
					sequence_patch((uint32_t)gd::base + 0x10568e, { 0x74, 0x4e });
					sequence_patch((uint32_t)gd::base + 0x1056a0, { 0x0f, 0x4f, 0xf0 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Lets you use hidden default songs in the editor.");

			if (ImGui::Checkbox("Editor Extension", &setting().onEditorExtension)) {
				if (setting().onEditorExtension) {
					sequence_patch((uint32_t)gd::base + 0x20cf68, { 0x00, 0xc8, 0x2f, 0x4b });
					sequence_patch((uint32_t)gd::base + 0x6fdb9, { 0x00, 0xd7, 0x2f, 0x4b });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x20cf68, { 0x00, 0xc8, 0xaf, 0x47 });
					sequence_patch((uint32_t)gd::base + 0x6fdb9, { 0x00, 0xd7, 0xaf, 0x47 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Increases the editor length by a factor of 128.");

			if (ImGui::Checkbox("Free Scroll", &setting().onFreeScroll)) {
				if (setting().onFreeScroll) {
					sequence_patch((uint32_t)gd::base + 0x6fe05, { 0xeb, 0x08 });
					sequence_patch((uint32_t)gd::base + 0x6fe1c, { 0xeb, 0x05 });
					sequence_patch((uint32_t)gd::base + 0x6fe35, { 0xeb, 0x08 });
					sequence_patch((uint32_t)gd::base + 0x6fe4c, { 0xeb, 0x05 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x6fe05, { 0x77, 0x08 });
					sequence_patch((uint32_t)gd::base + 0x6fe1c, { 0x77, 0x05 });
					sequence_patch((uint32_t)gd::base + 0x6fe35, { 0x77, 0x08 });
					sequence_patch((uint32_t)gd::base + 0x6fe4c, { 0x77, 0x05 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Allows you to scroll out the editor.");

			if (ImGui::Checkbox("Hide Preview Line", &setting().onHidePreviewLine)) {
				if (setting().onHidePreviewLine) {
					sequence_patch((uint32_t)gd::base + 0xff890, { 0xe9, 0x04, 0x01, 0x00, 0x00, 0x90 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0xff890, { 0x0f, 0x84, 0x03, 0x01, 0x00, 0x00 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Removes center line in editor while in preview mode.");

			if (ImGui::Checkbox("Hide UI", &setting().onHideUI)) {
				if (gd::GameManager::sharedState()->getLevelEditorLayer() && gd::GameManager::sharedState()->getLevelEditorLayer()->m_editorUI)
					gd::GameManager::sharedState()->getLevelEditorLayer()->m_editorUI->setVisible(!setting().onHideUI);
			}

			ImGui::Checkbox("Hitbox Bug Fix", &setting().onHitboxBugFix);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Fixes oriented hitboxes in the editor.");

			if (ImGui::Checkbox("Level Edit", &setting().onLevelEdit)) {
				if (setting().onLevelEdit) {
					sequence_patch((uint32_t)gd::base + 0x15baaf, { 0x90, 0x90 });
					sequence_patch((uint32_t)gd::base + 0x75757, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x15baaf, { 0x75, 0x62 });
					sequence_patch((uint32_t)gd::base + 0x75757, { 0x0f, 0x85, 0xca, 0x00, 0x00, 0x00 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Lets you edit any level through the pause menu.");

			if (ImGui::Checkbox("No (C) Mark", &setting().onNoCMark)) {
				if (setting().onNoCMark) {
					sequence_patch((uint32_t)gd::base + 0x7d0e0, { 0xb8, 0x00, 0x00, 0x00, 0x00, 0x90 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x7d0e0, { 0x8b, 0x81, 0x18, 0x02, 0x00, 0x00 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Removes the (C) mark when uploading copied levels.");

			if (ImGui::Checkbox("No Death X", &setting().onNoDeathX)) {
				if (setting().onNoDeathX) {
					sequence_patch((uint32_t)gd::base + 0xee33a, { 0x00, 0x00, 0x00, 0x00 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0xee33a, { 0x33, 0x33, 0x33, 0x3f });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Removes big annoying red X when dying whilst playtesting.");

			if (ImGui::Checkbox("Object Bypass", &setting().onObjectBypass)) {
				if (setting().onObjectBypass) {
					sequence_patch((uint32_t)gd::base + 0x5a727, { 0x68, 0xff, 0xff, 0xff, 0x7f });
					sequence_patch((uint32_t)gd::base + 0x69902, { 0x3d, 0xff, 0xff, 0xff, 0x7f });
					sequence_patch((uint32_t)gd::base + 0x6b895, { 0x3d, 0xff, 0xff, 0xff, 0x7f });
					sequence_patch((uint32_t)gd::base + 0x6bbf9, { 0x3d, 0xff, 0xff, 0xff, 0x7f });
					sequence_patch((uint32_t)gd::base + 0xef1af, { 0xff, 0xff, 0xff, 0x7f });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x5a727, { 0x68, 0x30, 0x75, 0x00, 0x00 });
					sequence_patch((uint32_t)gd::base + 0x69902, { 0x3d, 0x30, 0x75, 0x00, 0x00 });
					sequence_patch((uint32_t)gd::base + 0x6b895, { 0x3d, 0x30, 0x75, 0x00, 0x00 });
					sequence_patch((uint32_t)gd::base + 0x6bbf9, { 0x3d, 0x30, 0x75, 0x00, 0x00 });
					sequence_patch((uint32_t)gd::base + 0xef1af, { 0x30, 0x75, 0x00, 0x00 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Uncaps the object limit.");

			if (ImGui::Checkbox("Place Over", &setting().onPlaceOver)) {
				if (setting().onPlaceOver) {
					sequence_patch((uint32_t)gd::base + 0xef5e1, { 0x8b, 0xc1, 0x90 });
					sequence_patch((uint32_t)gd::base + 0xef5f0, { 0xe9, 0x45, 0x02, 0x00, 0x00, 0x90 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0xef5e1, { 0x0f, 0x48, 0xc1 });
					sequence_patch((uint32_t)gd::base + 0xef5f0, { 0x0f, 0x8f, 0x44, 0x02, 0x00, 0x00 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Lets you place the same object over itself in the editor.");

			if (ImGui::Checkbox("Scale Snap Bypass", &setting().onScaleSnapBypass)) {
				if (setting().onScaleSnapBypass) {
					sequence_patch((uint32_t)gd::base + 0x74869, { 0xeb, 0x26 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x74869, { 0x76, 0x26 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Removed snapping to 1.00 when moving the scale slider between 0.97 and 1.03.");

			if (ImGui::Checkbox("Smooth Trail", &setting().onSmoothTrail)) {
				if (setting().onSmoothTrail) {
					sequence_patch((uint32_t)gd::base + 0xfbd31, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0xfbd31, { 0x0f, 0x82, 0x7f, 0x00, 0x00, 0x00 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Makes the playtest line in the editor smoother by updating it every frame instead of at 30fps.");


			if (ImGui::Checkbox("Toolbox Button Bypass", &setting().onToolboxBypass)) {
				if (setting().onToolboxBypass) {
					sequence_patch((uint32_t)gd::base + 0xd9f3b, { 0x83, 0xf9, 0x01 });
					sequence_patch((uint32_t)gd::base + 0xd9f40, { 0x83, 0xf9, 0x7f });
					sequence_patch((uint32_t)gd::base + 0xd9f45, { 0xb9, 0x7f, 0x00, 0x00, 0x00 });
					sequence_patch((uint32_t)gd::base + 0xd9f4c, { 0xb8, 0x01, 0x00, 0x00, 0x00 });
					sequence_patch((uint32_t)gd::base + 0xd9fcb, { 0x83, 0xf9, 0x01 });
					sequence_patch((uint32_t)gd::base + 0xd9fd0, { 0x83, 0xf9, 0x7f });
					sequence_patch((uint32_t)gd::base + 0xd9fd5, { 0xb9, 0x7f, 0x00, 0x00, 0x00 });
					sequence_patch((uint32_t)gd::base + 0xd9fdc, { 0xb8, 0x01, 0x00, 0x00, 0x00 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0xd9f3b, { 0x83, 0xf9, 0x06 });
					sequence_patch((uint32_t)gd::base + 0xd9f40, { 0x83, 0xf9, 0x0c });
					sequence_patch((uint32_t)gd::base + 0xd9f45, { 0xb9, 0x0c, 0x00, 0x00, 0x00 });
					sequence_patch((uint32_t)gd::base + 0xd9f4c, { 0xb8, 0x06, 0x00, 0x00, 0x00 });
					sequence_patch((uint32_t)gd::base + 0xd9fcb, { 0x83, 0xf9, 0x02 });
					sequence_patch((uint32_t)gd::base + 0xd9fd0, { 0x83, 0xf9, 0x03 });
					sequence_patch((uint32_t)gd::base + 0xd9fd5, { 0xb9, 0x03, 0x00, 0x00, 0x00 });
					sequence_patch((uint32_t)gd::base + 0xd9fdc, { 0xb8, 0x02, 0x00, 0x00, 0x00 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Allows for more objects in the editor toolbox.");

			if (ImGui::Checkbox("Verify Hack", &setting().onVerifyHack)) {
				if (setting().onVerifyHack) {
					sequence_patch((uint32_t)gd::base + 0x594e5, { 0xeb, 0x49 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x594e5, { 0x75, 0x49 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Lets you upload unverified levels.");

			if (ImGui::Checkbox("Z Order Bypass", &setting().onZOrderBypass)) {
				if (setting().onZOrderBypass) {
					sequence_patch((uint32_t)gd::base + 0x18d9c1, { 0x90, 0x90, 0x90 });
					sequence_patch((uint32_t)gd::base + 0x18d9cb, { 0x90, 0x90, 0x90 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x18d9c1, { 0x0f, 0x4c, 0xc1 });
					sequence_patch((uint32_t)gd::base + 0x18d9cb, { 0x0f, 0x4f, 0xc1 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Removes the -100 to 100 Z order range limit.");

			if (ImGui::Checkbox("Zoom Bypass", &setting().onZoomBypass)) {
				if (setting().onZoomBypass) {
					sequence_patch((uint32_t)gd::base + 0x6b428, { 0x90, 0x90, 0x90 });
					sequence_patch((uint32_t)gd::base + 0x6b42d, { 0x90, 0x90, 0x90 });
					sequence_patch((uint32_t)gd::base + 0x6b4a8, { 0x90, 0x90, 0x90 });
					sequence_patch((uint32_t)gd::base + 0x6b4ad, { 0x90, 0x90, 0x90 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x6b428, { 0x0f, 0x2f, 0xc8 });
					sequence_patch((uint32_t)gd::base + 0x6b42d, { 0x0f, 0x28, 0xc8 });
					sequence_patch((uint32_t)gd::base + 0x6b4a8, { 0x0f, 0x2f, 0xc8 });
					sequence_patch((uint32_t)gd::base + 0x6b4ad, { 0x0f, 0x28, 0xc8 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Lets you zoom fully in & out. (NOTE: Can crash with an edited grid size)");
		}

		if (ImGui::Begin("Level", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)); {
			ImGui::SetWindowFontScale(setting().UISize);

			ImGui::Checkbox("Ball Rotation Bug Fix", &setting().onBallRotatingBugFix);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Fixes that ball rotation bug when entering a portal mid ball animation.");

			//ImGui::Checkbox("Checkpoint Lag Fix", &setting().onCheckpointLagFix);

			if (ImGui::Checkbox("Confirm Exit", &setting().onConfirmExit)) {
				if (setting().onConfirmExit) {
					sequence_patch((uint32_t)gd::base + 0x15d670, { 0x90, 0x90, 0x90, 0x90, 0x90 });
					sequence_patch((uint32_t)gd::base + 0x15d67d, { 0x90, 0x90 });
					sequence_patch((uint32_t)gd::base + 0x15d68d, { 0x90, 0x90, 0x90, 0x90, 0x90 });
					sequence_patch((uint32_t)gd::base + 0x15d695, { 0x90, 0x90, 0x90, 0x90, 0x90 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x15d670, { 0xe8, 0x0b, 0x08, 0x02, 0x00 });
					sequence_patch((uint32_t)gd::base + 0x15d67d, { 0x6a, 0x10 });
					sequence_patch((uint32_t)gd::base + 0x15d68d, { 0x68, 0x4c, 0x58, 0x5c, 0x00 });
					sequence_patch((uint32_t)gd::base + 0x15d695, { 0xe8, 0x86, 0xe7, 0xea, 0xff });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Requires confirmation when exiting a level.");

			if (ImGui::Checkbox("Corrective Music Sync", &setting().onCorrectiveMusicSync)) {
				if (setting().onCorrectiveMusicSync) {
					sequence_patch((uint32_t)gd::base + 0x179cbe, { 0xeb, 0x08 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x179cbe, { 0x75, 0x08 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Syncs music to checked speed-portals, instead of only ones the player hit.");

			if (ImGui::Checkbox("High FPS Rotation Fix", &setting().onHighFPSRotationFix)) {
				if (setting().onHighFPSRotationFix)
					sequence_patch((uint32_t)gd::base + 0x16286b, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
				else
					sequence_patch((uint32_t)gd::base + 0x16286b, { 0x0f, 0x82, 0x21, 0x01, 0x00, 0x00 });
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Fixes ship/ufo/wave rotation on high fps, affects hitboxes.");

			ImGui::Checkbox("Invisible Dual Fix", &setting().onInvisibleDualFix);

			if (ImGui::Checkbox("Hitboxes", &setting().onHitboxes)) {
				if (setting().onHitboxes) {
					cheatAdd();
				}
				else {
					cheatDec();
				}
			}
			//ImGui::SameLine();
			//if (ImGui::ArrowButton("##hit", 1))
			//	ImGui::OpenPopup("Hitboxes Settings");

			//if (ImGui::BeginPopup("Hitboxes Settings", ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoFocusOnAppearing)) {

			//	ImGui::Text("Popup");

			//	ImGui::EndPopup();
			//}

			if (ImGui::TreeNode("Hitboxes Settings")) {
				ImGui::SetNextItemWidth(setting().UISize * 60.f);
				ImGui::DragInt("Opacity", &setting().hitboxesOpacity, 1.f, 0, 255);

				ImGui::Checkbox("Player", &setting().onEnablePlayerHitboxes);

				ImGui::Checkbox("Solids", &setting().onEnableSolidHitboxes);
				ImGui::SameLine();
				if (ImGui::ColorEdit3("##solidsColor", solidsColor, ImGuiColorEditFlags_NoInputs)) {
					setting().solidHitboxesR = solidsColor[0] * 255;
					setting().solidHitboxesG = solidsColor[1] * 255;
					setting().solidHitboxesB = solidsColor[2] * 255;
				}

				ImGui::Checkbox("Hazards", &setting().onEnableHazardHitboxes);
				ImGui::SameLine();
				if (ImGui::ColorEdit3("##hazardsColor", hazardsColor, ImGuiColorEditFlags_NoInputs)) {
					setting().hazardHitboxesR = hazardsColor[0] * 255;
					setting().hazardHitboxesG = hazardsColor[1] * 255;
					setting().hazardHitboxesB = hazardsColor[2] * 255;
				}

				ImGui::Checkbox("Specials", &setting().onEnableSpecialHitboxes);
				ImGui::SameLine();
				if (ImGui::ColorEdit3("##specialsColor", specialsColor, ImGuiColorEditFlags_NoInputs)) {
					setting().specialHitboxesR = specialsColor[0] * 255;
					setting().specialHitboxesG = specialsColor[1] * 255;
					setting().specialHitboxesB = specialsColor[2] * 255;
				}

				ImGui::TreePop();
			}

			ImGui::Checkbox("Hitboxes On Death", &setting().onHitboxesOnDeath);

			if (ImGui::Checkbox("Noclip", &setting().onNoclip)) {
				if (setting().onNoclip) {
					cheatAdd();
				}
				else {
					cheatDec();
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Makes the player invincible.");

			if (ImGui::Checkbox("Pause During Completion", &setting().onPauseDuringComplete)) {
				if (setting().onPauseDuringComplete) {
					sequence_patch((uint32_t)gd::base + 0x16a146, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
					sequence_patch((uint32_t)gd::base + 0x16c87a, { 0x00 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x16a146, { 0x88, 0x81, 0x42, 0x03, 0x00, 0x00 });
					sequence_patch((uint32_t)gd::base + 0x16c87a, { 0x01 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Lets you pause during the level complete animation.");

			ImGui::Checkbox("Practice Bug Fix", &setting().onPracticeBugFix);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Saves & restores player velocity in practice mode.");

			if (ImGui::Checkbox("Practice Music", &setting().onPracticeMusic)) {
				if (setting().onPracticeMusic) {
					sequence_patch((uint32_t)gd::base + 0x17b765, { 0x90, 0x90 });
					sequence_patch((uint32_t)gd::base + 0x17b797, { 0x90, 0x90 });
					sequence_patch((uint32_t)gd::base + 0x17d0f5, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
					sequence_patch((uint32_t)gd::base + 0x17d953, { 0x90, 0x90 });
					sequence_patch((uint32_t)gd::base + 0x17d981, { 0x90, 0x90, 0x90, 0x90, 0x90 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x17b765, { 0x75, 0x3e });
					sequence_patch((uint32_t)gd::base + 0x17b797, { 0x75, 0x0c });
					sequence_patch((uint32_t)gd::base + 0x17d0f5, { 0x0f, 0x85, 0x53, 0x02, 0x00, 0x00 });
					sequence_patch((uint32_t)gd::base + 0x17d953, { 0x75, 0x41 });
					sequence_patch((uint32_t)gd::base + 0x17d981, { 0xe8, 0x2a, 0x3f, 0xea, 0xff });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Plays the level's song in-sync with your position.");

			ImGui::Checkbox("Replay Last Checkpoint", &setting().onLastCheckpoint);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Respawn from your last practice mode checkpoint after completing a level.");

			if (ImGui::Checkbox("Show Layout", &setting().onShowLayout)) {
				if (setting().onShowLayout) cheatAdd();
				else cheatDec();
			}

			if (ImGui::TreeNode("Show Layout Settings")) {
				if (ImGui::ColorEdit3("Ground Colour", levelGColor, ImGuiColorEditFlags_NoInputs)) {
					setting().levelGColorR = levelGColor[0] * 255.f;
					setting().levelGColorG = levelGColor[1] * 255.f;
					setting().levelGColorB = levelGColor[2] * 255.f;
				}
				if (ImGui::ColorEdit3("BG Colour", levelBGColor, ImGuiColorEditFlags_NoInputs)) {
					setting().levelBGColorR = levelBGColor[0] * 255.f;
					setting().levelBGColorG = levelBGColor[1] * 255.f;
					setting().levelBGColorB = levelBGColor[2] * 255.f;
				}
				ImGui::TreePop();
			}

			ImGui::Checkbox("Smart StartPos", &setting().onSmartStartPos);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Automatically sets gamemode, speed, size & border for a startpos.");

			ImGui::Checkbox("StartPos Switcher", &setting().onStartPosSwitcher);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Lets you switch between multiple start positions in-level.");
		}

		if (ImGui::Begin("Universal", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)); {
			ImGui::SetWindowFontScale(setting().UISize);

			ImGui::SetNextItemWidth(80 * setting().UISize);
			if (ImGui::InputFloat("##fpsbypass", &setting().fps, 0.f, 0.f, "%.0ffps"))
				update_fps_bypass();
			ImGui::SameLine();
			if (ImGui::Checkbox("FPS Bypass", &setting().onFPSBypass))
				update_fps_bypass();
			ImGui::EndTabItem();

			if (ImGui::Checkbox("Allow Low Volume", &setting().onAllowLowVolume)) {
				if (setting().onAllowLowVolume) {
					sequence_patch((uint32_t)gd::base + 0x15665e, { 0xeb, 0x08 });
					sequence_patch((uint32_t)gd::base + 0x15cebe, { 0xeb, 0x08 });
					sequence_patch((uint32_t)gd::base + 0x156500, { 0xeb, 0x08 });
					sequence_patch((uint32_t)gd::base + 0x15665e, { 0xeb, 0x08 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x15665e, { 0x76, 0x08 });
					sequence_patch((uint32_t)gd::base + 0x15cebe, { 0x76, 0x08 });
					sequence_patch((uint32_t)gd::base + 0x156500, { 0x76, 0x08 });
					sequence_patch((uint32_t)gd::base + 0x15665e, { 0x76, 0x08 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Removes snapping to 0.0 when setting volume to 0.3 or below.");

			if (ImGui::Checkbox("AntiCheat Bypass", &setting().onAntiCheatBypass)) {
				if (setting().onAntiCheatBypass) {
					sequence_patch((uint32_t)gd::base + 0x170fc2, { 0x90, 0x90, 0x90, 0x90, 0x90 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x170fc2, { 0xe8, 0xb9, 0xce, 0x00, 0x00 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Bypasses anticheat.");

			ImGui::Checkbox("Auto Safe Mode", &setting().onAutoSafeMode);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Enables Safe Mode only when cheats are enabled.");

			if (ImGui::Checkbox("Fast Alt-Tab", &setting().onFastAltTab)) {
				if (setting().onFastAltTab)
					sequence_patch((uint32_t)gd::base + 0x38cae, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
				else
					sequence_patch((uint32_t)gd::base + 0x38cae, { 0x8b, 0x03, 0x8b, 0xcb, 0xff, 0x50, 0x18 });
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables savefile saving on minimize.");

			if (ImGui::Checkbox("Force Visibility", &setting().onForceVisibility)) {
				if (setting().onForceVisibility) {
					sequence_patch((uint32_t)libcocosbase + 0x60813, { 0xb0, 0x01, 0x90 });
					sequence_patch((uint32_t)libcocosbase + 0x60d2a, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
				}
				else {
					sequence_patch((uint32_t)libcocosbase + 0x60813, { 0x8a, 0x45, 0x08 });
					sequence_patch((uint32_t)libcocosbase + 0x60d2a, { 0x0f, 0x84, 0xcb, 0x00, 0x00, 0x00 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Sets all nodes to be visible.");

			if (ImGui::Checkbox("Free Window Resize", &setting().onFreeWindowResize)) {
				if (setting().onFreeWindowResize) {
					sequence_patch((uint32_t)libcocosbase + 0x1110db, { 0x90, 0x90, 0x90, 0x90, 0x90 });
					sequence_patch((uint32_t)libcocosbase + 0x10fd93, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 });
					sequence_patch((uint32_t)libcocosbase + 0x110ad1, { 0xe9, 0x2f, 0xff, 0xff, 0xff, 0x90 });
				}
				else {
					sequence_patch((uint32_t)libcocosbase + 0x1110db, { 0xe8, 0xb0, 0xf3, 0xff, 0xff });
					//sequence_patch((uint32_t)libcocosbase + 0x10fd93, { 0xff, 0x15, 0x90, 0xa5, 0x8e, 0x55 });
					sequence_patch((uint32_t)libcocosbase + 0x110ad1, { 0x0f, 0x85, 0x2e, 0xff, 0xff, 0xff });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Removes limits in place for window resizing.");

			ImGui::Checkbox("Lock Cursor", &setting().onLockCursor);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Locks cursor position while playing.");

			ImGui::Checkbox("Cheat Indicator", &setting().onCheatIndicator);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Enables cheat indicator.");

			ImGui::Checkbox("No Transition", &setting().onNoTransition);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Shorterns scene transition time to 0s.");

			ImGui::Checkbox("Retry Keybind", &setting().onRetryKeybind);
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Lets you restart level by pressing R.");

			if (ImGui::Checkbox("Safe Mode", &setting().onSafeMode)) {
				if (setting().onSafeMode) safeModeON();
				else safeModeOFF();
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Disables progress and completion of levels.");

			if (ImGui::Checkbox("Show Restart Button", &setting().onShowRestartButton)) {
				if (setting().onShowRestartButton) {
					sequence_patch((uint32_t)gd::base + 0x15bc99, { 0x90, 0x90 });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x15bc99, { 0x75, 0x29 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Shows the restart button in pause menu.");

			if (ImGui::Checkbox("Transparent BG", &setting().onTransparentBG)) {
				if (setting().onTransparentBG) {
					sequence_patch((uint32_t)gd::base + 0x40f17, { 0x90, 0xb1, 0xff }); // CreatorLayer
					sequence_patch((uint32_t)gd::base + 0x40f1f, { 0xff, 0xff });
					sequence_patch((uint32_t)gd::base + 0xea5fb, { 0x90, 0xb1, 0xff }); // LevelBrowserLayer
					sequence_patch((uint32_t)gd::base + 0xea602, { 0xff, 0xff });
					sequence_patch((uint32_t)gd::base + 0x10c1fd, { 0x90, 0xb1, 0xff }); // LevelSearchLayer
					sequence_patch((uint32_t)gd::base + 0x10c205, { 0xff, 0xff });
					sequence_patch((uint32_t)gd::base + 0x107933, { 0x90, 0xb1, 0xff }); // LevelInfoLayer
					sequence_patch((uint32_t)gd::base + 0x10793a, { 0xff, 0xff });
					sequence_patch((uint32_t)gd::base + 0x571a1, { 0x90, 0xb1, 0xff }); // EditLevelLayer
					sequence_patch((uint32_t)gd::base + 0x571a8, { 0xff, 0xff });
					sequence_patch((uint32_t)gd::base + 0xe8da2, { 0x90, 0xb1, 0xff }); // LeaderboardsLayer
					sequence_patch((uint32_t)gd::base + 0xe8daa, { 0xff, 0xff });
				}
				else {
					sequence_patch((uint32_t)gd::base + 0x40f17, { 0x80, 0xc9, 0xff });
					sequence_patch((uint32_t)gd::base + 0x40f1f, { 0x00, 0x66 });
					sequence_patch((uint32_t)gd::base + 0xea5fb, { 0x80, 0xc9, 0xff });
					sequence_patch((uint32_t)gd::base + 0xea602, { 0x00, 0x66 });
					sequence_patch((uint32_t)gd::base + 0x10c1fd, { 0x80, 0xc9, 0xff });
					sequence_patch((uint32_t)gd::base + 0x10c205, { 0x00, 0x66 });
					sequence_patch((uint32_t)gd::base + 0x107933, { 0x80, 0xc9, 0xff });
					sequence_patch((uint32_t)gd::base + 0x10793a, { 0x00, 0x66 });
					sequence_patch((uint32_t)gd::base + 0x571a1, { 0x80, 0xc9, 0xff });
					sequence_patch((uint32_t)gd::base + 0x571a8, { 0x00, 0x66 });
					sequence_patch((uint32_t)gd::base + 0xe8da2, { 0x80, 0xc9, 0xff });
					sequence_patch((uint32_t)gd::base + 0xe8daa, { 0x00, 0x66 });
				}
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Removes the blue filter from menu's backgrounds.");

			if (ImGui::Checkbox("Zero Delay", &setting().onZeroDelay)) {
				auto cocos = GetModuleHandleA("libcocos2d.dll");
				if (!setting().onZeroDelay)
					MH_DisableHook(reinterpret_cast<LPVOID>(reinterpret_cast<uintptr_t>(cocos) + 0xfde30));
				else
					MH_EnableHook(reinterpret_cast<LPVOID>(reinterpret_cast<uintptr_t>(cocos) + 0xfde30));
			}
			if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.5f)
				ImGui::SetTooltip("Reduces input delay.");

			ImGui::SetNextWindowSize({ ImGui::GetWindowWidth() * setting().UISize, 0 });
		}

		if (ImGui::Begin("Speedhack", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize)); {
			ImGui::SetWindowFontScale(setting().UISize);

			ImGui::SetNextItemWidth(80 * setting().UISize);

			if (ImGui::InputFloat("##speedhack", &setting().speedhack, 0.f, 0.f, "%.2fx"))
			{
				update_speed_hack();
				if (setting().speedhack < 0.f) setting().speedhack = 0;
			}

			ImGui::SameLine();
			if (ImGui::Checkbox("Speedhack", &setting().onSpeedhack)) {
				update_speed_hack();
			}
			ImGui::Checkbox("Speedhack Music", &setting().onSpeedhackMusic);
			if (ImGui::Checkbox("Classic Mode", &setting().onClassicMode)) {
				update_speed_hack();
			}
		}
	}

	update_fps_bypass();
	update_speed_hack();

	if (setting().onFPSBypass) {
		update_fps_bypass();
	}
	if (setting().onSpeedhack) {
		update_speed_hack();
	}
}

void imgui_init() {
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->Clear();
	io.Fonts->AddFontFromFileTTF("Muli-SemiBold.ttf", 16.f);
	io.Fonts->Build();

	auto* colors = ImGui::GetStyle().Colors;

	color1.x = setting().Overlaycolor[0];
	color1.y = setting().Overlaycolor[1];
	color1.z = setting().Overlaycolor[2];
	color1.w = setting().Overlaycolor[3] - 0.4f;

	color2.x = setting().Overlaycolor[0];
	color2.y = setting().Overlaycolor[1];
	color2.z = setting().Overlaycolor[2];
	color2.w = 1.f;

	color3.x = setting().Overlaycolor[0] + 0.3f;
	color3.y = setting().Overlaycolor[1] + 0.3f;
	color3.z = setting().Overlaycolor[2] + 0.3f;
	color3.w = setting().Overlaycolor[3] + 0.3f;

	color4.x = setting().Overlaycolor[0] - 0.1f;
	color4.y = setting().Overlaycolor[1] - 0.1f;
	color4.z = setting().Overlaycolor[2] - 0.1f;
	color4.w = setting().Overlaycolor[3] - 0.1f;

	color5.x = setting().Overlaycolor[0] + 0.1f;
	color5.y = setting().Overlaycolor[1] + 0.1f;
	color5.z = setting().Overlaycolor[2] + 0.1f;
	color5.w = setting().Overlaycolor[3] + 0.1f;

	color6.x = setting().BGcolor[0];
	color6.y = setting().BGcolor[1];
	color6.z = setting().BGcolor[2];
	color6.w = setting().BGcolor[3];

	//colors[ImGuiCol_Text] = ImVec4(1, 0, 0, 1);
	colors[ImGuiCol_WindowBg] = color6;
	colors[ImGuiCol_Border] = color2;
	colors[ImGuiCol_BorderShadow] = color2;
	colors[ImGuiCol_Button] = color3;
	colors[ImGuiCol_ButtonHovered] = color1;
	colors[ImGuiCol_ButtonActive] = color1;
	colors[ImGuiCol_CheckMark] = ImVec4(1.f, 1.f, 1.f, 1.f);
	colors[ImGuiCol_FrameBg] = color1;
	colors[ImGuiCol_FrameBgHovered] = color3;
	colors[ImGuiCol_FrameBgActive] = color3;
	colors[ImGuiCol_Tab] = ImVec4(color5.x, color5.y, color5.z, color5.w - 0.2f);
	colors[ImGuiCol_TabHovered] = color3;
	colors[ImGuiCol_TabActive] = color3;
	colors[ImGuiCol_TitleBg] = color3;
	colors[ImGuiCol_TitleBgActive] = color3;
	colors[ImGuiCol_TitleBgCollapsed] = color3;
	colors[ImGuiCol_SliderGrab] = color4;
	colors[ImGuiCol_SliderGrabActive] = color4;
	colors[ImGuiCol_TextSelectedBg] = color2;
	colors[ImGuiCol_Header] = color1;
	colors[ImGuiCol_HeaderHovered] = color1;
	colors[ImGuiCol_HeaderActive] = color2;
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.f, 0.f, 0.f, 0.f);
}

void setup_imgui_menu() {
	SpeedHack::Setup();

	if (!std::filesystem::is_directory("PolzHax") || !std::filesystem::exists("PolzHax"))
	{
		std::filesystem::create_directory("PolzHax");
	}
	if (!std::filesystem::is_directory("PolzHax/extensions") || !std::filesystem::exists("PolzHax/extensions"))
	{
		std::filesystem::create_directory("PolzHax/extensions");
	}

	auto path = CCFileUtils::sharedFileUtils()->getWritablePath2() + "PolzHax/extensions";

	for (const auto& file : std::filesystem::directory_iterator(path))
	{
		if (file.path().extension() == ".dll")
		{
			auto dllname = file.path().filename().string();
			dllNames.push_back(dllname);
			LoadLibrary(file.path().string().c_str());
		}
	}

	std::cout << "Extensions Loaded: " << dllNames.size() << std::endl;
	for (const auto& name : dllNames) {
		std::cout << name << std::endl;
	}

	ImGuiHook::setToggleCallback([]() {setting().show = !setting().show; });
	ImGuiHook::setRenderFunction(imgui_render);
	ImGuiHook::setInitFunction(imgui_init);
	ImGuiHook::setupHooks([](auto addr, auto hook, auto orig) {
		MH_CreateHook(addr, hook, orig);
		MH_EnableHook(addr);
		});
}