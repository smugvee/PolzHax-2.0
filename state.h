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

	bool onAutoSave;

	bool onCharFilter;
	bool onIcons;
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
};

LayersStruct& layers();
SettingStruct& setting();

DEF_SCHEMA(
	SettingStruct,
	onAutoSave,
	onCharFilter,
	onIcons,
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
	onToolboxBypass,
	onVerifyHack
)
