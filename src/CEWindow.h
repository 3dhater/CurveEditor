#ifndef __CEWidnow_H__
#define __CEWidnow_H__

class CEWindow{
protected:
	int m_x = 0;
	int m_y = 0;
public:
	CEWindow(){}
	virtual ~CEWindow(){}
	int GetWindowHeight(){return m_y;}
	int GetWindowWidth(){return m_x;}
	virtual void * GetHandle() = 0;
};


#ifdef KRGUI_PLATFORM_WINDOWS
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#include <Windows.h>
	#ifdef CreateWindow
	#undef CreateWindow
	#endif

static int gClassNameNumber = 0;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

class CEWindowWin32 : public CEWindow{
	HWND m_hWnd = nullptr;
	std::u16string m_className;
	friend LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:

	CEWindowWin32(int x, int y)
	{
		m_x = x;
		m_y = y;
		m_className = u"DemoWindow";
		m_className += ++gClassNameNumber;
		DWORD style = WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SIZEBOX;
		WNDCLASSEX wc;
		ZeroMemory( &wc, sizeof( wc ) );
		wc.cbSize			= sizeof(WNDCLASSEX);
		wc.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wc.lpfnWndProc		= WndProc;
		wc.hInstance		= GetModuleHandle( 0 );
		wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground	= (HBRUSH)(COLOR_MENU+1);
		wc.lpszClassName	= (wchar_t*)m_className.c_str();
		RegisterClassEx(&wc);
		m_hWnd = CreateWindowW( (wchar_t*)m_className.c_str(),L"Window",style,0,0,x,y,nullptr,nullptr,wc.hInstance,this );
		ShowWindow( m_hWnd, SW_SHOWNORMAL  );
		SetForegroundWindow( m_hWnd );
		SetFocus( m_hWnd );
		UpdateWindow( m_hWnd );
		m_dc = GetDC(m_hWnd);
	}

	virtual ~CEWindowWin32()
	{
		if( m_hWnd )
		{
			ReleaseDC(m_hWnd,m_dc);
			DestroyWindow( m_hWnd );
		}
		UnregisterClass( (wchar_t*)m_className.c_str(), GetModuleHandle( 0 ) );
	}

	virtual void * GetHandle(){return m_hWnd;}
	HDC m_dc = nullptr;
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	CEWindowWin32* pD = nullptr;
	int wmId    = LOWORD(wParam);
	if( message == WM_NCCREATE ){
		pD = static_cast<CEWindowWin32*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		SetLastError(0);
		if( !SetWindowLongPtr(hWnd, -21, reinterpret_cast<LONG_PTR>(pD)) ){
			if( GetLastError() != 0 ) return FALSE;
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}else pD = reinterpret_cast<CEWindowWin32*>(GetWindowLongPtr(hWnd, -21));

	switch( message )
	{
	case WM_QUIT:
	case WM_CLOSE:
	case WM_DESTROY:
			gSystemRun = false;
	return 0;
	//case WM_MOUSEWHEEL:
	//	if(pD)
	//	{
	//		if( pD->m_focus )
	//		{
	//			ev.type = kkEventType::MouseWheel;
	//			ev.mouseEvent.state = 0u;
	//			ev.mouseEvent.wheel = int( (f32)GET_WHEEL_DELTA_WPARAM(wParam) / (f32)WHEEL_DELTA );
	//			kkGetMainSystem()->addEvent( ev );
	//		}
	//	}
	//	return 0;
	//case WM_LBUTTONDBLCLK:
	//case WM_RBUTTONDBLCLK:
	//case WM_LBUTTONDOWN:
	//case WM_LBUTTONUP:
	//case WM_RBUTTONDOWN:
	//case WM_RBUTTONUP:
	//case WM_MBUTTONDOWN:
	//case WM_MBUTTONUP:
	//case WM_MOUSEMOVE:
	//{
	//	ev.type = kkEventType::Mouse;
	//	ev.mouseEvent.state = 0u;

	//	static f32 crdX = 0.f;
	//	static f32 crdY = 0.f;
	//	if(pD)
	//	{
	//		if( pD->m_focus )
	//		{
	//			POINT point;
	//			GetCursorPos(&point);
	//			ScreenToClient(hWnd,&point);
	//			crdX = (float)point.x;
	//			crdY = (float)point.y;
	//		}
	//	}
	//	/*ev.mouseEvent.x = LOWORD(lParam);
	//	ev.mouseEvent.y = HIWORD(lParam);*/
	//	ev.mouseEvent.x = crdX;
	//	ev.mouseEvent.y = crdY;

	//	if( wParam & MK_LBUTTON )		ev.mouseEvent.state |= kkEventMouse::kkEventMouseState::MS_LMB_DOWN;
	//	if( wParam & MK_RBUTTON )		ev.mouseEvent.state |= kkEventMouse::kkEventMouseState::MS_RMB_DOWN;
	//	if( wParam & MK_MBUTTON )		ev.mouseEvent.state |= kkEventMouse::kkEventMouseState::MS_MMB_DOWN;
	//	if( wParam & MK_XBUTTON1 )
	//	{
	//		ev.mouseEvent.state |= kkEventMouse::kkEventMouseState::MS_X1MB_DOWN;
	//		switch( message )
	//		{
	//			case WM_XBUTTONUP: ev.mouseEvent.state |= kkEventMouse::kkEventMouseState::MS_X1MB_UP; break;
	//			case WM_XBUTTONDBLCLK: ev.mouseEvent.state |= kkEventMouse::kkEventMouseState::MS_X1MB_DOUBLE; break;
	//		}
	//	}
	//	if( wParam & MK_XBUTTON2 )
	//	{
	//		ev.mouseEvent.state |= kkEventMouse::kkEventMouseState::MS_X2MB_DOWN;
	//		switch( message )
	//		{
	//			case WM_XBUTTONUP: ev.mouseEvent.state |= kkEventMouse::kkEventMouseState::MS_X2MB_UP; break;
	//			case WM_XBUTTONDBLCLK: ev.mouseEvent.state |= kkEventMouse::kkEventMouseState::MS_X2MB_DOUBLE; break;
	//		}
	//	}

	//	switch( message )
	//	{
	//	case WM_LBUTTONUP:{	
	//		ev.mouseEvent.state    |= kkEventMouse::kkEventMouseState::MS_LMB_UP; 
	//	}break;
	//		case WM_RBUTTONUP:	ev.mouseEvent.state    |= kkEventMouse::kkEventMouseState::MS_RMB_UP; break;
	//		case WM_MBUTTONUP:	ev.mouseEvent.state    |= kkEventMouse::kkEventMouseState::MS_MMB_UP; break;
	//		case WM_LBUTTONDBLCLK: ev.mouseEvent.state |= kkEventMouse::kkEventMouseState::MS_LMB_DOUBLE; break;
	//		case WM_RBUTTONDBLCLK: ev.mouseEvent.state |= kkEventMouse::kkEventMouseState::MS_RMB_DOUBLE; break;
	//		case WM_MBUTTONDBLCLK: ev.mouseEvent.state |= kkEventMouse::kkEventMouseState::MS_MMB_DOUBLE; break;
	//	}


	//	kkGetMainSystem()->addEvent( ev );

	//	return 0;
	//}break;

	//case WM_SYSKEYDOWN:
	//case WM_SYSKEYUP:
	//case WM_KEYDOWN:
	//case WM_KEYUP:
	//{
	//	ev.type = kkEventType::Keyboard;
	//	ev.keyboardEvent.key = static_cast<kkKey>( wParam );

	//	if(message == WM_SYSKEYDOWN){ ev.keyboardEvent.state_is_pressed = 1; }
	//	if(message == WM_KEYDOWN){ev.keyboardEvent.state_is_pressed = 1; }
	//	if(message == WM_SYSKEYUP){  ev.keyboardEvent.state_is_relesed = 1; }
	//	if(message == WM_KEYUP){ ev.keyboardEvent.state_is_relesed = 1; }

	//	const UINT MY_MAPVK_VSC_TO_VK_EX = 3;

	//	if( ev.keyboardEvent.key == kkKey::K_SHIFT )
	//	{ // shift -> lshift rshift
	//		ev.keyboardEvent.key = static_cast<kkKey>( MapVirtualKey( ( static_cast<UINT>( lParam >> 16) & 255u ), MY_MAPVK_VSC_TO_VK_EX ) );
	//		ev.keyboardEvent.state_is_shift = TRUE;
	//	}
	//	if( ev.keyboardEvent.key == kkKey::K_CTRL )
	//	{ // ctrl -> lctrl rctrl
	//		ev.keyboardEvent.key = static_cast<kkKey>( MapVirtualKey( ( static_cast<UINT>( lParam >> 16) & 255 ), MY_MAPVK_VSC_TO_VK_EX ) );
	//		ev.keyboardEvent.state_is_ctrl = TRUE;
	//		if( lParam & 0x1000000 )
	//		{
	//			ev.keyboardEvent.key = static_cast<kkKey>( 163 );
	//		}
	//	}

	//	

	//	if( ev.keyboardEvent.key == kkKey::K_ALT )
	//	{ // alt -> lalt ralt
	//		ev.keyboardEvent.key = static_cast<kkKey>( MapVirtualKey( ( static_cast<UINT>( lParam >> 16) & 255 ), MY_MAPVK_VSC_TO_VK_EX ) );
	//		if( lParam & 0x1000000 )
	//		{
	//			ev.keyboardEvent.key = static_cast<kkKey>(165);
	//		}
	//		//printf("alt = %i\n",(int)ev.keyboardEvent.key);
	//		ev.keyboardEvent.state_is_alt = TRUE;
	//	}

	//	

	//	u8 keys[ 256u ];
	//	GetKeyboardState( keys );
	//	WORD chars[ 2 ];

	//	if( ToAsciiEx( (UINT)wParam, HIWORD(lParam), keys, chars, 0, KEYBOARD_INPUT_HKL ) == 1 )
	//	{
	//		WORD uChar;
	//		MultiByteToWideChar( KEYBOARD_INPUT_CODEPAGE, MB_PRECOMPOSED, (LPCSTR)chars,
	//			sizeof(chars), (WCHAR*)&uChar, 1 );
	//		ev.keyboardEvent.character = uChar;
	//	}
	//	else 
	//	{
	//		ev.keyboardEvent.character = 0;
	//	}

	//	kkGetMainSystem()->addEvent( ev );

	//	if( message == WM_SYSKEYDOWN || message == WM_SYSKEYUP )
	//	{
	//		return DefWindowProc( hWnd, message, wParam, lParam );
	//	}
	//	else
	//	{
	//		return 0;
	//	}
	//}break;
	case WM_SYSCOMMAND:
		if( ( wParam & 0xFFF0 ) == SC_SCREENSAVE ||
			( wParam & 0xFFF0 ) == SC_MONITORPOWER ||
			( wParam & 0xFFF0 ) == SC_KEYMENU)
			return 0;
	}
	return DefWindowProc( hWnd, message, wParam, lParam );
}
#else
#error For Windows
#endif

#endif