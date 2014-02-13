#pragma once

#include "event.h"

namespace Engine {

enum EventType {
  EVENT_TYPE_UNKNOWN = 0,

  /* ������ */
  EVENT_TYPE_RENDERER_INIT,
  EVENT_TYPE_RENDERER_BEFORE_DESTROY,
  EVENT_TYPE_RENDERER_AFTER_DESTROY,
  EVENT_TYPE_RENDERER_BEFORE_RENDER,
  EVENT_TYPE_RENDERER_AFTER_RENDER,

  /* ���� */
  EVENT_TYPE_CORE_CLOSE,
  EVENT_TYPE_CORE_CREATED_RENDERER,

  /* ���-��� */
  EVENT_TYPE_MOTION,

  /* ���������� � ������ ���� */
  EVENT_TYPE_KEY_PRESSED,
  EVENT_TYPE_KEY_RELEASED,
  EVENT_TYPE_KEY_CLICKED,
  EVENT_TYPE_KEY_DBLCLICKED,

  /* ������ ���� */
  EVENT_TYPE_MOUSE_MOVE,

  /* ��������� ������, ����� ��� "�����" � ��������� �������� */
  EVENT_TYPE_SYSTEM_KEY,

  EVENT_TYPE_MAX
};

/* ���� ������ ���������� � ���� */
enum KeyCode {
  KEY_CODE_UNKNOWN      = 0,
  KEY_CODE_MOUSE_LEFT   = 0x01,
  KEY_CODE_MOUSE_RIGHT  = 0x02,
  KEY_CODE_MOUSE_MIDDLE = 0x03,
  KEY_CODE_BACKSPACE    = 0x08,
  KEY_CODE_TAB          = 0x09,
  KEY_CODE_ENTER        = 0x0D,
  KEY_CODE_SHIFT        = 0x10,
  KEY_CODE_CONTROL      = 0x11,
  KEY_CODE_ALT          = 0x12,
  KEY_CODE_ESCAPE       = 0x1B,
  KEY_CODE_SPACE        = 0x20,
  KEY_CODE_DELETE       = 0x2E,
  KEY_CODE_CHAR_0       = 0x30,
  KEY_CODE_CHAR_1       = 0x31,
  KEY_CODE_CHAR_2       = 0x32,
  KEY_CODE_CHAR_3       = 0x33,
  KEY_CODE_CHAR_4       = 0x34,
  KEY_CODE_CHAR_5       = 0x35,
  KEY_CODE_CHAR_6       = 0x36,
  KEY_CODE_CHAR_7       = 0x37,
  KEY_CODE_CHAR_8       = 0x38,
  KEY_CODE_CHAR_9       = 0x39,
  KEY_CODE_CHAR_A       = 0x41,
  KEY_CODE_CHAR_B       = 0x42,
  KEY_CODE_CHAR_C       = 0x43,
  KEY_CODE_CHAR_D       = 0x44,
  KEY_CODE_CHAR_E       = 0x45,
  KEY_CODE_CHAR_F       = 0x46,
  KEY_CODE_CHAR_G       = 0x47,
  KEY_CODE_CHAR_H       = 0x48,
  KEY_CODE_CHAR_I       = 0x49,
  KEY_CODE_CHAR_J       = 0x4A,
  KEY_CODE_CHAR_K       = 0x4B,
  KEY_CODE_CHAR_L       = 0x4C,
  KEY_CODE_CHAR_M       = 0x4D,
  KEY_CODE_CHAR_N       = 0x4E,
  KEY_CODE_CHAR_O       = 0x4F,
  KEY_CODE_CHAR_P       = 0x50,
  KEY_CODE_CHAR_Q       = 0x51,
  KEY_CODE_CHAR_R       = 0x52,
  KEY_CODE_CHAR_S       = 0x53,
  KEY_CODE_CHAR_T       = 0x54,
  KEY_CODE_CHAR_U       = 0x55,
  KEY_CODE_CHAR_V       = 0x56,
  KEY_CODE_CHAR_W       = 0x57,
  KEY_CODE_CHAR_X       = 0x58,
  KEY_CODE_CHAR_Y       = 0x59,
  KEY_CODE_CHAR_Z       = 0x5A,
  KEY_CODE_F1           = 0x70,
  KEY_CODE_F2           = 0x71,
  KEY_CODE_F3           = 0x72,
  KEY_CODE_F4           = 0x73,
  KEY_CODE_F5           = 0x74,
  KEY_CODE_F6           = 0x75,
  KEY_CODE_F7           = 0x76,
  KEY_CODE_F8           = 0x77,
  KEY_CODE_F9           = 0x78,
  KEY_CODE_F10          = 0x79,
  KEY_CODE_F11          = 0x7A,
  KEY_CODE_F12          = 0x7B,
  KEY_CODE_MINUS        = 0xBD,
  KEY_CODE_EQUAL        = 0xBB,
  KEY_CODE_TILDE        = 0xC0,
  KEY_CODE_MOUSE_OTHER_0= 0x100,
  KEY_CODE_MOUSE_OTHER_1= 0x101,
  KEY_CODE_MOUSE_OTHER_2= 0x102,
  KEY_CODE_MOUSE_OTHER_3= 0x103,
  KEY_CODE_MOUSE_OTHER_4= 0x104,
  KEY_CODE_MOUSE_OTHER_5= 0x105,
  KEY_CODE_MOUSE_OTHER_6= 0x106,
  KEY_CODE_MOUSE_OTHER_7= 0x107,
  KEY_CODE_MOUSE_OTHER_8= 0x108,
  KEY_CODE_MOUSE_OTHER_9= 0x109,
};

enum PlatformType {
  PLATFORM_TYPE_UNKNOWN = 0,
  PLATFORM_TYPE_ANDROID = 1,
  PLATFORM_TYPE_WIN32   = 2,
  PLATFORM_TYPE_LINUX   = 3,
};

class EventKey: public Event {
public:
  int keyCode;
  bool isLeft;

  EventKey();
  EventKey( const int setEventId );
  virtual ~EventKey() {}
};

class EventMouseMove: public Event {
public:
  int
    x,
    y;

  EventMouseMove();
  virtual ~EventMouseMove() {}
};

};
