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

	float Overlaycolor[4] = { 0.235f, 0.235f, 0.235f, 1.f };
	float BGcolor[4] = { 0.f, 0.f, 0.f, 0.902f };
	float UISize = 1.0f;
	float overlayCol = 0.235f;
	float overlayCol2 = 0.235f;
	float overlayCol3 = 0.235f;
	float overlayCol4 = 1.f;

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

	bool onForceDontEnter;
	bool onForceDontFade;
	bool onHideAttempts;
	bool onHidePauseButton;
	bool onHidePauseMenu;
	bool onHidePlayer;
	bool onHidePracticeButtons;
	bool onNoBackgroundFlash;
	bool onNoDeathEffect;
	bool onNoLightning;
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
	bool onDefaultSongBypass;
	bool onEditorExtension;
	bool onFreeScroll;
	bool onHidePreviewLine;
	bool onHideUI;
	bool onLevelEdit;
	bool onNoCMark;
	bool onNoDeathX;
	bool onObjectBypass;
	bool onPlaceOver;
	bool onScaleSnapBypass;
	bool onSmoothTrail;
	bool onToolboxBypass;
	bool onVerifyHack;
	bool onZOrderBypass;
	bool onZoomBypass;

	bool onBallRotatingBugFix;
	bool onCheckpointLagFix;
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
	float pitch = 0.8f;
	bool onAllowLowVolume;
	bool onAntiCheatBypass;
	bool onAutoSafeMode;
	bool onForceVisibility;
	bool onLockCursor;
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

	onForceDontEnter,
	onForceDontFade,
	onHideAttempts,
	onHidePauseButton,
	onHidePauseMenu,
	onHidePlayer,
	onHidePracticeButtons,
	onNoBackgroundFlash,
	onNoDeathEffect,
	onNoLightning,
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
	onDefaultSongBypass,
	onEditorExtension,
	onFreeScroll,
	onHidePreviewLine,
	onHideUI,
	onLevelEdit,
	onNoCMark,
	onNoDeathX,
	onObjectBypass,
	onPlaceOver,
	onScaleSnapBypass,
	onSmoothTrail,
	onToolboxBypass,
	onVerifyHack,
	onZOrderBypass,
	onZoomBypass,

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
	onAllowLowVolume,
	onAntiCheatBypass,
	onAutoSafeMode,
	onForceVisibility,
	onLockCursor,
	onNoTransition,
	onRetryKeybind,
	onSafeMode,
	onShowRestartButton,
	onZeroDelay,

	speedhack,
	onSpeedhack,
	onSpeedhackMusic,
	onClassicMode
)
