// Mod loader for Freeware Cave Story
// Public domain

#pragma once

#include <ddraw.h>
#include <windows.h>

// Structs
#pragma pack(push,1)
typedef struct __attribute__((aligned(4))) gxObject
{
  int cond;
  int x;
  int y;
  int xm;
  int ym;
  int xoff;
  int yoff;
  int w;
  int h;
  int surf;
  int type;
  int state;
  int count1;
  int count2;
  int count3;
  int life;
  int damage;
  int flag;
  int shock;
  int score;
  int type2;
  int child;
  int rot1;
  int count4;
  int num;
  int destroyHitVoice;
  RECT rect;
} gxObject;

typedef struct __attribute__((aligned(4))) gxShip
{
  int speed;
  int cond;
  int x;
  int y;
  int xm;
  int ym;
  int xoff;
  int yoff;
  int w;
  int h;
  int surf;
  int type;
  int state;
  int count1;
  int count2;
  int count3;
  int life;
  int damage;
  int flag;
  int shock;
  int score;
  int type2;
  int child;
  int rot1;
  int count4;
  int num;
  int destroyHitVoice;
  RECT rect;
  int unused;
  int unused2;
  
  char thruster_frame;
  char invinc_flash;
  short align;
  int fire_hold_frames;
  int powerup;
  
} gxShip;

typedef struct __declspec(align(4)) gxShipExtra
{
  char thruster_frame;
  char invinc_flash;
  short align;
  int fire_hold_frames;
  int powerup;
} gxShipExtra;

typedef struct gxObjectAttr
{
  char flag;
  char padding;
  char score;
  char damage;
  char xoff;
  char yoff;
  char w;
  char h;
  short life;
  char surf;
  char destroyHitVoice;
} gxObjectAttr;

#pragma pack(pop)

const int GX_Score = 0x44C848;
const int GX_ShipSpeed = 0x44C2A8;
const int GX_Ship = 0x44C2B0;
const int GX_ShipLife = 0x44C2EC;
const int GX_ShipExtra = 0x44C330;
const int GX_ScreenOffsetW = 0x441A8C;

const int GX_ObjectAttrTbl = 0x441110;


//Funcs

static void (* const GXF_ResetShip)(void) = (void(*)(void))0x416060;
static void (* const GXF_SetInitialShipValues)(void) = (void(*)(void))0x416110;

static void (* const GXF_ActShipState)(int input) = (void(*)(void))0x4165D0;
static void (* const GXF_HitShipPowerup)(void) = (void(*)(void))0x4183C0;
static void (* const GXF_HitShipMap)(void) = (void(*)(void))0x417570;

static void (* const GXF_PutPlayer)(int screen_offsetW) = (void(*)(int))0x417960;
static void (* const GXF_PutPlayerExtra)(int screen_offsetW) = (void(*)(int))0x417610;

char* NpcFuncNames[] = {
"ActNpc000",
"ActNpc001_Explode",
"ActNpc002_CloudL",
"ActNpc003_CloudS",
"ActNpc004_Puff",
"ActNpc005_Hanger",
"ActNpc006_CloudGen",
"ActNpc007_Powerup",
"ActNpc008_Climber",
"ActNpc009_Kagome",
"ActNpc011_Wing",
"ActNpc012_WingCorpse",
"ActNpc013_Bullet",
"ActNpc014_BulletSlow",
"ActNpc010_Unused?",
"ActNpc015_Boss1",
"ActNpc016_Boss1Eye",
"ActNpc017_Boss1BigEye",
"ActNpc018_BulletLong",
"ActNpc019_BGM",
"ActNpc020_Asteroid_L",
"ActNpc021_Asteroid_S",
"ActNpc022_ScrollSpdSet",
"ActNpc023_Asteroid_SGravity",
"ActNpc024_Hanger_Wave",
"ActNpc025_Hanger_Shoot",
"ActNpc026_RockHugger",
"ActNpc027_RHAsteroidL",
"ActNpc028_RHAsteroidS",
"ActNpc029_Bullet2?",
"ActNpc030_Elka",
"ActNpc031_Sodi",
"ActNpc032_Boss2",
"ActNpc033_Boss2RocketSpawner",
"ActNpc034_B2ExplosionSpawner?",
"ActNpc035_B2Rocket",
"ActNpc036_Boss2Eye",
"ActNpc037_Stars?",
"ActNpc038_StarGen",
"ActNpc039_Elka2",
"ActNpc040_GuxtFort",
"ActNpc041_CatEye",
"ActNpc042_Slider",
"ActNpc043_Cycloid",
"ActNpc044_Chester",
"ActNpc045_Bonus",
"ActNpc046_BonusGen",
"ActNpc047_CloudXL",
"ActNpc048_Gimmick",
"ActNpc049_Boss3",
"ActNpc050_62_PowerupHelper",
"ActNpc051_B3Eye1?",
"ActNpc052_B3Eye2?",
"ActNpc053_B3Eye3?",
"ActNpc054_B3BulletBlocker?",
"ActNpc055_B3Target??",
"ActNpc056_Checkpoint",
"ActNpc057_Boss3TurretSpawner",
"ActNpc058_B3Turret",
"ActNpc059_PowerupSpinnerShield",
"ActNpc060_PowerupBox",
"ActNpc061_PowerupStatic",
"ActNpc063_BulletWhite",
"ActNpc064_PowerupSpinner",
"ActNpc065_ClimberShoot",
"ActNpc066_Tri",
"ActNpc067_GuxtTank",
"ActNpc068_MedusaEye",
"ActNpc069_GuxtMine",
"ActNpc070_BlendyGen",
"ActNpc071_Blendy",
"ActNpc072_Cycloid2",
"ActNpc073_Cycloid2Gen",
"ActNpc074_PowerupHidden",
"ActNpc075_PowerupAsteroid",
"ActNpc076_Boss4",
"ActNpc077",
"ActNpc078",
"ActNpc079",
"ActNpc080",
"ActNpc081_SpaceAmeba",
"ActNpc082",
"ActNpc083_B4Bullet",
"ActNpc084_Square",
"ActNpc085_CloudDarkL",
"ActNpc086_CloudDarkS",
"ActNpc087_CloudDarkGen",
"ActNpc088_MissilePot",
"ActNpc089",
"ActNpc090_Missile",
"ActNpc091",
"ActNpc092_BGRock",
"ActNpc093_ClearRockCloudGen",
"ActNpc094_SandStamper",
"ActNpc095",
"ActNpc096_Stamp",
"ActNpc097_Brick",
"ActNpc098_BulletSquare",
"ActNpc099_BulletSquareGen",
"ActNpc100_Boss5",
"ActNpc101",
"ActNpc102_B5Bullet",
"ActNpc103",
"ActNpc104_B5Laser",
"ActNpc105_B5LaserTrail",
"ActNpc106_AzaBonusSpawner?",
"ActNpc107_ShipShadow",
"ActNpc108_Boss6",
"ActNpc109",
"ActNpc110",
"ActNpc111_B6Laser",
"ActNpc112",
"ActNpc113",
"ActNpc114_B6Ball",
"ActNpc115_B6Caret",
"ActNpc116_CreditText",
"ActNpc117_CreditReplace",
"ActNpc118_CreditLoadImg",
"ActNpc119_CPhoto",
"ActNpc120_CreditGimmick",
"ActNpc121_BonusHidden"
};

