#include "Global.h"
cInput::cInput()
{
	m_pDI = NULL;
}

cInput::~cInput()
{
	Shutdown();
}
BOOL cInput::Shutdown()
{
	SAFE_RELEASE(m_pDI);
	return TRUE;
}
BOOL cInput::Init()
{
	Shutdown();
	DirectInput8Create(g_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDI, NULL);
	return TRUE;
}
IDirectInput8 *cInput::GetDirectInputCOM()
{
	return m_pDI;
}
cInputDevice::cInputDevice()
{
	// Clear parent cInput object pointer
	m_Input = NULL;

	// Setup device to none
	m_Type = NONE;

	// Set windowed usage to TRUE
	m_Windowed = TRUE;

	// Clear the DirectInput interface pointer
	m_pDIDevice = NULL;

	// Point the mouse and joystick structures to the state buffer
	m_MouseState = (DIMOUSESTATE*)&m_State;

	// Clear the device variables
	Clear();
}

cInputDevice::~cInputDevice()
{
	// Free a prior install
	Free();
}
BOOL cInputDevice::Free()
{
	// Unacquire and release the object
	if (m_pDIDevice != NULL) {
		m_pDIDevice->Unacquire();
		SAFE_RELEASE(m_pDIDevice);
	}

	// Set to no device installed
	m_Type = NONE;

	// Clear the data  
	Clear();

	return TRUE;
}

BOOL cInputDevice::Clear()
{
	short i;

	ZeroMemory(&m_State, 256);

	for (i = 0; i < 256; i++)
		m_Locks[i] = FALSE;

	m_XPos = 0;
	m_YPos = 0;

	return TRUE;
}
BOOL cInputDevice::Create(cInput *Input, short Type, BOOL Windowed)
{
	DIDATAFORMAT *DataFormat;


	// Free a prior device
	Free();
	m_Input = Input;
	switch (Type) {
	case KEYBOARD:
		if (FAILED(m_Input->GetDirectInputCOM()->CreateDevice(GUID_SysKeyboard, &m_pDIDevice, NULL)))
			return FALSE;
		DataFormat = (DIDATAFORMAT*)&c_dfDIKeyboard;
		break;

	case MOUSE:
		if (FAILED(m_Input->GetDirectInputCOM()->CreateDevice(GUID_SysMouse, &m_pDIDevice, NULL)))
			return FALSE;
		DataFormat = (DIDATAFORMAT*)&c_dfDIMouse;
		break;



	default: return FALSE;
	}

	// Set the windowed usage
	m_Windowed = Windowed;

	// Set the data format of keyboard
	m_pDIDevice->SetDataFormat(DataFormat);


	// Set the cooperative level - Foreground & Nonexclusive
	m_pDIDevice->SetCooperativeLevel(g_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);


	m_pDIDevice->Acquire();


	// Set the device type
	m_Type = Type;

	// Clear the device information
	Clear();

	// Return a success
	return TRUE;
}


long cInputDevice::GetXPos()
{
	return m_XPos;
}

BOOL cInputDevice::SetXPos(long XPos)
{
	m_XPos = XPos;
	return TRUE;
}

long cInputDevice::GetYPos()
{
	return m_YPos;
}

BOOL cInputDevice::SetYPos(long YPos)
{
	m_YPos = YPos;
	return TRUE;
}

long cInputDevice::GetXDelta()
{
	switch (m_Type) {
	case MOUSE:
		return m_MouseState->lX;
	default: return 0;
	}
}

long cInputDevice::GetYDelta()
{
	switch (m_Type) {
	case MOUSE:
		return m_MouseState->lY;

	default: return 0;
	}
}

BOOL cInputDevice::GetKeyState(BYTE Num)
{
	if ((m_State[Num] & 0x80))
	{
		if (m_Locks[Num] == FALSE)
		{
			return TRUE;
		}
		return FALSE;
	}
	m_Locks[Num] = FALSE;
	return FALSE;
}


BOOL cInputDevice::GetPureKeyState(BYTE Num)
{
	return ((m_State[Num] & 0x80) ? TRUE : FALSE);
}

BOOL cInputDevice::GetButtonState(BYTE Num)
{
	char State = 0;

	if (m_Type == MOUSE)
		State = m_MouseState->rgbButtons[Num];

	if (State & 0x80)   //是否被按下
	{
		if (m_Locks[Num] == FALSE)  //是否已经处理过这个消息
		{
			return TRUE;
		}
		return FALSE;
	}
	//如果没有被按下, 重置
	m_Locks[Num] = FALSE;
	return FALSE;
}

BOOL cInputDevice::GetPureButtonState(BYTE Num)
{
	char State = 0;

	if (m_Type == MOUSE)
		State = m_MouseState->rgbButtons[Num];
	if (State & 0x80)
		return TRUE;
	return FALSE;
}


BOOL cInputDevice::Acquire(BOOL Active)
{
	if (m_pDIDevice == NULL)
		return FALSE;

	if (Active == TRUE)
		m_pDIDevice->Acquire();
	else
		m_pDIDevice->Unacquire();

	return TRUE;
}

BOOL cInputDevice::Read()
{
	HRESULT hr;
	long BufferSizes[2] = { 256, sizeof(DIMOUSESTATE) };

	if (m_Type < 1 || m_Type > 3)
		return FALSE;
	for (;;) {
		// Poll
		m_pDIDevice->Poll();

		// Read in state
		if (SUCCEEDED(hr = m_pDIDevice->GetDeviceState(BufferSizes[m_Type - 1], (LPVOID)&m_State)))
			break;

		// Return on an unknown error  
		if (hr != DIERR_INPUTLOST && hr != DIERR_NOTACQUIRED)
			return FALSE;

		// Reacquire and try again
		if (FAILED(m_pDIDevice->Acquire()))
			return FALSE;
	}

	// Since only the mouse coordinates are relative, you'll
	// have to deal with them now
	if (m_Type == MOUSE) {
		// If windowed usage, ask windows for coordinates
		if (m_Windowed == TRUE) {
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(g_hWnd, &pt);
			m_XPos = pt.x;
			m_YPos = pt.y;
		}
		else {
			m_XPos += m_MouseState->lX;
			m_YPos += m_MouseState->lY;
		}
	}
	return TRUE;
}

void cInputDevice::SetLock(BYTE Num)
{
	 m_Locks[Num] = TRUE;
}
