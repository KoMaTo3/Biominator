#include "squareatlas.h"
#include "tools.h"
#include <vector>

using namespace Engine;


SquareAtlas::SquareAtlas() {
}

SquareAtlas::~SquareAtlas() {
}


void SquareAtlas::Init( const Engine::Size& size ) {
  this->items.clear();
  this->items.insert( std::make_pair< _MapKey, Engine::Rect< uint32_t > >( _MapKey( size.width * size.height, 0, 0 ), Engine::Rect< uint32_t >( 0, 0, size.width - 1, size.height - 1 ) ) );
}//Init


bool SquareAtlas::HasPlace( const Engine::Size& size ) {
  SquareAtlasMap::iterator iterEnd = this->items.end();
  for( SquareAtlasMap::iterator iter = this->items.begin(); iter != iterEnd; ++iter ) {
    if( this->_CanBePlaced( size, &iter->second ) ) {
      return true;
    }
  }

  for( SquareAtlasMap::iterator iter = this->items.begin(); iter != iterEnd; ++iter ) {
    LOGI( "=> [%d; %d] - [%d; %d]", iter->second.left, iter->second.top, iter->second.right, iter->second.bottom );
  }

  return false;
}//HasPlace


bool SquareAtlas::Cut( const Engine::Size& size, Engine::Rect< uint32_t > *result ) {
  //поиск достаточной области
  SquareAtlasMap::iterator iter, iter2, iterEnd;
  bool  can = false;
  Engine::Rect< uint32_t > placeTo;

  iterEnd = this->items.end();
  for( iter = this->items.begin(); iter != iterEnd; ++iter ) {
    if( this->_CanBePlaced( size, &iter->second ) ) {
      can = true;
      placeTo = Engine::Rect< uint32_t >( iter->second.left, iter->second.top, iter->second.left + size.width - 1, iter->second.top + size.height - 1 );
      break;
    }
  }

  if( !can ) {
    LOGW( "SquareAtlas::Cut => can't" );
    return false;
  }

  //место есть => режем все пересекающие области
  //предположительно (как бы это ещё проверить?) - новый блок не может быть в центре какого-либо другого блока,
  //  только перекрывать с бока/угла (да и с угла - вряд ли)
  Engine::Rect< uint32_t > rect;
  iterEnd = this->items.end();
  for( iter = this->items.begin(); iter != iterEnd; ++iter ) {
    if( this->_IsIntersect( placeTo, iter->second ) ) { //нужная площадь пересекает свободную => обрезаем
      //разрезаем iter->second
      Engine::Rect< uint32_t > iterRect = iter->second;
      this->items.erase( iter );

      //Нужная площадь всегда в левом-верхнем углу
      rect = Engine::Rect< uint32_t >( placeTo.left, placeTo.bottom + 1, iterRect.right, iterRect.bottom );
	    this->items.insert( SquareAtlasMap::value_type( _MapKey( rect ), rect ) );

      rect = Engine::Rect< uint32_t >( placeTo.right + 1, placeTo.top, iterRect.right, placeTo.bottom );
	    this->items.insert( SquareAtlasMap::value_type( _MapKey( rect ), rect ) );

      break;
    }//обрезка
  }//for items

  //проверка на поглощение одних свободных блоков другими
  bool deleted = true;

  while( deleted ) {
    deleted = false;
    iterEnd = this->items.end();
    for( iter = this->items.begin(); iter != iterEnd && !deleted; ++iter ) {
      for( iter2 = iter; iter2 != iterEnd; ++iter2 ) {
        if( iter == iter2 ) {
          continue;
        }
        if( this->_IsHave( iter->second, iter2->second ) ) { //iter полностью в iter2
          deleted = true;
          this->items.erase( iter );
          iter = this->items.begin();
          break;
        }
        else
        if( this->_IsHave( iter2->second, iter->second ) ) { //iter2 полностью в iter
          deleted = true;
          this->items.erase( iter2 );
          iter = this->items.begin();
          break;
        }
      }//for
    }
  }//while deleted

  if( result )
    *result = placeTo;

  this->OptimizeFreeSpace();
  return true;
}//Cut


// Если есть возможность - объединить свободные блоки, иначе просто добавить новый свободный блок
void SquareAtlas::Release( const Engine::Rect< uint32_t > &rect ) {
  for( auto &freeRect: this->items ) {
    if( freeRect.second.top == rect.top && freeRect.second.bottom == rect.bottom ) {
      if( freeRect.second.left == rect.right + 1 ) {
        freeRect.second.left = rect.left;
        this->OptimizeFreeSpace();
        return;
      } else if( freeRect.second.right == rect.left - 1 ) {
        freeRect.second.right = rect.right;
        this->OptimizeFreeSpace();
        return;
      }
    } else if( freeRect.second.left == rect.left && freeRect.second.right == rect.right ) {
      if( freeRect.second.top == rect.bottom + 1 ) {
        freeRect.second.top = rect.top;
        this->OptimizeFreeSpace();
        return;
      } else if( freeRect.second.bottom == rect.top - 1 ) {
        freeRect.second.bottom = rect.bottom;
        this->OptimizeFreeSpace();
        return;
      }
    }
  }

	this->items.insert( SquareAtlasMap::value_type( _MapKey( rect ), rect ) );
  this->OptimizeFreeSpace();
}//Release


bool SquareAtlas::_CanBePlaced( const Engine::Size& _who, const Engine::Rect<uint32_t> *_where ) {
  if( _who.width > _where->right - _where->left + 1 ) {
    return false;
  }

  if( _who.height > _where->bottom - _where->top + 1 ) {
    return false;
  }

  return true;
}//_CanBePlaced


bool SquareAtlas::_IsIntersect( const Engine::Rect< uint32_t >& square1, const Engine::Rect< uint32_t >& square2 ) {
  if( square1.right < square2.left ) {
    return false;
  }

  if( square1.left > square2.right ) {
    return false;
  }

  if( square1.top > square2.bottom ) {
    return false;
  }

  if( square1.bottom < square2.top ) {
    return false;
  }

  return true;
}//_IsIntersect


bool SquareAtlas::_IsHave( const Engine::Rect< uint32_t >& minSquare, const Engine::Rect< uint32_t >& maxSquare ) {
  return
    (
    minSquare.left   >= maxSquare.left  &&
    minSquare.right  <= maxSquare.right &&
    minSquare.top    >= maxSquare.top   &&
    minSquare.bottom <= maxSquare.bottom
    )
  ;
}//_IsHave

void SquareAtlas::OptimizeFreeSpace() {
  LOGI( "Before optimization:" );
  this->__Dump();

  bool complete;
  do {
    complete = true;
    auto
      iterFirst = this->items.rbegin(),
      iterEnd = this->items.rend();
    auto
      iterFirstNormal = this->items.end();
    Engine::Rect< uint32_t >
      firstPlace,
      secondPlace;

    while( complete && iterFirst != iterEnd ) {
      --iterFirstNormal;
      LOGI( "optimize: [%d; %d] - [%d; %d]", iterFirst->second.left, iterFirst->second.top, iterFirst->second.right, iterFirst->second.bottom );

      auto iterSecond = iterFirst;
      auto iterSecondNormal = iterFirstNormal;
      ++iterSecond;
      while( complete && iterSecond != iterEnd ) {
        --iterSecondNormal;
        if( this->_CanBeCombined( iterFirst->second, iterSecond->second, &firstPlace, &secondPlace ) ) {
          this->items.erase( iterFirstNormal );
          this->items.erase( iterSecondNormal );
          this->items.insert( std::make_pair( _MapKey( firstPlace ), firstPlace ) );
          if( secondPlace.left != secondPlace.right || secondPlace.top != secondPlace.bottom ) {
            this->items.insert( std::make_pair( _MapKey( secondPlace ), secondPlace ) );
          }
          complete = false;
          break;
        }
        ++iterSecond;
      }//2

      ++iterFirst;
    }//1
  } while( !complete );

  LOGI( "After optimization:" );
  this->__Dump();
}//OptimizeFreeSpace

bool SquareAtlas::_CanBeCombined( Engine::Rect< uint32_t > firstPlace, Engine::Rect< uint32_t > secondPlace, Engine::Rect< uint32_t > *firstResult, Engine::Rect< uint32_t > *secondResult ) {
  LOGI( "_CanBeCombined src: [%d; %d] - [%d; %d]", firstPlace.left, firstPlace.top, firstPlace.right, firstPlace.bottom );
  LOGI( "_CanBeCombined dst: [%d; %d] - [%d; %d]", secondPlace.left, secondPlace.top, secondPlace.right, secondPlace.bottom );
  bool leftOrRightSame = ( firstPlace.right == secondPlace.right || firstPlace.left == secondPlace.left );
  bool TopOrBottomSame = ( firstPlace.bottom == secondPlace.bottom || firstPlace.top == secondPlace.top );
  bool leftOrRightDifferent = ( firstPlace.right != secondPlace.right || firstPlace.left != secondPlace.left );
  bool TopOrBottomDifferent = ( firstPlace.bottom != secondPlace.bottom || firstPlace.top != secondPlace.top );
  bool leftOrRightTooClose = ( firstPlace.left == secondPlace.right + 1 || firstPlace.right == secondPlace.left - 1 );
  bool TopOrBottomTooClose = ( firstPlace.top == secondPlace.bottom + 1 || firstPlace.bottom == secondPlace.top - 1 );

  //1. области полностью совпадают одними из сторон (left&right либо top&bottom)
  //2. области прилегают одной из перпендикулярных пункту 1 сторон
  if( leftOrRightSame && !leftOrRightDifferent && TopOrBottomTooClose
    || TopOrBottomSame && !TopOrBottomDifferent && leftOrRightTooClose
    ) {
      LOGI( "test:2" );
      this->_Combine( firstPlace, secondPlace, *firstResult, *secondResult );
      return true;
  }

  //1. области прилегают друг к другу
  //2. одна из сторон, перпендикулярная прилегающим, совпадает у обоих областей
  //3. противоположные стороны пункту 2 не совпадают
  // если после переразбивки площадь получается больше - переразбить
  bool
    isVertical = leftOrRightSame && leftOrRightDifferent && TopOrBottomTooClose,
    isHorizontal = TopOrBottomSame && TopOrBottomDifferent && leftOrRightTooClose;
  if( isVertical || isHorizontal ) {
    size_t square =
      ( isVertical ?
        ( Min2( firstPlace.right - firstPlace.left, secondPlace.right - secondPlace.left ) + 1 ) * ( ( firstPlace.bottom - firstPlace.top ) + ( secondPlace.bottom  - secondPlace.top ) + 2 )
        : ( Min2( firstPlace.bottom - firstPlace.top, secondPlace.bottom - secondPlace.top ) + 1 ) * ( ( firstPlace.right - firstPlace.left ) + ( secondPlace.right - secondPlace.left ) + 2 )
      );
    size_t squareFirst = ( firstPlace.right - firstPlace.left + 1 ) * ( firstPlace.bottom - firstPlace.top + 1 );
    size_t squareSecond = ( secondPlace.right - secondPlace.left + 1 ) * ( secondPlace.bottom - secondPlace.top + 1 );
    if( square > squareFirst && square > squareSecond ) {
      LOGI( "test:1 %d => %d,%d", square, squareFirst, squareSecond );
      if( firstResult ) {
        auto edge = this->_GetSliceEdge( firstPlace, secondPlace );
        LOGI( ".slice edge[%d; %d]", edge.x, edge.y );
        this->_Slice( firstPlace, secondPlace, *firstResult, *secondResult );
      }
      return true;
    }
  }

  return false;
}//_CanBeCombined

Engine::Position SquareAtlas::_GetSliceEdge( const Engine::Rect< uint32_t > &firstPlace, const Engine::Rect< uint32_t > &secondPlace ) {
  if( firstPlace.bottom == secondPlace.top - 1 || firstPlace.top == secondPlace.bottom + 1 ) { //top & bottom
    if( firstPlace.left == secondPlace.left ) {
      return Engine::Position( Min2( firstPlace.right, secondPlace.right ), 0 );
    } else if( firstPlace.right == secondPlace.right ) {
      return Engine::Position( Max2( firstPlace.left, secondPlace.left ), 0 );
    }
  }

  if( firstPlace.right == secondPlace.left - 1 || firstPlace.left == secondPlace.right + 1 ) { //left & right
    if( firstPlace.top == secondPlace.top ) {
      return Engine::Position( Min2( firstPlace.bottom, secondPlace.bottom ), 0 );
    } else if( firstPlace.bottom == secondPlace.bottom ) {
      return Engine::Position( Max2( firstPlace.top, secondPlace.top ), 0 );
    }
  }

  return Engine::Position();
}//_GetSliceEdge

//Если 2 области прилегают друг к другу и площадь от их переразбивки станет большей - они переразбиваются
void SquareAtlas::_Slice( const Engine::Rect< uint32_t > &firstPlace, const Engine::Rect< uint32_t > &secondPlace, Engine::Rect< uint32_t > &result0, Engine::Rect< uint32_t > &result1 ) {
  Engine::Position axis( this->_GetSliceEdge( firstPlace, secondPlace ) );

  if( !axis.x && !axis.y ) {
    return;
  }

  const Engine::Rect< uint32_t >
    *toCombine = 0,
    *toSlice = 0;

  if( axis.x ) { //vertical
    toCombine = ( firstPlace.left == axis.x || firstPlace.right == axis.x ? &firstPlace : &secondPlace );
    toSlice = ( toCombine == &firstPlace ? &secondPlace : &firstPlace );
    result0.left = toCombine->left;
    result0.right = toCombine->right;
    result0.top = Min2( firstPlace.top, secondPlace.top );
    result0.bottom = Max2( firstPlace.bottom, secondPlace.bottom );
    bool sliceByLeft = ( firstPlace.left != secondPlace.left );
    result1.left = ( sliceByLeft ? Min2( firstPlace.left, secondPlace.left ) : Min2( firstPlace.right, secondPlace.right ) + 1 );
    result1.right = ( sliceByLeft ? Max2( firstPlace.left, secondPlace.left ) - 1 : Max2( firstPlace.right, secondPlace.right ) );
    result1.top = toSlice->top;
    result1.bottom = toSlice->bottom;
    LOGI( "slice parts: [%d; %d]-[%d; %d] && [%d; %d]-[%d; %d]", result0.left, result0.top, result0.right, result0.bottom, result1.left, result1.top, result1.right, result1.bottom );
  } else { //horizontal
    toCombine = ( firstPlace.top == axis.y || firstPlace.bottom == axis.y ? &firstPlace : &secondPlace );
    toSlice = ( toCombine == &firstPlace ? &secondPlace : &firstPlace );
    result0.left = Min2( firstPlace.left, secondPlace.left );
    result0.right = Max2( firstPlace.right, secondPlace.right );
    result0.top = toCombine->top;
    result0.bottom = toCombine->bottom;
    bool sliceByTop = ( firstPlace.top != secondPlace.top );
    result1.left = toSlice->left;
    result1.right = toSlice->right;
    result1.top = ( sliceByTop? Min2( firstPlace.top, secondPlace.top ) : Min2( firstPlace.bottom, secondPlace.bottom ) + 1 );
    result1.bottom = ( sliceByTop? Max2( firstPlace.top, secondPlace.top ) - 1 : Max2( firstPlace.bottom, secondPlace.bottom ) );
    LOGI( "slice parts: [%d; %d]-[%d; %d] && [%d; %d]-[%d; %d]", result0.left, result0.top, result0.right, result0.bottom, result1.left, result1.top, result1.right, result1.bottom );
  }
}//_Slice


void SquareAtlas::_Combine( const Engine::Rect< uint32_t > &firstPlace, const Engine::Rect< uint32_t > &secondPlace, Engine::Rect< uint32_t > &result0, Engine::Rect< uint32_t > &result1 ) {
  result0.left = Min2( firstPlace.left, secondPlace.left );
  result0.right = Max2( firstPlace.right, secondPlace.right);
  result0.top = Min2( firstPlace.top, secondPlace.top );
  result0.bottom = Max2( firstPlace.bottom, secondPlace.bottom );
  result1.left = result1.right = result1.top = result1.bottom = 0;
  LOGI( "Combined from [%d; %d]-[%d; %d] & [%d; %d]-[%d; %d] => [%d; %d]-[%d; %d]",
    firstPlace.left, firstPlace.top, firstPlace.right, firstPlace.bottom,
    secondPlace.left, secondPlace.top, secondPlace.right, secondPlace.bottom,
    result0.left, result0.top, result0.right, result0.bottom
    );
}//_Combine


void SquareAtlas::__Dump() {
  LOGI( "== SquareAtlas begin ==" );
  for( auto &rect: this->items ) {
    LOGI( "=> [%d; %d]-[%d; %d] = %d", rect.second.left, rect.second.top, rect.second.right, rect.second.bottom, ( rect.second.right - rect.second.left + 1 ) * ( rect.second.bottom - rect.second.top + 1 ) );
  }
  LOGI( "== SquareAtlas end ==" );
}//__Dump
