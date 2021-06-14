﻿#pragma once

UENUM(BlueprintType)
enum class ECygnusError : uint8
{
	// No Error
	NoError,

	/// Item Errors

	// ItemStack Error
	ItemMismatch,
	
	// Hotbar Error
	HotbarFull,

	// RiftSack Error
	RiftSackFull
};