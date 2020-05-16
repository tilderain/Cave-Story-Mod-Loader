// 60FPS mod for Freeware Cave Story
// Copyright © 2018 Clownacy

#include "60fps.h"

#include <stddef.h>
#include <windows.h>

#include "cave_story.h"
#include "mod_loader.h"

void UpdateTicks(void)
{
	const unsigned int delay_ticks[] = {16, 17, 17};

	static unsigned char counter;

	ModLoader_WriteByte((void*)0x41A744, delay_ticks[counter % 3]);
	ModLoader_WriteByte((void*)0x41A776, delay_ticks[counter % 3]);

	++counter;
}

__asm(
"_UpdateTicks_ASM:\n"
"	call	_UpdateTicks\n"
"	pushl	0x43B29C\n"
"	ret\n"
);
extern char UpdateTicks_ASM;

void CompensateScrollSpeed(int scroll)
{
	scroll = (int)(scroll / 1.2);
	*(int *)0x44C858 = scroll;
}

__asm(
"_CompensateScrollSpeed_ASM:\n"
"	push	%eax\n"
"	call	_CompensateScrollSpeed\n"
"   pop %ebp\n"
"	ret\n"
);
extern char CompensateScrollSpeed_ASM;

void Apply60FPSPatch(void)
{
	ModLoader_WriteByte((void*)0x423116, 0x90);
	ModLoader_WriteCall((void*)0x423116 + 1, &UpdateTicks_ASM);
	
	//ModLoader_WriteCall((void*)0x4210A6, &CompensateScrollSpeed_ASM);
}


