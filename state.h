#pragma once
#include "pch.h"
#include <string>
#include <iostream>
#include "schema.hpp"

struct LayersStruct {
	CCLayer* PauseLayerObject;
};

struct SettingStruct {
	void loadState();
	void saveState();

	bool show;
	bool explorer;

	float Overlaycolor[4] = { 0.235, 0.235, 0.235, 1 };
	float BGcolor[4] = { 0, 0, 0, 0.902 };
	float UISize = 1.0f;

	int cheatsCount = 0;
	int beforeRestartCheatsCount = 0;
	bool isSafeMode;

	bool onAutoSave;

	bool onCharFilter;
	bool onIcons;
	bool onLoadFailed;
	bool onMainLevels;
	bool onSliderLimit;
	bool onTextLength;
	bool onTheVault;

	bool onHideAttempts;
	bool onHidePauseButton;
	bool onHidePauseMenu;
	bool onHidePlayer;
	bool onHidePracticeButtons;
	bool onNoDeathEffect;
	bool onNoMiniIcon;
	bool onNoOrbRing;
	bool onNoParticles;
	bool onNoRespawnFlash;
	bool onPracticePulse;
	bool onSolidWaveTrail;
	bool onTrailAlwaysOff;
	bool onTrailAlwaysOn;
	bool onTrailBugFix;

	bool onAbsolutePosition;
	bool onCopyHack;
	bool onCustomObjectBypass;
	bool onFreeScroll;
	bool onLevelEdit;
	bool onNoCMark;
	bool onObjectBypass;
	bool onScaleSnapBypass;
	bool onSmoothTrail;
	bool onToolboxBypass;
	bool onVerifyHack;

	bool onBallRotatingBugFix;
	bool onConfirmExit;
	bool onCorrectiveMusicSync;
	bool onNoclip;
	bool onPauseDuringComplete;
	bool onPracticeBugFix;
	bool onPracticeMusic;
	bool onSmartStartPos;
	bool onStartPosSwitcher;

	float fps = 60.f;
	bool onFPSBypass;
	bool onAntiCheatBypass;
	bool onAutoSafeMode;
	bool onForceVisibility;
	bool onNoTransition;
	bool onRetryKeybind;
	bool onSafeMode;
	bool onShowRestartButton;
	bool onZeroDelay;

	float speedhack = 1.f;
	bool onSpeedhack;
	bool onSpeedhackMusic;
	bool onClassicMode;
};

LayersStruct& layers();
SettingStruct& setting();

DEF_SCHEMA(
	SettingStruct,
	cheatsCount,
	beforeRestartCheatsCount,
	isSafeMode,

	onAutoSave,
	onCharFilter,
	onTheVault,
	onIcons,
	onLoadFailed,
	onMainLevels,
	onSliderLimit,
	onTextLength,

	onHideAttempts,
	onHidePauseButton,
	onHidePauseMenu,
	onHidePlayer,
	onHidePracticeButtons,
	onNoDeathEffect,
	onNoMiniIcon,
	onNoOrbRing,
	onNoParticles,
	onNoRespawnFlash,
	onPracticePulse,
	onSolidWaveTrail,
	onTrailAlwaysOff,
	onTrailAlwaysOn,
	onTrailBugFix,

	onAbsolutePosition,
	onCopyHack,
	onCustomObjectBypass,
	onFreeScroll,
	onLevelEdit,
	onNoCMark,
	onObjectBypass,
	onScaleSnapBypass,
	onSmoothTrail,
	onToolboxBypass,
	onVerifyHack,

	onBallRotatingBugFix,
	onConfirmExit,
	onCorrectiveMusicSync,
	onNoclip,
	onPauseDuringComplete,
	onPracticeBugFix,
	onPracticeMusic,
	onSmartStartPos,
	onStartPosSwitcher,

	fps,
	onFPSBypass,
	onAntiCheatBypass,
	onAutoSafeMode,
	onForceVisibility,
	onNoTransition,
	onRetryKeybind,
	onSafeMode,
	onShowRestartButton,
	onZeroDelay
)
