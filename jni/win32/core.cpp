#include "core.h"
#include "renderer.h"
#include "../tools.h"
#include "../eventtypes.h"
#include "filemanager.h"

#include <string>

using namespace Engine;

CoreWin32::CoreWin32( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
:Core(), windowHandle( NULL ) {
  //this->state->onAppCmd = CoreWin32::HandleCmd;
  //this->state->onInputEvent = CoreWin32::HandleInput;
  //this->state->userData = this;

  this->fileManager = new FileManagerType( "data/" );
  this->mapVirtualKeys.leftShift = MapVirtualKey( VK_LSHIFT, 0 );
  this->mapVirtualKeys.leftControl = MapVirtualKey( VK_LCONTROL, 0 );
  this->mapVirtualKeys.leftAlt = MapVirtualKey( VK_LMENU, 0 );

  WNDCLASSEX wndClass;
  memset( &wndClass, 0, sizeof( wndClass ) );
  std::string className = "Biominator";
  std::string windowName = "Biominator";

  wndClass.cbSize       = sizeof( wndClass );
  wndClass.style        = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
  wndClass.lpfnWndProc  = CoreWin32::HandleCmd;
  wndClass.cbClsExtra   = 0;
  wndClass.cbWndExtra   = 0 | DLGWINDOWEXTRA;
  wndClass.hInstance    = GetModuleHandle( NULL );
  wndClass.hIcon        = LoadIcon( NULL, IDI_APPLICATION );
  wndClass.hCursor      = 0;
  wndClass.hbrBackground = ( HBRUSH ) GetStockObject( WHITE_BRUSH );
  wndClass.lpszMenuName = 0;
  wndClass.lpszClassName = className.c_str();
  wndClass.hIconSm = 0;
  if( !( RegisterClassEx( &wndClass ) ) ) {
    this->isValid = false;
  } else {
    this->screenWidth = 320;
    this->screenHeight = 200;
    DWORD style   = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;  //WS_POPUP for fullscreen
    DWORD styleEx = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE; //WS_EX_APPWINDOW for fullscreen
    RECT windowRect;
    windowRect.left   = 0;
    windowRect.top    = 0;
    windowRect.right  = this->screenWidth;
    windowRect.bottom = this->screenHeight;
    AdjustWindowRectEx( &windowRect, style, false, styleEx );

    this->windowHandle = CreateWindowEx(
      styleEx,
      className.c_str(),
      windowName.c_str(),
      style | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
      0,
      0,
      windowRect.right - windowRect.left,
      windowRect.bottom - windowRect.top,
      0,
      NULL,
      GetModuleHandle( NULL ),
      NULL
    );
    SetWindowLong( this->windowHandle, GWL_USERDATA, LONG( this ) );

    ShowWindow  ( this->windowHandle, SW_NORMAL );
    UpdateWindow( this->windowHandle );
  }
}


CoreWin32::~CoreWin32() {
  SAFE_DELETE( this->fileManager );
}

void CoreWin32::Update() {

  MSG msg;
  while( this->isValid && PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ) {
    if( GetMessage( &msg, NULL, 0, 0 ) == TRUE ) {
      TranslateMessage( &msg );
      DispatchMessage( &msg );
    }
  }
}//Update

HWND CoreWin32::GetWindow() {
  return this->windowHandle;
}//GetWindow

void CoreWin32::Destroy() {
  this->isValid = false;
  this->isFocused = false;
  this->isVisible = false;
  this->animating = false;
}//Destroy

void CoreWin32::Stop() {
  this->isFocused = false;
  this->isVisible = false;
  this->animating = false;
  ::ShowWindow( this->windowHandle, SW_MINIMIZE );
}//Stop

bool CoreWin32::IsKeyLeft( WPARAM wParam, LPARAM lParam ) {
  switch( wParam ) {
    case VK_SHIFT: {
      return ( ( ( lParam & 0xFF0000 ) >> 16 ) == this->mapVirtualKeys.leftShift );
    }
    break;
    case VK_CONTROL: {
      if( lParam & 0x01000000 ) {
        return false;
      } else {
        LONG msg_time = GetMessageTime();
        MSG nextMsg;
        if( PeekMessage( &nextMsg, NULL, 0, 0, PM_NOREMOVE ) ) {
          if( nextMsg.message == WM_KEYDOWN || nextMsg.message == WM_SYSKEYDOWN ) {
            if( nextMsg.wParam == VK_MENU && ( nextMsg.lParam & 0x01000000 ) && nextMsg.time == msg_time ) {
              return false;
            }
          }
        }
        return true;
      }
    }
    break;
    case VK_MENU: {
      return !( lParam & 0x01000000 );
    }
    break;
  }
  return true;
}//IsKeyLeft

LRESULT APIENTRY CoreWin32::HandleCmd( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) {
  CoreWin32 *core = ( CoreWin32* ) GetWindowLong( hWnd, GWL_USERDATA );
  switch( message ) {
    case WM_ACTIVATE: {
      if( !core->renderer ) {
        core->renderer = new Engine::RendererWin32gl( core->GetWindow(), core->screenWidth, core->screenHeight );
      }
      core->isVisible = true;
      core->animating = core->isFocused && core->isVisible;
    }
    return 0;

    case WM_KILLFOCUS: {
      core->isFocused = false;
      core->animating = false;
    }
    break;

    case WM_SETFOCUS: {
      core->isFocused = true;
      core->animating = core->isFocused && core->isVisible;
    }
    break;

    case WM_CLOSE: {
      if( core ) {
        core->TouchEvent( Engine::EVENT_TYPE_CORE_CLOSE, NULL );
        return 0;
      }
    }
    break;

    case WM_SIZE: {
      switch( wParam ) {
        case SIZE_MINIMIZED: {
          core->isVisible = false;
        }
        default: {
          core->isVisible = true;
        }
        break;
      }//wparam
    }
    break;

    case WM_KEYDOWN: {
      if( !( lParam & 0x40000000 ) ) {  //prevent auto-repeating
        EventKey key( Engine::EVENT_TYPE_KEY_PRESSED );
        key.keyCode = wParam;
        key.isLeft = core->IsKeyLeft( wParam, lParam );
        core->TouchEvent( &key );
      }
      return 0;
    }

    case WM_KEYUP: {
      EventKey key( Engine::EVENT_TYPE_KEY_RELEASED );
      key.keyCode = wParam;
        key.isLeft = core->IsKeyLeft( wParam, lParam );
      core->TouchEvent( &key );
      return 0;
    }

    case WM_MOUSEMOVE: {
      EventMouseMove event;
      event.x = LOWORD( lParam );
      event.y = HIWORD( lParam );
      core->TouchEvent( &event );
      return 0;
    }

    case WM_LBUTTONDOWN: {
      EventKey key( Engine::EVENT_TYPE_KEY_PRESSED );
      key.keyCode = VK_LBUTTON;
      core->TouchEvent( &key );
      return 0;
    }

    case WM_LBUTTONUP: {
      EventKey key( Engine::EVENT_TYPE_KEY_RELEASED );
      key.keyCode = VK_LBUTTON;
      core->TouchEvent( &key );
      //}
      return 0;
    }

    case WM_RBUTTONDOWN: {
      EventKey key( Engine::EVENT_TYPE_KEY_PRESSED );
      key.keyCode = VK_RBUTTON;
      core->TouchEvent( &key );
      return 0;
    }

    case WM_RBUTTONUP: {
      EventKey key( Engine::EVENT_TYPE_KEY_RELEASED );
      key.keyCode = VK_RBUTTON;
      core->TouchEvent( &key );
      return 0;
    }

    case WM_MBUTTONDOWN: {
      EventKey key( Engine::EVENT_TYPE_KEY_PRESSED );
      key.keyCode = VK_MBUTTON;
      core->TouchEvent( &key );
      return 0;
    }

    case WM_MBUTTONUP: {
      EventKey key( Engine::EVENT_TYPE_KEY_RELEASED );
      key.keyCode = VK_MBUTTON;
      core->TouchEvent( &key );
      return 0;
    }

    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_SYSCHAR: {
      bool altPressed = ( ( lParam&( 1<<29 ) ) == ( 1<<29) );
      if( wParam == VK_F4 && altPressed ) { //Alt+F4
        core->TouchEvent( EVENT_TYPE_CORE_CLOSE, NULL );
      }
      if( message == WM_SYSKEYDOWN ) {
        if( !( lParam & 0x40000000 ) ) {
          EventKey key( Engine::EVENT_TYPE_KEY_PRESSED );
          key.keyCode = KEY_CODE_ALT;
          key.isLeft = core->IsKeyLeft( wParam, lParam );
          core->TouchEvent( &key );
        }
      } else {
        EventKey key( Engine::EVENT_TYPE_KEY_RELEASED );
        key.keyCode = KEY_CODE_ALT;
        key.isLeft = core->IsKeyLeft( wParam, lParam );
        core->TouchEvent( &key );
      }
      return 0;
    }

    case WM_DESTROY: {
      PostQuitMessage( 0 );
      return 0;
    }

    case WM_SYSCOMMAND: {
      switch( wParam ) {
        case SC_SCREENSAVE:   // Пытается ли запустится скринсейвер?
        case SC_MONITORPOWER: // Пытается ли монитор перейти в режим сбережения энергии?
        return 0;
      }
      break;
    }

    /*
    case WM_SIZE:
      if( core )
        core->Signal( CORE_SIGNAL_RESIZE, lParam );
    return 0;

    case WM_SETCURSOR:
      return 0;
    break;

    default:
    break;
    */
  }
  return DefWindowProc( hWnd, message, wParam, lParam );
}//HandleCmd
