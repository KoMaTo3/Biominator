#pragma once


#include "math/kvector.h"
#include "types.h"
#include <map>

namespace Engine {

class SquareAtlas {
public:
  struct _MapKey {
    uint32_t
        square, //площадь
        left,   //левый
        top;    //угол
    _MapKey(){}
    _MapKey( uint32_t newSquare, uint32_t newLeft, uint32_t newTop )
    :square( newSquare ), left( newLeft ), top( newTop )
    { }
    _MapKey( const Engine::Rect<uint32_t> &rect )
    :square( ( rect.right - rect.left ) * ( rect.bottom - rect.top ) ), left( rect.left ), top( rect.top )
    { }
  };
  struct _MapKey_Less {
    bool operator()( const _MapKey &l, const _MapKey &r ) const
    {
      return  ( l.square < r.square ) ||
              ( l.square == r.square && l.top < r.top ) ||
              ( l.square == r.square && l.top == r.top && l.left < r.left );
    }
  };

  typedef std::map< _MapKey, Engine::Rect< uint32_t >, _MapKey_Less > SquareAtlasMap;

private:
  SquareAtlasMap items;  //свободные блоки

public:
  SquareAtlas();
  virtual ~SquareAtlas();

  void        Init        ( const Engine::Size& size );
  bool        Cut         ( const Engine::Size& size, Engine::Rect< uint32_t > *result );
  bool        HasPlace    ( const Engine::Size& size );

  bool        _CanBePlaced( const Engine::Size& _who, const Engine::Rect< uint32_t > *_where );
  bool        _IsIntersect( const Engine::Rect< uint32_t >& square1, const Engine::Rect< uint32_t >& square2 );
  bool        _IsHave     ( const Engine::Rect< uint32_t >& minSquare, const Engine::Rect< uint32_t >& maxSquare );

  void        __Dump      ();
};

};
