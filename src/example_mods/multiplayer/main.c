//Today's C was brought to you by the letter G

#define typeof __typeof__
#define pack(__c, __i) do {typeof(__i) *__x = ((typeof(__i) *) __c); *__x = __i;} while(0);
#define unpack(__type, __c) (*((__type *) __c))

#define TCP_NODELAY 1

#include "mod_loader.h"
#include <SDL2/SDL_thread.h>
#include "SDL2/SDL_net.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "cave_story.h"
#include "guxt.h"

SDL_Thread* threadID = NULL;

IPaddress ip, *remoteIP;
TCPsocket sd, csd;
char xbuf[32];
char buffer[32];

bool hosting = false;
int recv_input = 0;
int port = 6087;
int ping = 0;

int pad_delay = 5; //network buffer
bool inputAvailable = false;

int my_cur_input = 0;
int other_cur_input = 0;
int my_inputs[65535];
int other_inputs[65535];

__declspec(dllexport) gxObject bullets[256] = {0};

int packets_recvd = 0;
bool buf_recv = false;
bool control = false;

short frame_count = 0;

//other player vars

gxShip main_ship;
gxShip other_ship;

int main_speed, other_speed;

gxShipExtra main_extra;
gxShipExtra other_extra;

int AwaitInput(int input);
int InitNetwork(void);

SDL_ThreadFunction ThreadedAwaitInput(void *data);

void MP_ResetShip(void);
void MP_SetInitialShipValues(void);

void MP_ActShipState(int input);
void MP_HitShipPowerup(void);
void MP_HitShipMap(void);
     
void MP_PutPlayer(int screen_offsetW);
void MP_PutPlayerExtra(int screen_offsetW);



void PrintGXObject(gxShip Ship);

void PrintGXObject(gxShip o)
{
	ModLoader_PrintDebug("cond %d \n", o.cond);
	ModLoader_PrintDebug("x %d \n", o.x);                    
	ModLoader_PrintDebug("y %d \n",  o.y);                   
	ModLoader_PrintDebug("xm %d \n" , o.xm);                   
	ModLoader_PrintDebug("ym %d \n",  o.ym);                   
	ModLoader_PrintDebug("xoff %d \n", o.xoff);                    
	ModLoader_PrintDebug("yoff %d \n", o.yoff);                    
	ModLoader_PrintDebug("w %d \n", o.w);                    
	ModLoader_PrintDebug("h %d \n", o.h);                    
	ModLoader_PrintDebug("surf %d \n", o.surf);                    
	ModLoader_PrintDebug("type %d \n", o.type);                    
	ModLoader_PrintDebug("state %d \n", o.state);                    
	ModLoader_PrintDebug("count1 %d \n", o.count1);                    
	ModLoader_PrintDebug("count2 %d \n", o.count2);                    
	ModLoader_PrintDebug("count3 %d \n", o.count3);                   
	ModLoader_PrintDebug("life %d \n",  o.life);                  
	ModLoader_PrintDebug("damage %d \n", o.damage);                  
	ModLoader_PrintDebug("flag %d \n", o.flag);                   
	ModLoader_PrintDebug("shock %d \n", o.shock);
	ModLoader_PrintDebug("score %d \n", o.score);
	ModLoader_PrintDebug("type2 %d \n", o.type2);
	ModLoader_PrintDebug("child %d \n", o.child);
	ModLoader_PrintDebug("rot1 %d \n", o.rot1);
	ModLoader_PrintDebug("count4 %d \n", o.count4);
	ModLoader_PrintDebug("num %d \n", o.num);
	ModLoader_PrintDebug("destroyHitVoice %d \n", o.destroyHitVoice);
	
}

void PrintGXObjectAttr(gxObjectAttr Ship);

void PrintGXObjectAttr(gxObjectAttr o)
{
	ModLoader_PrintDebug("flag %d \n", o.flag);      
	ModLoader_PrintDebug("score %d \n", o.score);
	ModLoader_PrintDebug("damage %d \n", o.damage);                     
	ModLoader_PrintDebug("xoff %d \n", o.xoff);                    
	ModLoader_PrintDebug("yoff %d \n", o.yoff);                    
	ModLoader_PrintDebug("w %d \n", o.w);                    
	ModLoader_PrintDebug("h %d \n", o.h);              
	ModLoader_PrintDebug("life %d \n",  o.life);   	
	ModLoader_PrintDebug("surf %d \n", o.surf);                                                     
	ModLoader_PrintDebug("destroyHitVoice %d \n", o.destroyHitVoice);
}


void MP_InputFunc(void (*f)(int));
void MP_GenericFunc(void (*f)(int), int arg);
void MP_NoArgFunc(void (*f)());

void MP_InputFunc(void (*f)(int)) 
{
	
	if(hosting) (*f)(&my_inputs[frame_count]);
	else (*f)(&other_inputs[frame_count]);
	
	main_ship = *(gxShip *)GX_Ship;
	
	//PrintGXObject(*(gxShip *)GX_Ship);
	*(gxShip *)GX_Ship = other_ship;
	
	if(hosting) (*f)(&other_inputs[frame_count]);
	else (*f)(&my_inputs[frame_count]);
	
	other_ship = *(gxShip *)GX_Ship;
	*(gxShip *)GX_Ship = main_ship;
	
	//PrintGXObject(*(gxShip *)GX_Ship);
	
}


void MP_GenericFunc(void (*f)(int), int arg) 
{
	if(hosting) (*f)(arg);
	else (*f)(arg);
	
	main_ship = *(gxShip *)GX_Ship;
	*(gxShip *)GX_Ship = other_ship;
	
	if(hosting) (*f)(arg);
	else (*f)(arg);
	
	other_ship = *(gxShip *)GX_Ship;
	*(gxShip *)GX_Ship = main_ship;
}


void MP_NoArgFunc(void (*f)()) 
{
	if(hosting) (*f)();
	else (*f)();
	
	main_ship = *(gxShip *)GX_Ship;
	*(gxShip *)GX_Ship = other_ship;
	
	if(hosting) (*f)();
	else (*f)();
	
	other_ship = *(gxShip *)GX_Ship;
	*(gxShip *)GX_Ship = main_ship;
	
}

	//ModLoader_ReadStruct((void*)GX_Ship, &main_ship, sizeof(gxObject));
	
	//ModLoader_WriteStruct((void*)GX_Ship, &main_ship, sizeof(gxObject));
	//WriteShipStruct(main_ship);
	//ModLoader_ReadStruct((void*)GX_Ship, &main_ship, sizeof(gxObject));
	//PrintGXObject(main_ship);


void MP_ResetShip(void){ MP_NoArgFunc(GXF_ResetShip); }
void MP_SetInitialShipValues(void) { MP_NoArgFunc(GXF_SetInitialShipValues); }

void MP_ActShipState(int input)  { MP_InputFunc(GXF_ActShipState); }
void MP_HitShipPowerup(void) { MP_NoArgFunc(GXF_HitShipPowerup); }
void MP_HitShipMap(void) { MP_NoArgFunc(GXF_HitShipMap); }
     
void MP_PutPlayer(int screen_offsetW) { MP_GenericFunc(GXF_PutPlayer, GX_ScreenOffsetW); }
void MP_PutPlayerExtra(int screen_offsetW) { MP_GenericFunc(GXF_PutPlayerExtra, GX_ScreenOffsetW); }



void AdjustObjectHealthAttr(void);
void AdjustObjectHealthAttr(void)
{

	((gxObjectAttr*)GX_ObjectAttrTbl)[5].life = 1010;
	((gxObjectAttr*)GX_ObjectAttrTbl)[15].life = 1100;
	((gxObjectAttr*)GX_ObjectAttrTbl)[32].life = 1042;
	((gxObjectAttr*)GX_ObjectAttrTbl)[34].life = 1060;
	((gxObjectAttr*)GX_ObjectAttrTbl)[36].life = 1040;
	
	//check these
	((gxObjectAttr*)GX_ObjectAttrTbl)[51].life = 1040;
	((gxObjectAttr*)GX_ObjectAttrTbl)[52].life = 1040;
	((gxObjectAttr*)GX_ObjectAttrTbl)[53].life = 1040;
	((gxObjectAttr*)GX_ObjectAttrTbl)[55].life = 1040;
	((gxObjectAttr*)GX_ObjectAttrTbl)[76].life = 1050;
	((gxObjectAttr*)GX_ObjectAttrTbl)[77].life = 1010;
	((gxObjectAttr*)GX_ObjectAttrTbl)[78].life = 1010;
	((gxObjectAttr*)GX_ObjectAttrTbl)[80].life = 1010;
	((gxObjectAttr*)GX_ObjectAttrTbl)[82].life = 1010;
	
	((gxObjectAttr*)GX_ObjectAttrTbl)[84].life = 1005;
	((gxObjectAttr*)GX_ObjectAttrTbl)[94].life = 1020;
	
	((gxObjectAttr*)GX_ObjectAttrTbl)[95].life = 1010;
	
	((gxObjectAttr*)GX_ObjectAttrTbl)[100].life = 1030;
	
	((gxObjectAttr*)GX_ObjectAttrTbl)[101].life = 1030;
	
	((gxObjectAttr*)GX_ObjectAttrTbl)[104].life = 1010;
	((gxObjectAttr*)GX_ObjectAttrTbl)[105].life = 1010;
	
	((gxObjectAttr*)GX_ObjectAttrTbl)[108].life = 1010;
	
	((gxObjectAttr*)GX_ObjectAttrTbl)[109].life = 1010;
	((gxObjectAttr*)GX_ObjectAttrTbl)[110].life = 1010;
	((gxObjectAttr*)GX_ObjectAttrTbl)[112].life = 1010;
	((gxObjectAttr*)GX_ObjectAttrTbl)[113].life = 1010;
	((gxObjectAttr*)GX_ObjectAttrTbl)[114].life = 1010;
	
	((gxObjectAttr*)GX_ObjectAttrTbl)[121].life = 1010;
	/////
	
	
	((gxObjectAttr*)GX_ObjectAttrTbl)[60].life = 1001;
	((gxObjectAttr*)GX_ObjectAttrTbl)[64].life = 1001;
	((gxObjectAttr*)GX_ObjectAttrTbl)[74].life = 1001;
	
	for(int i = 0; i < 122; i++)
	{
		gxObjectAttr attr = ((gxObjectAttr*)GX_ObjectAttrTbl)[i];
		int life = attr.life;
		if(life > 100) continue;
		//ModLoader_PrintDebug("hp is %d\n", life);
		life = (life * 42) >> 5;
		if(life <= 4) life = 5;
		//ModLoader_PrintDebug("adjusted hp is %d\n", life);
		
		((gxObjectAttr*)GX_ObjectAttrTbl)[i].life = life;
	}
	
}

int InitNetwork(void)
{
	memset(&bullets, 0, sizeof(bullets));
	bullets[1].damage = 55;
	
	for(int i = 0; i<122; i++)
	{
		//ModLoader_PrintDebug("----------------%s---------------- \n", NpcFuncNames[i]);
		//PrintGXObjectAttr(((gxObjectAttr*)GX_ObjectAttrTbl)[i]);
		if(((gxObjectAttr*)GX_ObjectAttrTbl)[i].life > 100)
		{
			ModLoader_PrintDebug("----------------%s---------------- \n", NpcFuncNames[i]);
			ModLoader_PrintDebug("life %d\n",((gxObjectAttr*)GX_ObjectAttrTbl)[i].life);
		}
	}
	
	
	//AdjustObjectHealthAttr();
	
	if (SDLNet_Init() < 0)
	{
		ModLoader_PrintDebug("SDLNet_Init: %d\n", SDLNet_GetError());
		return 0;
	}
	//goto client;
	if(SDLNet_ResolveHost(&ip, "127.0.0.1", port) != 0)
	{
client:
		ModLoader_PrintDebug("Joining!");
		while(true)
		{
			if(SDLNet_ResolveHost(&ip, "127.0.0.1", port) == 0) break;
			Sleep(1);
		}
		if(!(sd = SDLNet_TCP_Open(&ip)))
		{
			ModLoader_PrintDebug("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		}
		 	
		// send a hello over sock
		//TCPsocket sock;
		int result;//,len
		char *msg="                       ";
		
		//len=strlen(msg)+1; // add one for the terminating NULL
		
		int thetime = 1;//(unsigned)time(NULL);
		pack(&buffer[0], thetime);
		
		result=SDLNet_TCP_Send(sd,&buffer,4);
		result=SDLNet_TCP_Send(sd,&buffer,4);
		result=SDLNet_TCP_Send(sd,&buffer,4);
		if(result<4) {
			ModLoader_PrintDebug("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
			// It may be good to disconnect sock because it is likely invalid now.
		}
		if (SDLNet_TCP_Recv(sd, buffer, 512) > 0)
		{
			ModLoader_PrintDebug("Apparently the ping is: ");
			//ModLoader_PrintDebug(buffer);
			ping = (int)SDLNet_Read32(buffer);
			ModLoader_PrintDebug(" convert %d", ping);
		}
	}
	else if(SDLNet_ResolveHost(&ip, NULL, port) == 0)
	{
		ModLoader_PrintDebug("Hosting!");
		if(!(sd = SDLNet_TCP_Open(&ip)))
		{
			ModLoader_PrintDebug("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
			goto client;
		}
		hosting = true;
		while(true)
		{
			if(!(csd = SDLNet_TCP_Accept(sd))) Sleep(1);
			else break;
		}
		if((remoteIP = SDLNet_TCP_GetPeerAddress(csd)))
		{
			ModLoader_PrintDebug("Host connected: %x %d\n", SDLNet_Read32(&remoteIP->host), SDLNet_Read16(&remoteIP->port)); 
			GetTickCount();
			if (SDLNet_TCP_Recv(csd, buffer, 32) > 0)
			{
				//ModLoader_PrintDebug(buffer);
			}
			
			int iptr = unpack(uint32_t, &buffer[0]);
			
			ModLoader_PrintDebug("Apparently the time is: %d\n", 1);//(unsigned)time(NULL));
			
			ModLoader_PrintDebug("What I got was: %d\n", (unsigned)buffer[0]);
			
			ping = 1;//(unsigned)time(NULL) - (unsigned)buffer[0];
			ModLoader_PrintDebug("Apparently the ping is: %d", ping);
			SDLNet_TCP_Send(csd, &ping, 4);
			//ModLoader_PrintDebug(buffer);
			//ping = (int)buffer[0];
			ModLoader_PrintDebug(" convert %d", ping);
			
		}
	}
	
	int data = 1;
	threadID = SDL_CreateThread( ThreadedAwaitInput, "LazyThread", (void*)data );
	
	for(int i = 0; i < pad_delay; i++)
	{
		my_inputs[i] = 0;
		other_inputs[i] = 0;
	}
	
	return 1;
}

int SendTCP(const void *data);

int SendTCP(const void *data)
{
	if(hosting) return SDLNet_TCP_Send(csd, &data, 32);
	return SDLNet_TCP_Send(sd, &data, sizeof(data));
}

int RecvTCP(void *data);

int RecvTCP(void *data)
{
	if(hosting) return SDLNet_TCP_Recv(csd, data, 32);
	return SDLNet_TCP_Recv(sd, data, 32);
}

SDL_ThreadFunction ThreadedAwaitInput(void *data)
{
	if(hosting) control = true;
	while(true)
	{
		//memset(buffer, 0, 32); //why does this cause it to not work? i know why
		RecvTCP((void*)buffer);
		buf_recv = true;
		packets_recvd++;
		
		//now's a good time to send any unacknowleged frames
		
		other_inputs[packets_recvd + pad_delay] = (int)buffer[0];
		
		//ModLoader_PrintDebug("%d    ", (short)SDLNet_Read16(buffer+4));
		/*for (int i = 0; i < sizeof(buffer); i++)
		{
			if (i > 0) printf(":");
			printf("%02X", buffer[i]);
		}*/
		//printf("\n");
		Sleep(1);
	}
}

int AwaitInput(int input)
{
	/*
		this combo works but that's about it
		
		
		SendTCP((void*)input);
		
		return (int)buffer[0];
		
	*/
	//ModLoader_PrintDebug("%d", input);
	
	//Check the frame array and see if there is an input for our frame
	
	//If not, wait until there is.
	
	//memset(xbuf, 0, 32);
	
	frame_count++;
	
	//*(int *)GX_Score = frame_count % 180;
	
	my_inputs[pad_delay + frame_count] = input;
	
	/*xbuf[0] = ((unsigned int)input>>24) & 0xff;
	xbuf[1] = ((unsigned int)input>>16) & 0xff;
	xbuf[2] = ((unsigned int)input>>8)  & 0xff;
	xbuf[3] = (unsigned int)input & 0xff;
	
	
	//memcpy(buffer, frame_count, 1);
	xbuf[4] = (unsigned int)(frame_count >> 8) & 0xff;
	xbuf[5] = (unsigned int)frame_count & 0xff;*/
	

	pack(xbuf, input);
	//sprintf(buffer,"%d", input);
	//ModLoader_PrintDebug("%d\n", input);
	//ModLoader_PrintDebug("%d\n", (int)SDLNet_Read32(buffer));
	//buffer[2] = input;
	//int i;
	/*ModLoader_PrintDebug("pre    ", (short)SDLNet_Read16(buffer+4));
	for (i = 0; i < sizeof(buffer); i++)
	{
		if (i > 0) printf(":");
		printf("%02X", buffer[i]);
	}
	printf("\n");*/
	
	SendTCP((void*)input);	
	//RecvTCP((void*)buffer);	
	
	while(frame_count >= packets_recvd + pad_delay){Sleep(23);}
	buf_recv = false;
	
	/*ModLoader_PrintDebug("%d    ", (short)SDLNet_Read16(buffer+4));
	for (i = 0; i < sizeof(buffer); i++)
	{
		if (i > 0) printf(":");
		printf("%02X", buffer[i]);
	}
	printf("\n");*/
	
	//if(frame_count % 180 == 0) control ^= 1;
	
	//if(control) return input;
	//return (int)buffer[0];
	
	if(hosting) return my_inputs[frame_count];
	
	//ModLoader_PrintDebug("%d\n", (short)SDLNet_Read16(buffer+4));
	//ModLoader_PrintDebug("%d\n", (int)SDLNet_Read32(buffer));
	
	return other_inputs[frame_count];
}

__asm(
"_DragReplace_ASM:\n"
"	call	_InitNetwork\n"
"	call	0x4192A0\n"
"	ret\n"
);
extern char DragReplace_ASM;


__asm(
"_TrgGet_ASM:\n"
"	call	0x401310\n"
"	push	%eax\n"
"	call	_AwaitInput\n"
"	add	$4,%esp\n"
"	ret\n"
);
extern char TrgGet_ASM;

__asm(
"_ObjectHealth_ASM:\n"
"	movl	-0x04(%ebp), %ecx\n"
"	push	%ecx\n"
"	movl	$42, %ecx\n"
"	imul	%ecx, %eax\n"
"	shr		$5, %eax\n" 
"	cmp		$1, %eax\n"
"	je	2f\n"
"	add 	$1, %eax\n"
"2:\n"
"	pop 	%ecx\n"
"	movl	%eax, 0x3C(%ecx)\n"
"	movl	-0x08(%ebp), %edx\n"
"	jmp	*1f\n"
"1:\n"
"	.long	0x403557\n"
);
extern char ObjectHealth_ASM;

int AdjustObjectHealth(int life);
int AdjustObjectHealth(int life)
{
	life = (life * 42) >> 5;
	if(life == 1) life++;
	return life;
}

__asm(
"_ObjectHealthF_ASM:\n"
"	movl	-0x04(%ebp), %ecx\n"
"	push	%eax\n"
"	call 	_AdjustObjectHealth\n"
"	movl	%eax, 0x3C(%ecx)\n"
"	movl	-0x08(%ebp), %edx\n"
"	jmp	*1f\n"
"1:\n"
"	.long	0x403557\n"
);
extern char ObjectHealthF_ASM;

__asm(
"_DisableScoreAttack_ASM:\n"
"	jmp	*1f\n"
"1:\n"
"	.long	0x4195C1\n"
);
extern char DisableScoreAttack_ASM;

void InitMod(void)
{
	
	ModLoader_WriteCall((void*)0x41A4BE, &DragReplace_ASM);
	
	ModLoader_WriteCall((void*)0x41B530, &TrgGet_ASM);
	
	//Since all the player funcs work on hardcoded values,
	// we are going to swap in our second ship and run the functions twice.
	
	ModLoader_WriteRelativeAddress((void*)0x4165C5 + 1, &MP_ResetShip);
	ModLoader_WriteRelativeAddress((void*)0x4196B2 + 1, &MP_ResetShip);
	ModLoader_WriteRelativeAddress((void*)0x419C0B + 1, &MP_ResetShip);
	
	ModLoader_WriteRelativeAddress((void*)0x419745 + 1, &MP_SetInitialShipValues);
	//+1
	
	ModLoader_WriteRelativeAddress((void*)0x41B5DE + 1, &MP_ActShipState);

	ModLoader_WriteRelativeAddress((void*)0x41B5F5 + 1, &MP_HitShipPowerup);
	ModLoader_WriteRelativeAddress((void*)0x41B5FA + 1, &MP_HitShipMap);

	ModLoader_WriteRelativeAddress((void*)0x41B669 + 1, &MP_PutPlayer);
	ModLoader_WriteRelativeAddress((void*)0x41B683 + 1, &MP_PutPlayerExtra);
	
	//SetShipInvincibility x 5
	
	//Sync ininviciblity?
	
	//SetSpeedPowerup?
	
	
	//increase shield and speed
	ModLoader_WriteByte((void*)0x4160CA, 55);
	ModLoader_WriteByte((void*)0x4160EC + 6, 1);
	
	//lives
	ModLoader_WriteByte((void*)0x4196A3 + 6, 5);
	
	//deleteme invince
	//ModLoader_WriteLong((void*)0x41613B + 6, 0xFFFFFFFF);
		
	//disable powerup reset on death
	ModLoader_WriteNOPs((void*)0x4160F6, 10);
	
	//default stage
	ModLoader_WriteByte((void*)0x419683+6, 5);

	
	ModLoader_WriteJump((void*)0x4195B3, &DisableScoreAttack_ASM); //why does this cause a crash on bullet?
	//ModLoader_WriteJump((void*)0x4135C9, (void*)0x4135DE);
	//ModLoader_WriteJump((void*)0x413431, (void*)0x41343F);

	
	//ModLoader_WriteNOPs((void*)0x41342E, 16);
	
	//ModLoader_WriteNOPs((void*)0x4135C9, 19);
	
	 
	
	//increase bullet count
	//ModLoader_WriteRelativeAddress((void*)0x4021D3 + 1, &bullets);
	//ModLoader_WriteRelativeAddress((void*)0x4021ED + 1, &bullets);
	//ModLoader_WriteRelativeAddress((void*)0x402218 + 2, &bullets);
	//ModLoader_WriteRelativeAddress((void*)0x402278 + 2, &bullets);
	/*ModLoader_WriteRelativeAddress((void*)0x402334 + 2, &bullets);
	ModLoader_WriteRelativeAddress((void*)0x402414 + 1, &bullets);
	ModLoader_WriteRelativeAddress((void*)0x40245B + 2, &bullets);
	ModLoader_WriteRelativeAddress((void*)0x4024CC + 2, &bullets);
	ModLoader_WriteRelativeAddress((void*)0x4024EE + 2, &bullets);
	ModLoader_WriteRelativeAddress((void*)0x40246C + 2, &bullets + 2);*/
	
	//ModLoader_WriteLong((void*)0x4021E6+1, 0x11850);
	/*ModLoader_WriteByte((void*)0x40220C+3, 0xFF);
	ModLoader_WriteByte((void*)0x402268+3, 0xFF);
	ModLoader_WriteByte((void*)0x402328+3, 0xFF);
	ModLoader_WriteByte((void*)0x402400+3, 0xFF);
	ModLoader_WriteByte((void*)0x40244F+3, 0xFF);
	ModLoader_WriteByte((void*)0x40244F+3, 0xFF);
	
	ModLoader_WriteByte((void*)0x4024C0+3, 0xFF);
	ModLoader_WriteByte((void*)0x4024DB+3, 0xFF);*/
	
	//ignore max bullet count per bullet for now
	ModLoader_WriteJump((void*)0x4024A2, (void*)0x4024AE);
	
	
	//ModLoader_WriteJump((void*)0x40354E, &ObjectHealthF_ASM);
	
	//powerup box
	//hanger
	//boss3 assistant
	
	
	//increase health of entities
	
	//Init multiplayer vars (ship)
	
	//
	
	//Input collector
	
	//Input packet sender
	
	//Input packet receiver
	
	//Apply to other player object
	
	//Multiplayer menu
		//Create a socket
		//Establish a connection
		
		
	
	
	//For every NPC func, get the nearest player and copy their X and Y position into memory.
}



/*
//get your packets here


PACKET_INPUT

PACKET_CONNECT

PACKET_QUIT

PACKET_READY


*/