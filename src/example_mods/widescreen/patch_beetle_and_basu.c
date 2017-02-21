#include "mod_loader.h"

#include "common.h"
#include "patch_beetle_and_basu.h"

void PatchBeetleAndBasu(void)
{
	// Beetle (object 8)
	WriteLong(0x428010 + 2, ((SCREEN_WIDTH / 2) + 96) * 0x200);
	WriteLong(0x42802E + 2, ((SCREEN_WIDTH / 2) + 96) * 0x200);
	// Basu (object 58)
	WriteLong(0x431D93 + 1, ((SCREEN_WIDTH / 2) + 96) * 0x200);
	WriteLong(0x431DAF + 1, ((SCREEN_WIDTH / 2) + 96) * 0x200);
}
