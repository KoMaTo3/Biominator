#include "core.h"
#include "renderer.h"
#include "../tools.h"
#include "../eventtypes.h"
#include "X11/X.h"
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
      | SubstructureNotifyMask | FocusChangeMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | KeymapStateMask;
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
    this->isValid = false;
    /*
    glXMakeCurrent( this->display, None, NULL );
    //glXDestroyContext( this->display, this->context );
    */
  }
  break;
  case Expose: {
    XGetWindowAttributes( this->display, this->window, &this->getWndAttr );
    glViewport( 0, 0, this->getWndAttr.width, this->getWndAttr.height );
  }
  break;
  case ClientMessage: {
    this->isValid = false;
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
        core->TouchEvent( &key );
      }
      return 0;
    }

    case WM_KEYUP: {
      EventKey key( Engine::EVENT_TYPE_KEY_RELEASED );
      key.keyCode = wParam;
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
