#include "stdafx.h"
#include "War3Window.h"
#include "Event.h"
#include "Game.h"
#include "Tools.h"
#include "Input.h"
#include "Offsets.h"
#include "GameEventObserver.h"
#include "UI.h"
#include "Version.h"
#include "IUIObject.h"
#include "Timer.h"

static const uint32_t MAX_WNDPROC = 32;
static uint32_t WndProcCount = 0;
static CustomWndProc WndProcs[ MAX_WNDPROC ] = { 0 };

static void CallWndProcs( HWND win, UINT message, WPARAM wParam, LPARAM lParam ) {
	for ( uint32_t i = 0; i < MAX_WNDPROC && WndProcs[ i ]; ++i ) {
		if ( WndProcs[ i ]( win, message, wParam, lParam ) )
			break;
	}
}

void AddWindowProc( CustomWndProc proc ) {
	if ( WndProcCount < MAX_WNDPROC )
		WndProcs[ WndProcCount++ ] = proc;
}

namespace DEVICE_TYPE {
	static const int D3D = 1;
	static const int OpenGL = 2;
}

static int DeviceType = 0;
static war3::CGxDevice *DeviceObject = NULL;

static bool Locked;
static RECT OldRect;
static RECT LockRect;
static RECT ClientAreaScreenRect;
static POINT Point;
static RECT OldWindowRect;
static RECT OldClientRect;
static LONG DefaultWindowStyle;
static LONG DefaultWindowExStyle;
static RECT TaskBarRect;

static bool EnableImprovedWindow;
static bool Fullscreen;
static float WidthRatioX = 1.0f;
static float WidthRatioY = 1.0f;
//static bool KeepRatio;
HWND Hwnd = NULL;

HWND GetWar3Window( ) { return Hwnd; }

typedef LRESULT( __fastcall *WAR3WNDPROC )( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

WAR3WNDPROC OldWndProc;

bool UpdateClipCursor( );
void UpdateFullscreen( WORD width = 0, WORD height = 0 );
void HookReady( );

void D3DSetStageSize( float width, float height ) {
	war3::CGxDeviceD3d * device = reinterpret_cast< war3::CGxDeviceD3d* >( DeviceObject );
	device->unk_6C0 = 0.f;
	device->unk_6C4 = 0.f;
	device->windowWidth = static_cast< float >( width );
	device->windowHeight = static_cast< float >( height );
	device->unk_6D0 = 1;
	/*
	if (*reinterpret_cast<DWORD*>(0x6FA9E7A0))
		*reinterpret_cast<DWORD*>(Offset(GLOBAL_UPDATE_STAGE_SIZE)) = MAKELPARAM(static_cast<WORD>(width), static_cast<WORD>(height));
	*/
}

void RefreshUICallback( Timer *tm )
{
	UI_Refresh( );
}

static POINTS LastMousePoint, MousePoint;
LRESULT __fastcall DetourWindowProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
)
{
	static bool Inited = false;
	RECT orgRect;

	if ( !Inited ) {
		if ( TlsGetValue( GameTlsIndexGet( ) ) ) {
			Inited = true;
			if ( !Hwnd ) {
				Hwnd = hwnd;
				HookReady( );
			}
			if ( hwnd != Hwnd ) {
				Hwnd = hwnd;
				HookReady( );
			}
		}
	}

	if ( uMsg == WM_SIZE )
	{
		// (x / y) / (4 / 3 ) = (640 / 480) / ( 4 / 3) = 
		WidthRatioX = ( ( float )LOWORD( lParam ) / ( float )HIWORD( lParam ) ) / ( 4.f / 3.f );
		// (y / x) / (3 / 4 ) = (480 / 640) / ( 3 / 4) = 
		WidthRatioY = ( ( float )HIWORD( lParam ) / ( float )LOWORD( lParam ) ) / ( 3.f / 4.f );

		GetTimer( 0.05, RefreshUICallback )->start( );
	}
	//else if (EnableImprovedWindow && Inited) {
	//	switch (uMsg) {
	//	case WM_SYSCOMMAND:
	//		if (wParam == SC_KEYMENU)
	//			return 0;
	//		break;
	//	case WM_SYSKEYDOWN:
	//	case WM_SYSKEYUP:
	//		break;
	//	case WM_ACTIVATEAPP:
	//		if (wParam == TRUE) {
	//			if (Locked) {
	//				UpdateClipCursor();
	//			}
	//			if (Fullscreen) {
	//				GetWindowRect(Hwnd, &orgRect);
	//				SetWindowPos(Hwnd, HWND_TOPMOST, orgRect.left, orgRect.top, orgRect.right - orgRect.left, orgRect.bottom - orgRect.top, SWP_SHOWWINDOW);
	//			}
	//		}
	//		else {
	//			ClipCursor(&OldRect);
	//			if (Fullscreen) {
	//				GetWindowRect(Hwnd, &orgRect);
	//				SetWindowPos(Hwnd, HWND_NOTOPMOST, orgRect.left, orgRect.top, orgRect.right - orgRect.left, orgRect.bottom - orgRect.top, SWP_NOACTIVATE);
	//			}
	//		}
	//		break;
	//	case WM_SIZING:
	//		break;
	//	case WM_SIZE:
	//		if (!Fullscreen && (wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED)) {

	//		}
	//		if (Locked) {
	//			UpdateClipCursor();
	//		}

	//		WidthRatioX = ((float)LOWORD(lParam) / (float)HIWORD(lParam)) / (4.f / 3.f);
	//		//OutputScreen(1, "widthratio = %.3f", WidthRatioX);

	//		GetTimer(0.05, RefreshUICallback)->start();
	//		break;
	//	case WM_MOVE:
	//		if (!Fullscreen) {
	//			GetWindowRect(Hwnd, &orgRect);
	//		}
	//		if (Locked) {
	//			UpdateClipCursor();
	//		}
	//		break;
	//	case WM_DISPLAYCHANGE:
	//		if (Fullscreen)
	//			UpdateFullscreen();
	//		break;
	//	}
	//}
	//TODO 把上面的switch换成不同的callback

	CallWndProcs( hwnd, uMsg, wParam, lParam );

	return OldWndProc( hwnd, uMsg, wParam, lParam );
}

__declspec( noinline ) void War3Window_Init( ) {
	//检测设备类型
	DeviceObject = GxDeviceGet( );
	DeviceType = RTTIClassNameGet( DeviceObject )[ 9 ] == 'D' ? DEVICE_TYPE::D3D : DEVICE_TYPE::OpenGL;
#ifdef _DEBUG
	;//OutputDebug("DeviceObject 0x%08X vtable = 0x%08X, window = 0x%08X\n", DeviceObject, VTBL(DeviceObject), dreamaero::offset_element_get<void*>(DeviceObject, 0x57C));
	HWND win = dreamaero::offset_element_get<HWND>( DeviceObject, 0x578 );
	//GetObjectHookManager()->analysis(DeviceObject, 45);
#endif

	memset( &OldRect, 0, sizeof( RECT ) );
	memset( &LockRect, 0, sizeof( RECT ) );
	memset( &ClientAreaScreenRect, 0, sizeof( RECT ) );
	memset( &Point, 0, sizeof( POINT ) );
	GetClipCursor( &OldRect );
	Hwnd = DeviceObject->vtable->GetDeviceWindow( );
	OldWndProc = reinterpret_cast< war3::CGxDeviceD3d* >( DeviceObject )->wndProc;
	reinterpret_cast< war3::CGxDeviceD3d* >( DeviceObject )->wndProc = &( DetourWindowProc );

}

__declspec( noinline ) void War3Window_Cleanup( ) {
	if ( !IsBadWritePtr( DeviceObject, 4 ) )
		reinterpret_cast< war3::CGxDeviceD3d* >( DeviceObject )->wndProc = OldWndProc;

	GameEventObserver_Cleanup( );
}

//当确保Hook成功时调用
void HookReady( ) {
	GameEventObserver_Init( );
}

void UpdateFullscreen( WORD width, WORD height ) {
	HDC windowHDC = GetDC( Hwnd );
	WORD screenWidth = width ? width : GetDeviceCaps( windowHDC, HORZRES );
	WORD screenHeight = height ? height : GetDeviceCaps( windowHDC, VERTRES );
	WORD windowWidth = screenWidth;
	WORD windowHeight = screenHeight;
	//0.8 x 0.6

	if ( DeviceType == DEVICE_TYPE::D3D )
		D3DSetStageSize( static_cast< float >( screenWidth ), static_cast< float >( screenHeight ) );

	SetWindowLongPtr( Hwnd, GWL_EXSTYLE, WS_EX_APPWINDOW );
	SetWindowLongPtr( Hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE );
	SetWindowPos( Hwnd, HWND_TOPMOST, ( screenWidth - windowWidth ) / 2, ( screenHeight - windowHeight ) / 2, windowWidth, windowHeight, SWP_SHOWWINDOW );
}

bool UpdateClipCursor( ) {
	Point.x = Point.y = 0;
	if ( GetClientRect( Hwnd, &LockRect ) && ClientToScreen( Hwnd, &Point ) ) {
		LockRect.left += Point.x;
		LockRect.right += Point.x;
		LockRect.top += Point.y;
		LockRect.bottom += Point.y;
		ClipCursor( &LockRect );
		return true;
	}
	else
		return false;
}

void LockCursor( ) {
	if ( Locked )
		return;


	if ( UpdateClipCursor( ) ) {
		Locked = true;
	}
}

void UnlockCursor( ) {
	if ( !Locked )
		return;

	ClipCursor( &OldRect );
	Locked = false;
}

bool ToggleCursorLock( ) {
	if ( !Locked )
		LockCursor( );
	else
		UnlockCursor( );
	return Locked;
}

void EnableFullscreen( ) {
	if ( !Fullscreen ) {
		Fullscreen = true;
		GetWindowRect( Hwnd, &OldWindowRect );
		GetClientRect( Hwnd, &OldClientRect );
		UpdateFullscreen( );
	}
}

void DisableFullscreen( ) {
	if ( Fullscreen ) {
		HDC windowHDC = GetDC( Hwnd );
		WORD screenWidth = GetDeviceCaps( windowHDC, HORZRES );
		WORD screenHeight = GetDeviceCaps( windowHDC, VERTRES );
		WORD windowWidth = static_cast< WORD >( OldWindowRect.right - OldWindowRect.left );
		WORD windowHeight = static_cast< WORD >( OldWindowRect.bottom - OldWindowRect.top );
		windowWidth = static_cast< WORD >( OldWindowRect.right - OldWindowRect.left );
		windowHeight = static_cast< WORD >( OldWindowRect.bottom - OldWindowRect.top );
		if ( DeviceType == DEVICE_TYPE::D3D )
			D3DSetStageSize( static_cast< float >( windowWidth ), static_cast< float >( windowHeight ) );
		SetWindowLongPtr( Hwnd, GWL_EXSTYLE, DefaultWindowExStyle );
		SetWindowLongPtr( Hwnd, GWL_STYLE, DefaultWindowStyle );
		SetWindowPos( Hwnd, HWND_NOTOPMOST, OldWindowRect.left, OldWindowRect.top, windowWidth, windowHeight, 0 );
		Fullscreen = false;
	}
}

bool ToggleFullscreen( ) {
	if ( Fullscreen ) {
		DisableFullscreen( );
	}
	else {
		EnableFullscreen( );
	}
	return Fullscreen;
}

bool IsFullscreen( ) {
	return Fullscreen;
}

float War3WindowRatioX( )
{
	//TODO
	return WidthRatioX ? WidthRatioX : 1.f;
}

float War3WindowRatioY( )
{
	//TODO
	return WidthRatioY ? WidthRatioY : 1.f;
}

