#pragma once
#define MOUSE_LBUTTON          0
#define MOUSE_RBUTTON          1
#define MOUSE_MBUTTON          2
enum InputDevices {
	NONE = 0,
	KEYBOARD,
	MOUSE,
};
class cInput
{
protected:

	IDirectInput8 *m_pDI;

public:
	cInput();
	~cInput();
	IDirectInput8 *GetDirectInputCOM();
	BOOL Init();
	BOOL Shutdown();
};
class cInputDevice
{
public:
	cInput               *m_Input;

	short                 m_Type;
	IDirectInputDevice8  *m_pDIDevice;

	BOOL                  m_Windowed;

	char                  m_State[256];
	DIMOUSESTATE         *m_MouseState;


	BOOL                  m_Locks[256];

	long                  m_XPos, m_YPos;



public:
	cInputDevice();
	~cInputDevice();

	IDirectInputDevice8 *DeviceCOM();

	// Generic functions - all devices
	BOOL Create(cInput *Input, short Type, BOOL Windowed = TRUE);
	BOOL Free();

	BOOL Clear();
	BOOL Read();
	BOOL Acquire(BOOL Active = TRUE);



	long GetXPos();
	BOOL SetXPos(long XPos);
	long GetYPos();
	BOOL SetYPos(long YPos);
	long GetXDelta();
	long GetYDelta();

	// Keyboard specific functions
	BOOL  GetKeyState(BYTE Num);
	BOOL  GetPureKeyState(BYTE Num);

	// Mouse/Joystick specific functions
	BOOL  GetButtonState(BYTE Num);
	BOOL  GetPureButtonState(BYTE Num);
	void  SetLock(BYTE Num);
	void  UnSetLock(BYTE Num){ m_Locks[Num] = FALSE; }

};