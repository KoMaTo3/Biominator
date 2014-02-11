#include "core.h"
#include "renderer.h"
#include "../tools.h"
#include "../eventtypes.h"
#include "X11/X.h"
#include "filemanager.h"
//#include "X11/Xlib.h"


#include <string>

using namespace Engine;

CoreLinux::CoreLinux()
:Core() {
  static const GLint tmpAttr[] = {
      GLX_RGBA,
      GLX_RED_SIZE, 4,
      GLX_GREEN_SIZE, 4,
      GLX_BLUE_SIZE, 4,
      GLX_DEPTH_SIZE, 16,
      GLX_DOUBLEBUFFER,
      None };
  this->windowAttr = tmpAttr;
  this->screenWidth = 320;
  this->screenHeight = 200;
  this->fileManager = new FileManagerType( "data/" );

  this->display = XOpenDisplay( NULL );
  if( !this->display ) {
    printf( "Error: XOpenDisplay failed\n" );
    return;
  }
  this->windowRoot = DefaultRootWindow( this->display );
  this->visualInfo = glXChooseVisual( this->display, 0, ( GLint* ) this->windowAttr );
  if( !this->visualInfo ) {
    printf( "Error: glXChooseVisual failed\n" );
    return;
  }
  this->colormap = XCreateColormap( this->display, this->windowRoot, this->visualInfo->visual, AllocNone );
  this->setWndAttr.colormap = this->colormap;
  this->setWndAttr.event_mask =
      ExposureMask | KeyPressMask | KeyReleaseMask | VisibilityChangeMask | StructureNotifyMask
      | SubstructureNotifyMask | FocusChangeMask | ButtonPressMask | ButtonReleaseMask
      | PointerMotionMask /* | KeymapStateMask */ ;
  this->window = XCreateWindow(
      this->display,
      this->windowRoot,
      0,
      0,
      this->screenWidth,
      this->screenHeight,
      0,
      this->visualInfo->depth,
      InputOutput,
      this->visualInfo->visual,
      CWColormap | CWEventMask,
      &this->setWndAttr
      );
  XMapWindow( this->display, this->window );
  XGetWindowAttributes( this->display, this->windowRoot, &this->getWndAttr );
  XMoveWindow( this->display, this->window, ( this->getWndAttr.width - this->screenWidth ) >> 1, ( this->getWndAttr.height - this->screenHeight ) >> 1 );
  XStoreName( this->display, this->window, "Biominator" );
  Atom wmDelete = XInternAtom( this->display, "WM_DELETE_WINDOW", False );
  XSetWMProtocols( display, this->window, &wmDelete, 1 );
  this->isVisible = true;

  this->renderer = new Engine::RendererLinuxGL(
      this->display, this->visualInfo, &this->window,
      this->screenWidth, this->screenHeight
      );
}


CoreLinux::~CoreLinux() {
  SAFE_DELETE( this->fileManager );
}

void CoreLinux::Update() {
  XEvent event;
  while( XPending( this->display ) > 0 ) {
    XNextEvent( this->display, &event );
    this->HandleCmd( &event );
  }

  if( !this->isValid ) {
    SAFE_DELETE( this->renderer );
  }
  /*
  while( XPending( this->display ) > 0 ) {
    XNextEvent( this->display, &event );
    switch( event.type ) {
      case Expose: {
        XGetWindowAttributes( this->display, this->window, &this->getWndAttr );
        //glViewport( 0, 0, this->getWndAttr.width, this->getWndAttr.height );
        //this->Redraw( NULL );
        break;
      }//Expose
      case KeyPress: {
        glXMakeCurrent( this->display, None, NULL );
        //glXDestroyContext( this->display, this->context );
        XDestroyWindow( this->display, this->window );
        XCloseDisplay( this->display );
        this->isValid = false;
        return;
      }
    }//switch event type
  }
  */
}//Update

/*
HWND CoreWin32::GetWindow() {
  return this->windowHandle;
}//GetWindow
*/

void CoreLinux::Destroy() {
  if( this->display ) {
    SAFE_DELETE( this->renderer );
    XDestroyWindow( this->display, this->window );
    XCloseDisplay( this->display );
    this->display = NULL;
    this->window = 0;
  }
  this->isValid = false;
  this->isValid = false;
  this->isFocused = false;
  this->isVisible = false;
  this->animating = false;
}//Destroy

void CoreLinux::Stop() {
  this->isFocused = false;
  this->isVisible = false;
  this->animating = false;
  //::ShowWindow( this->windowHandle, SW_MINIMIZE );
}//Stop

void CoreLinux::HandleCmd( XEvent *event ) {
  //LOGI( "cmd: %d", event->type );
  switch( event->type ) {
  case KeyPress: {
    this->OnKeyEvent( event, true );
  }
  break;
  case KeyRelease: {
    bool isRepeat = false;
    if( XEventsQueued( this->display, QueuedAfterReading ) ) {
      XEvent nev;
      XPeekEvent( this->display, &nev );
      if( nev.type == KeyPress && nev.xkey.time == event->xkey.time &&
          nev.xkey.keycode == event->xkey.keycode ) {
        XNextEvent( this->display, event );
        isRepeat = true;
      }
    }
    if( !isRepeat ) {
      this->OnKeyEvent( event, false );
      //XLookupKeysym (&report.xkey, 0));
    }
  }
  break;
  case Expose: {
    XGetWindowAttributes( this->display, this->window, &this->getWndAttr );
    glViewport( 0, 0, this->getWndAttr.width, this->getWndAttr.height );
  }
  break;
  case ClientMessage: {
    this->TouchEvent( Engine::EVENT_TYPE_CORE_CLOSE, NULL );
    //this->isValid = false;
  }
  break;
  case FocusIn: {
    this->isFocused = true;
    this->animating = this->isFocused && this->isVisible;
  }
  break;
  case FocusOut: {
    this->isFocused = false;
    this->animating = false;
  }
  break;
  case ButtonPress: {
    this->OnMouseEvent( event, true );
  }
  break;
  case ButtonRelease: {
    this->OnMouseEvent( event, false );
  }
  break;
  case MotionNotify: {
  }
  break;
  default: {
    LOGI( "unknown event x%X", event->type );
  }
    /*
    case WM_ACTIVATE: {
      //core->Signal( 1, !HIWORD( wParam ) );
      if( !core->renderer ) {
        core->renderer = new Engine::RendererWin32gl( core->GetWindow(), core->screenWidth, core->screenHeight );
      }
      core->isVisible = true;
      core->animating = core->isFocused && core->isVisible;
    }
    return 0;

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

    case WM_MOUSEMOVE: {
      EventMouseMove event;
      event.x = LOWORD( lParam );
      event.y = HIWORD( lParam );
      core->TouchEvent( &event );
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
          EventKey key( Engine::EVENT_TYPE_SYSTEM_KEY );
          key.keyCode = wParam;
          core->TouchEvent( &key );
          //core->keyboard.DoPress( wParam );
        }
      } else {
        //core->keyboard.DoRelease( wParam );
      }
      return 0;
    }

    case WM_DESTROY: {
      PostQuitMessage( 0 );
      return 0;
    }
    */
  }
}//HandleCmd

void CoreLinux::OnKeyEvent( XEvent *event, bool isPressed ) {
  char buffer[ 128 ] = { 0 };
  KeySym key;
  //int length =
  XLookupString( &event->xkey, buffer, sizeof( buffer ), &key, NULL );
  LOGI( "Pressed: x%X", ( int ) key );
  int code = 0;
  switch( key ) {
    case XK_Escape: code = KEY_CODE_ESCAPE; break;
    case XK_BackSpace: code = KEY_CODE_BACKSPACE; break;
    case XK_Tab: code = KEY_CODE_TAB; break;
    case XK_Return: code = KEY_CODE_ENTER; break;
    case XK_Shift_L: code = KEY_CODE_SHIFT; break;
    case XK_Shift_R: code = KEY_CODE_SHIFT; break;
    case XK_Control_L: code = KEY_CODE_CONTROL; break;
    case XK_Control_R: code = KEY_CODE_CONTROL; break;
    case XK_Alt_L: code = KEY_CODE_ALT; break;
    case XK_Alt_R: code = KEY_CODE_ALT; break;
    case XK_space: code = KEY_CODE_SPACE; break;
    case XK_Delete: code = KEY_CODE_DELETE; break;
    case XK_0: code = KEY_CODE_CHAR_0; break;
    case XK_1: code = KEY_CODE_CHAR_1; break;
    case XK_2: code = KEY_CODE_CHAR_2; break;
    case XK_3: code = KEY_CODE_CHAR_3; break;
    case XK_4: code = KEY_CODE_CHAR_4; break;
    case XK_5: code = KEY_CODE_CHAR_5; break;
    case XK_6: code = KEY_CODE_CHAR_6; break;
    case XK_7: code = KEY_CODE_CHAR_7; break;
    case XK_8: code = KEY_CODE_CHAR_8; break;
    case XK_9: code = KEY_CODE_CHAR_9; break;
    case XK_A: code = KEY_CODE_CHAR_A; break;
    case XK_B: code = KEY_CODE_CHAR_B; break;
    case XK_C: code = KEY_CODE_CHAR_C; break;
    case XK_D: code = KEY_CODE_CHAR_D; break;
    case XK_E: code = KEY_CODE_CHAR_E; break;
    case XK_F: code = KEY_CODE_CHAR_F; break;
    case XK_G: code = KEY_CODE_CHAR_G; break;
    case XK_H: code = KEY_CODE_CHAR_H; break;
    case XK_I: code = KEY_CODE_CHAR_I; break;
    case XK_J: code = KEY_CODE_CHAR_J; break;
    case XK_K: code = KEY_CODE_CHAR_K; break;
    case XK_L: code = KEY_CODE_CHAR_L; break;
    case XK_M: code = KEY_CODE_CHAR_M; break;
    case XK_N: code = KEY_CODE_CHAR_N; break;
    case XK_O: code = KEY_CODE_CHAR_O; break;
    case XK_P: code = KEY_CODE_CHAR_P; break;
    case XK_Q: code = KEY_CODE_CHAR_Q; break;
    case XK_R: code = KEY_CODE_CHAR_R; break;
    case XK_S: code = KEY_CODE_CHAR_S; break;
    case XK_T: code = KEY_CODE_CHAR_T; break;
    case XK_U: code = KEY_CODE_CHAR_U; break;
    case XK_V: code = KEY_CODE_CHAR_V; break;
    case XK_W: code = KEY_CODE_CHAR_W; break;
    case XK_X: code = KEY_CODE_CHAR_X; break;
    case XK_Y: code = KEY_CODE_CHAR_Y; break;
    case XK_Z: code = KEY_CODE_CHAR_Z; break;
    case XK_F1: code = KEY_CODE_F1; break;
    case XK_F2: code = KEY_CODE_F2; break;
    case XK_F3: code = KEY_CODE_F3; break;
    case XK_F4: code = KEY_CODE_F4; break;
    case XK_F5: code = KEY_CODE_F5; break;
    case XK_F6: code = KEY_CODE_F6; break;
    case XK_F7: code = KEY_CODE_F7; break;
    case XK_F8: code = KEY_CODE_F8; break;
    case XK_F9: code = KEY_CODE_F9; break;
    case XK_F10: code = KEY_CODE_F10; break;
    case XK_F11: code = KEY_CODE_F11; break;
    case XK_F12: code = KEY_CODE_F12; break;
    case XK_minus: code = KEY_CODE_MINUS; break;
    case XK_equal: code = KEY_CODE_EQUAL; break;
    case XK_grave: code = KEY_CODE_TILDE; break;
  }
  if( code ) {
    EventType type = isPressed ? EVENT_TYPE_KEY_PRESSED : EVENT_TYPE_KEY_RELEASED;
    EventKey engineEvent( type );
    engineEvent.keyCode = code;
    this->TouchEvent( type, &engineEvent );
  }
}//OnKeyPress

void CoreLinux::OnMouseEvent( XEvent *event, bool isPressed ) {
  EventKey key( isPressed ? Engine::EVENT_TYPE_KEY_PRESSED : Engine::EVENT_TYPE_KEY_RELEASED );
  switch( event->xbutton.button ) {
  case 1: key.keyCode = KEY_CODE_MOUSE_LEFT; break;
  case 2: key.keyCode = KEY_CODE_MOUSE_MIDDLE; break;
  case 3: key.keyCode = KEY_CODE_MOUSE_RIGHT; break;
  default: key.keyCode = KEY_CODE_MOUSE_OTHER_0 + event->xbutton.button - 3; break;
  }
  this->TouchEvent( &key );
}//OnMouseEvent
