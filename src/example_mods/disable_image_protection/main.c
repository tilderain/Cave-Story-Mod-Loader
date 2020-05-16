// Image integrity check disabler mod for Freeware Cave Story
// Copyright © 2017 Clownacy

#include "mod_loader.h"

static int PixelProtection(void)
{
	return 1;
}

void InitMod(void)
{
	
	ModLoader_WriteByte((void*)0x41D047, 0x00); //decode disable
	//ModLoader_WriteJump((void*)0x4112E0, PixelProtection);
	
	/*while (!memcmp((void*)0x43C46F, (char[]){0x70, 0x78, 0x69, 0x6D, 0x67}, 5))
	{
		ModLoader_WriteLongBE((void*)0x43C46F, 0x626D7000);
	}*/
	
	
	
	//mutex disable
	ModLoader_WriteJump((void*)0x41CEA8, 0x41CED1);
	ModLoader_WriteJump((void*)0x41D009, 0x41D032);

}
