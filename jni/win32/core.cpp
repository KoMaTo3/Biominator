#include "core.h"
#include "renderer.h"
#include "../tools.h"
#include "../eventtypes.h"

#include <string>

using namespace Engine;

CoreWin32::CoreWin32( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
:Core(), windowHandle( NULL ) {
  //this->state->onAppCmd = CoreWin32::HandleCmd;
  //this->state->onInputEvent = CoreWin32::HandleInput;
  //this->state->userData = this;

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
}//Destroy


LRESULT APIENTRY CoreWin32::HandleCmd( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) {
  CoreWin32 *core = ( CoreWin32* ) GetWindowLong( hWnd, GWL_USERDATA );
  LOGI( "HandleCmd" );
  switch( message ) {
    case WM_ACTIVATE: {
      //core->Signal( 1, !HIWORD( wParam ) );
      LOGI( "HandleCmd => WM_ACTIVATE, window[%p]", core->GetWindow() );
      core->renderer = new Engine::RendererWin32gl( core->GetWindow(), core->screenWidth, core->screenHeight );
      core->isFocused = true;
      core->isVisible = true;
    }
    return 0;

    case WM_CLOSE: {
      if( core ) {
        LOGI( "HandleCmd => WM_CLOSE" );
        core->TouchEvent( Engine::EVENT_TYPE_CORE_CLOSE );
        core->Destroy();
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
      if( wParam == VK_ESCAPE ) {
        core->isValid = false;
      }
    }
    break;

    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    case WM_SYSCHAR: {
      bool altPressed = ( ( lParam&( 1<<29 ) ) == ( 1<<29) );
      if( wParam == VK_F4 && altPressed ) { //Alt+F4
        core->isValid = false;
      }
      if( message == WM_SYSKEYDOWN ) {
        if( !( lParam & 0x40000000 ) ) {
          //core->keyboard.DoPress( wParam );
        }
      } else {
        //core->keyboard.DoRelease( wParam );
      }
      return 0;
    }

    case WM_DESTROY: {
      PostQuitMessage( 0 );
    }
    return 0;

    /*
    case WM_SIZE:
      if( core )
        core->Signal( CORE_SIGNAL_RESIZE, lParam );
    return 0;

    case WM_SYSCOMMAND: // Перехватываем системную команду
    {
      switch( wParam )   // Останавливаем системный вызов
      {
        case SC_SCREENSAVE:   // Пытается ли запустится скринсейвер?
        case SC_MONITORPOWER: // Пытается ли монитор перейти в режим сбережения энергии?
        return 0;
      }
      break;
    }

    case WM_KEYDOWN:
    {
      if( !( lParam & 0x40000000 ) ) {  //prevent auto-repeating
        //__log.PrintInfo( Filelevel_DEBUG, "WM_KEYDOWN: %d[x%X] => %d mods[%d]", wParam, wParam, Keyboard::KeyCodeToGlut( wParam ), KeyboardGetModifiers() );
        core->keyboard.DoPress( wParam );
      }
      //}
      return 0;
    }

    case WM_KEYUP:
    {
      core->keyboard.DoRelease( wParam );
      return 0;
    }

    case WM_MOUSEMOVE:
    {
      core->mouse.MoveCursor( Vec2( float( LOWORD( lParam ) ), float( HIWORD( lParam ) ) ) );
      return 0;
    }

    case WM_LBUTTONDOWN:
        core->mouse.DoPress( VK_LBUTTON );
      //}
      return 0;
    break;

    case WM_LBUTTONUP:
        core->mouse.DoRelease( VK_LBUTTON );
      //}
      return 0;
    break;

    case WM_RBUTTONDOWN:
        core->mouse.DoPress( VK_RBUTTON );
      //}
      return 0;
    break;

    case WM_RBUTTONUP:
        core->mouse.DoRelease( VK_RBUTTON );
      //}
      return 0;
    break;

    case WM_MBUTTONDOWN:
        core->mouse.DoPress( VK_MBUTTON );
      //}
      return 0;
    break;

    case WM_MBUTTONUP:
        core->mouse.DoRelease( VK_MBUTTON );
      //}
      return 0;
    break;
    */

    case WM_SETCURSOR:
      return 0;
    break;

    default:
    break;
  }
  return DefWindowProc( hWnd, message, wParam, lParam );
}//HandleCmd
