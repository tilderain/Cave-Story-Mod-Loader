// Graphics enhancement mod for Freeware Cave Story
// Copyright © 2017 Clownacy

#include "patch_teleport_screen.h"

#include "mod_loader.h"

#include "../common.h"
#include "drawsprite1_centred.h"

void PatchTeleportScreen(void)
{
	// Clip rectangle
	ModLoader_WriteLong((void*)0x41D854 + 3, SCREEN_WIDTH);
	// "WARP"
	ModLoader_WriteRelativeAddress((void*)0x41D8E2 + 1, DrawSprite1_hijack);
	// Cursor
	ModLoader_WriteRelativeAddress((void*)0x41D961 + 1, DrawSprite1_hijack);
	// Location icons
	ModLoader_WriteRelativeAddress((void*)0x41D9EE + 1, DrawSprite1_hijack);
}
