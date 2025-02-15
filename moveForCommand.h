#pragma once
#include "pch.h"

namespace moveForCommand {
	static constexpr const gd::EditCommand kEditCommandHalfLeft = static_cast<gd::EditCommand>(0x400);
	static constexpr const gd::EditCommand kEditCommandHalfRight = static_cast<gd::EditCommand>(0x401);
	static constexpr const gd::EditCommand kEditCommandHalfUp = static_cast<gd::EditCommand>(0x402);
	static constexpr const gd::EditCommand kEditCommandHalfDown = static_cast<gd::EditCommand>(0x403);

	static constexpr const gd::EditCommand kEditCommandQuarterLeft = static_cast<gd::EditCommand>(0x404);
	static constexpr const gd::EditCommand kEditCommandQuarterRight = static_cast<gd::EditCommand>(0x405);
	static constexpr const gd::EditCommand kEditCommandQuarterUp = static_cast<gd::EditCommand>(0x406);
	static constexpr const gd::EditCommand kEditCommandQuarterDown = static_cast<gd::EditCommand>(0x407);

	static constexpr const gd::EditCommand kEditCommandEightLeft = static_cast<gd::EditCommand>(0x408);
	static constexpr const gd::EditCommand kEditCommandEightRight = static_cast<gd::EditCommand>(0x409);
	static constexpr const gd::EditCommand kEditCommandEightUp = static_cast<gd::EditCommand>(0x40a);
	static constexpr const gd::EditCommand kEditCommandEightDown = static_cast<gd::EditCommand>(0x40b);

	static constexpr const gd::EditCommand kEditCommandUnitLeft = static_cast<gd::EditCommand>(0x40c);
	static constexpr const gd::EditCommand kEditCommandUnitRight = static_cast<gd::EditCommand>(0x40d);
	static constexpr const gd::EditCommand kEditCommandUnitUp = static_cast<gd::EditCommand>(0x40e);
	static constexpr const gd::EditCommand kEditCommandUnitDown = static_cast<gd::EditCommand>(0x40f);
}

namespace rotationForCommand {
	static constexpr const gd::EditCommand kEditCommandRotate265CW = static_cast<gd::EditCommand>(0x414);
	static constexpr const gd::EditCommand kEditCommandRotate265CCW = static_cast<gd::EditCommand>(0x415);
}