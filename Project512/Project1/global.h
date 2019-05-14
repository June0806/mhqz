#pragma once
#include <windows.h>
#define DIRECTINPUT_VERSION 0x0800
#include "dinput.h"
//#include <d3d9.h>                   // D3D头文件
//#include <d3dx9.h>                  // D3DX库头文件
//#include <DSound.h>
//#include <dshow.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include <list>
#include <vector>
#include "time.h"
#include <string>
#include <process.h>

#include <stdio.h>
#include <fstream>
#include <sstream>
#include "Input.h"
#include <math.h>
#include "MyMath.h"

#define  TESTDEBU
#ifdef TESTDEBU
#include <vld.h>
#endif
#include "ChangeColor.h"
#include "mysound.h"
#include "MyScript.h"
#include "A_Start.h"
#include "Object.h"
#include "PetData.h"
#include "item.h"
#include "Skill.h"

#include "Interface.h"
#include "Interface2.h"
#include "Script.h"
#include "Map.h"

#include "AI.h"
#include "trigger.h"
#include "Canvas.h"
#include "trigger2.h"
#include "State.h"

extern LPDIRECTSOUND8	 g_pDirectSound;
extern string g_strMediaPath;
extern int g_xOffset;
extern int g_yOffset;
//临界区
//extern  CRITICAL_SECTION g_cs;
//extern HANDLE g_hMutex;
//位图头
extern BITMAPINFO * g_pBmpinfo;
//内存DC
extern HDC g_ShowDC;
extern HDC g_DC;
extern HDC g_DC2;
extern HBITMAP g_MemBitmap;
//extern HBITMAP g_MemBitmap2;
extern HBRUSH  g_redbrush;
extern HBRUSH  g_blackbrush;
extern HBRUSH  g_whitebrush;
extern HBRUSH  g_bluebrush;
extern HPEN  g_WhitePen;
extern HPEN  g_BWhitePen;
extern BOOL g_bFillScreen;
extern int nScreenWidth;
extern int nScreenHeight;
extern void SetFullScreen(BOOL bFill);
//路径
extern  string g_ScriptPath;
extern  string g_MapPath;
//窗口句柄
extern WNDCLASSEX g_wcex;
extern HWND  g_hWnd;
extern HINSTANCE g_hInst;
//脚本
extern cScript* g_pScriptParent;
extern cScript* g_pFirst;

// jpg
#ifdef  __cplusplus
extern "C"
{
#endif
#include "jpeglib/jpeg.h"
#include "jpeglib/JPEGINT.H"
//#include "jpeglib/RGB.C"
#ifdef  __cplusplus
}
#endif

//#pragma comment(lib,"..\\Lib\\x86\\d3d9.lib")
//#pragma comment(lib,"..\\Lib\\x86\\d3dx9.lib")
//#pragma comment(lib,"Lib\x86\Dsound.lib")
//#pragma comment(lib,"strmiids.lib")
//#pragma comment(lib,"Msacm32.lib")


#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma warning(disable:4996) 
#pragma warning(disable:4995) 
#pragma warning(disable:4018) 
//宏
#define __DEBUG__AACC_  
#ifdef __DEBUG__AACC_  
#pragma comment (lib,"jpeglib\\debug\\jpeglib.lib")
#endif


#define SAFE_DELETE(p) if (p) {delete p; p = NULL;}  
#define SAFE_DELETE_ARRAY(p) if (p) {delete[] p; p = NULL;}  
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif



#define ERRBOX {MessageBoxA(NULL,__FUNCTION__, "ERR",MB_OK);}// assert(0);
#define ERRBOX2(num) {char data[20]; itoa(num,data,10); MessageBoxA(NULL,__FUNCTION__, data,MB_OK);}// assert(0);
//8个方向
#define DIRECTION_UP		 8
#define DIRECTION_DOWN		 2
#define DIRECTION_RIGHT		 6
#define DIRECTION_LEFT		 4
#define DIRECTION_LEFT_UP	 7
#define DIRECTION_RIGHT_UP	 9
#define DIRECTION_LEFT_DOWN  1
#define DIRECTION_RIGHT_DOWN 3

//#define OBJ_TYPE_PC       1
//#define OBJ_TYPE_NPC      2
//#define OBJ_TYPE_PET     3


#define POS_STAND    1
#define POS_WALK     2
#define POS_FIGHT_STAND 3
#define POS_RUN       4
#define POS_ATTACK       5
#define POS_MAGIC       6
#define POS_DEF       7
#define POS_SUFFER       8
#define POS_DEAD       9
#define POS_RETURN       10
#define POS_ATTACK2      11
#define POS_GREET      12
#define POS_DANCE      13
#define POS_BOW      14
#define POS_ANGRY      15
#define POS_CRY      16
#define POS_SIT      17
#define POS_BIGHEAD      18
#define POS_DIALOG      19
#define POS_SMALLHEAD    20

#define POS_SOUNDATTACK       21
#define POS_SOUNDMAGIC       22
#define POS_SOUNDDEF       23
#define POS_SOUNDSUFFER       24
#define POS_SOUNDDEAD       25
#define POS_SOUNDATTACK2      26



extern int g_StateType;
extern int g_FPS;
#define STATE_FIGHT 2
#define STATE_MAIN  1
#define STATE_EXT  3
#define STATE_MENU 4
#define STATE_GAMEOVER 5

#define MAX_NPC_NUM		60
#define WFourCC(str) ((DWORD)str[0]|(DWORD)str[1]<<8|(DWORD)str[2]<<16|(DWORD)str[3]<<24)

#define WAlloc(size) ((void*)new BYTE[(size)])
#define WFree(ptr) (delete []((BYTE*)(ptr)))
extern CMainState* g_pMainState;

extern cCharacter* g_pCharacter;
extern cObj* g_pHeroObj;
extern int g_xMouse;
extern int g_yMouse;
