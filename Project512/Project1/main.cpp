#include "global.h"
string g_strMediaPath="";
int g_FPS = 60;
int g_xOffset=0;
int g_yOffset=0;
CMainState* g_pMainState;
cCharacter* g_pCharacter;
cObj* g_pHeroObj;
HBRUSH  g_redbrush;
HBRUSH  g_blackbrush;
HBRUSH  g_whitebrush;
HBRUSH  g_bluebrush;
HPEN  g_WhitePen;
HPEN  g_BWhitePen;
int g_xMouse;
int g_yMouse;
//HANDLE  g_hMutex;
//CRITICAL_SECTION g_cs;
WNDCLASSEX g_wcex;
BOOL g_bFillScreen;
int g_StateType;
HDC g_DC;
HDC g_DC2;
HDC g_ShowDC;
HBITMAP g_MemBitmap;
LPDIRECTSOUND8	 g_pDirectSound=0;

cScript* g_pScriptParent = 0;
cScript* g_pFirst = 0;
long  g_NumScriptActions = 0;


BITMAPINFO * g_pBmpinfo;

 string g_ScriptPath; //�ű���·��
 string g_MapPath;  //��ͼ��·��
HWND  g_hWnd;
HINSTANCE g_hInst;
void  Resize(long Width, long Height);

int g_iFPS;

LRESULT CALLBACK  WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
static DWORD UpdateTimer = timeGetTime();
static DWORD CurrentTime = 0;
int nScreenWidth;
int nScreenHeight;
void SetFullScreen(BOOL bFill);
int PASCAL WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
#ifdef TESTDEBU
	g_strMediaPath = "..\\..\\Media\\";
#else
	g_strMediaPath = "Media\\";
#endif
	g_ScriptPath = g_strMediaPath;
	g_MapPath = g_strMediaPath;
	g_ScriptPath += "script\\1\\"; //�ű���·��
	g_MapPath += "scene\\";  //��ͼ��·��
	//
	nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	//��ʼ�����ǵĳ��� 
	g_hInst = hInstance;
	//InitializeCriticalSection(&g_cs);
//	g_hMutex=CreateMutex(0, 0, 0);

	g_bFillScreen = FALSE;
	//��ʼ��λͼͷ
	g_pBmpinfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO)+sizeof(RGBQUAD)* 2);
	g_pBmpinfo->bmiHeader.biPlanes = 1;
	g_pBmpinfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	g_pBmpinfo->bmiHeader.biCompression = 3;
	g_pBmpinfo->bmiHeader.biBitCount = 16;
	g_pBmpinfo->bmiHeader.biClrUsed = 0;
	g_pBmpinfo->bmiHeader.biClrImportant = 0;
	g_pBmpinfo->bmiHeader.biXPelsPerMeter = 0;
	g_pBmpinfo->bmiHeader.biYPelsPerMeter = 0;
	DWORD colorbit = 0xf800;
	memmove(&g_pBmpinfo->bmiColors[0], &colorbit, sizeof(RGBQUAD));
	colorbit = 0x7e0;
	memmove(&g_pBmpinfo->bmiColors[1], &colorbit, sizeof(RGBQUAD));
	colorbit = 0x1f;
	memmove(&g_pBmpinfo->bmiColors[2], &colorbit, sizeof(RGBQUAD));
	////��ƴ���
		g_wcex.cbClsExtra = 0;
		g_wcex.cbSize = sizeof(g_wcex);
		g_wcex.cbWndExtra = 0;
		g_wcex.hbrBackground = 0;
		g_wcex.hCursor = LoadCursor(0, IDC_ARROW);
		g_wcex.hIcon = LoadIcon(0, IDI_APPLICATION);
		g_wcex.hIconSm = g_wcex.hIcon;
		g_wcex.lpszClassName = L"����";
		g_wcex.hInstance = hInstance;
		g_wcex.lpfnWndProc = WindowProc;	
		g_wcex.lpszMenuName = 0;
		g_wcex.style = CS_CLASSDC;
		//ע�ᴰ��
		if (!RegisterClassEx(&g_wcex))return 0;

		//��������
		g_hWnd = CreateWindow(
			g_wcex.lpszClassName,  //����
			L"�λ�Ⱥ����5",
			WS_CAPTION | WS_SYSMENU|WS_MINIMIZEBOX,//, //���ڵ����ͣ��б���Ͳ˵�
			0, 0, 640, 480,  //���ڵĴ�С
			0, 0,          //�����ڵľ��  �˵��ľ��
			g_hInst,   // Ӧ�ó���ľ��
			0
			);
		if (!g_hWnd)ERRBOX;
		Resize(640, 480);
		//�Ա괰�ڲ����´���
		ShowWindow(g_hWnd, SW_SHOW);
		UpdateWindow(g_hWnd);
		
	// ��ʼCOM
	CoInitialize(NULL);

	CWFile m_File;

	{
		vector<string> wdfpath;
		wdfpath.resize(33);
		for (int i = 0; i < 33; i++)
		{
			wdfpath[i] = g_strMediaPath;
		}
		wdfpath[0] += "shape.wda";
		wdfpath[1] += "shape.wdb";
		wdfpath[2] += "shape.wdc";
		wdfpath[3] += "shape.wdf";
		wdfpath[4] += "shape1.wdf";
		wdfpath[5] += "shape.wd1";
		wdfpath[6] += "shape.wd2";
		wdfpath[7] += "shape.wd3";
		wdfpath[8] += "shape.wd4";
		wdfpath[9] += "shape.wd5";
		wdfpath[10] += "shape.wd6";
		wdfpath[11] += "shape.wd7";
		wdfpath[12] += "shape.wd8";
		wdfpath[13] += "shape.wd9";
		wdfpath[14] += "shape.wd11";
		wdfpath[15] += "wzife.wd1";
		wdfpath[16] += "wzife.wd2";
		wdfpath[17] += "wzife.wd3";
		wdfpath[18] += "wzife.wd4";
		wdfpath[19] += "wzife.wdf";
		wdfpath[20] += "smap.wdf";
		wdfpath[21] += "item.wdf";
		wdfpath[22] += "item.wd1";
		wdfpath[23] += "item.wd2";
		wdfpath[24] += "addon.wdf";
		wdfpath[25] += "magic.wdf";
		wdfpath[26] += "magic.wd1";
		wdfpath[27] += "magic.wd2";
		wdfpath[28] += "waddon.wdf";  //״̬
		wdfpath[29] += "sound.wdf";
		wdfpath[30] += "sound1.wdf";
		wdfpath[31] += "mapani.wdf";
		wdfpath[32] += "atom.wdf";
		for (int i = 0; i < 33; i++)
			m_File.Open(wdfpath[i].c_str());
	}
	CMainState* MainState;
	MainState = new CMainState;
	cFightState FightState;
	cEndState EndState;
	cMenuState MenuState;
	g_pMainState = MainState;
	MainState->m_pEndState = &EndState;
	MainState->m_pMenuState = &MenuState;
	g_ShowDC = GetDC(g_hWnd);
	g_DC = CreateCompatibleDC(g_ShowDC);
	g_DC2 = CreateCompatibleDC(g_ShowDC);
	HPEN  oldpen = (HPEN)SelectObject(g_DC2, g_WhitePen);
	COLORREF oldcolor = SetTextColor(g_DC2, RGB(255, 255, 255));



	g_WhitePen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	g_BWhitePen = CreatePen(PS_SOLID, 4, RGB(255, 255, 255));
	
	//�����ı��Ļ��ģʽ
	SetBkMode(g_DC2, TRANSPARENT);
	//SetBkColor(g_DC2, RGB(8, 0, 0));

	g_MemBitmap = CreateCompatibleBitmap(g_ShowDC, 640, 480);
	HBITMAP oldmap = (HBITMAP)SelectObject(g_DC, g_MemBitmap);
	g_StateType =  STATE_MAIN; //STATE_MENU;//

	////��������
	{
		string path;
		wstring path2;
		path = g_strMediaPath;
		path += "font\\hyh1gjm.ttf";
		StringToWString(path, path2);
		AddFontResourceEx(path2.c_str(), FR_PRIVATE, 0);
		path = g_strMediaPath;
		path += "font\\hyb1gjm.ttf";
		StringToWString(path, path2);
		AddFontResourceEx(path2.c_str(), FR_PRIVATE, 0);
		path = g_strMediaPath;
		path += "font\\hyj4gjm.ttf";
		StringToWString(path, path2);
		AddFontResourceEx(path2.c_str(), FR_PRIVATE, 0);
		path = g_strMediaPath;
		path += "font\\hksnt.ttf";
		StringToWString(path, path2);
		AddFontResourceEx(path2.c_str(), FR_PRIVATE, 0);
		path = g_strMediaPath;
		path += "font\\������ͷ�����.ttf";
		StringToWString(path, path2);
		AddFontResourceEx(path2.c_str(), FR_PRIVATE, 0);
	}
	
	MainState->m_pFightState = &FightState;
	MainState->m_SkillManager.m_pFightState = &FightState;

	int addtime =1000 / g_FPS;

	ShowCursor(false);
	SetBkMode(g_DC, TRANSPARENT);//�����ı��Ļ��ģʽ
	//��Ϣѭ��
	MSG Msg;
	ZeroMemory(&Msg, sizeof(MSG));
	for (;;)
	{
		if (PeekMessage(&Msg, g_hWnd, 0, 0, PM_REMOVE))
		{	
			if (Msg.message == WM_QUIT)break;
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
	//	else
		{
			if (timeGetTime() < UpdateTimer + addtime)
			{
				Sleep(1);   //��CPU��Ϣһ�� 
				continue;
			}
			UpdateTimer = timeGetTime();
			CurrentTime += addtime;
			g_iFPS += 1;
			if (g_StateType == STATE_MAIN)
			{
				MainState->Frame();
			}
			else
			if (g_StateType == STATE_FIGHT)
			{
				FightState.Frame();
			}
			else
			if (g_StateType == STATE_MENU)
			{
				MenuState.Frame();
			}
			else
			if (g_StateType == STATE_GAMEOVER)
			{
				EndState.Frame();
			}
			else
			{
				PostQuitMessage(0);
				break;
			}
		}
	}
	ShowCursor(TRUE);

	{
		string path;
		wstring path2;
		path = g_strMediaPath;
		path += "font\\hyh1gjm.ttf";
		StringToWString(path, path2);
		RemoveFontResourceEx(path2.c_str(), FR_PRIVATE, 0);
		path = g_strMediaPath;
		path += "font\\hyb1gjm.ttf";
		StringToWString(path, path2);
		RemoveFontResourceEx(path2.c_str(), FR_PRIVATE, 0);
		path = g_strMediaPath;
		path += "font\\hyj4gjm.ttf";
		StringToWString(path, path2);
		RemoveFontResourceEx(path2.c_str(), FR_PRIVATE, 0);
		path = g_strMediaPath;
		path += "font\\hksnt.ttf";
		StringToWString(path, path2);
		RemoveFontResourceEx(path2.c_str(), FR_PRIVATE, 0);
		path = g_strMediaPath;
		path += "font\\������ͷ�����.ttf";
		StringToWString(path, path2);
		RemoveFontResourceEx(path2.c_str(), FR_PRIVATE, 0);
	}

	
	SelectObject(g_DC2, oldpen);
	DeleteObject(g_WhitePen);
	DeleteObject(g_BWhitePen);
	SetTextColor(g_DC2, oldcolor);
	MainState->Free();
	FightState.Free();
	free(g_pBmpinfo);
	SelectObject(g_DC, oldmap);
	DeleteObject(g_MemBitmap);
	m_File.CloseWDF();
	m_File.Free();
	DeleteDC(g_DC);
	//DeleteDC(g_DC2);
	//DeleteCriticalSection(&g_cs);
	//CloseHandle(g_hMutex);
	SAFE_DELETE(MainState);
	UnregisterClass(g_wcex.lpszClassName, hInstance);
	PostQuitMessage(1);
	return 0;
}


LRESULT CALLBACK  WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		g_StateType = 10;
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void Resize(long Width, long Height)
{
	RECT WndRect, ClientRect;
	long WndWidth, WndHeight;

	GetWindowRect(g_hWnd, &WndRect);
	GetClientRect(g_hWnd, &ClientRect);

	WndWidth = (WndRect.right - (ClientRect.right - Width)) - WndRect.left;
	WndHeight = (WndRect.bottom - (ClientRect.bottom - Height)) - WndRect.top;

	MoveWindow(g_hWnd, WndRect.left, WndRect.top, WndWidth, WndHeight, TRUE);


}


void SetFullScreen(BOOL bFill)
{

	if (bFill)												// ȫ��ģʽ
	{
		
		DEVMODE dmScreenSettings;								// �豸ģʽ
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	// ȷ���ڴ����Ϊ��

		dmScreenSettings.dmSize = sizeof(dmScreenSettings);		// DEVMODE�ṹ�Ĵ�С

		dmScreenSettings.dmPelsWidth = 640;					// ��ѡ��Ļ���
		dmScreenSettings.dmPelsHeight = 480;					// ��ѡ��Ļ�߶�
		dmScreenSettings.dmBitsPerPel = 32;		// ÿ������ѡ��ɫ�����
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// ����������ʾģʽ�����ؽ����ע:CDS_FULLSCREEN ��ȥ��״̬��
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			ERRBOX;
		}
		LONG styleValue = ::GetWindowLong(g_hWnd, GWL_STYLE);
		styleValue &= ~WS_CAPTION;
		::SetWindowLong(g_hWnd, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

		SetWindowPos(g_hWnd, 0, 0,0,640,480, SWP_FRAMECHANGED);
	}
	else
	{//���ڻ�
		DEVMODE dmScreenSettings;								// �豸ģʽ
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	// ȷ���ڴ����Ϊ��
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);		// DEVMODE�ṹ�Ĵ�С
		dmScreenSettings.dmPelsWidth = nScreenWidth;					// ��ѡ��Ļ���
		dmScreenSettings.dmPelsHeight = nScreenHeight;					// ��ѡ��Ļ�߶�
		dmScreenSettings.dmBitsPerPel = 32;		// ÿ������ѡ��ɫ�����
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		// ����������ʾģʽ�����ؽ����ע:CDS_FULLSCREEN ��ȥ��״̬��
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			ERRBOX;
		}

		::SetWindowLong(g_hWnd, GWL_STYLE, GetWindowLong(g_hWnd, GWL_STYLE)
			| WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
		::SetWindowPos(g_hWnd, NULL, (nScreenWidth >> 1) - 320, (nScreenHeight >> 1) - 240, 640, 480,  SWP_FRAMECHANGED);
		Resize(640, 480);
	}
	
	
}
