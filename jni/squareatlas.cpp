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
  for( SquareAtlasMap::iterator iter = this->items.begin(); iter != iterEnd; ++iter )
    if( this->_CanBePlaced( size, &iter->second ) )
      return true;

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

  return true;
}//Cut


// Если есть возможность - объединить свободные блоки, иначе просто добавить новый свободный блок
void SquareAtlas::Release( const Engine::Rect< uint32_t > &rect ) {
  for( auto &freeRect: this->items ) {
    if( freeRect.second.top == rect.top && freeRect.second.bottom == rect.bottom ) {
      if( freeRect.second.left == rect.right + 1 ) {
        freeRect.second.left = rect.left;
        return;
      } else if( freeRect.second.right == rect.left - 1 ) {
        freeRect.second.right = rect.right;
        return;
      }
    } else if( freeRect.second.left == rect.left && freeRect.second.right == rect.right ) {
      if( freeRect.second.top == rect.bottom + 1 ) {
        freeRect.second.top = rect.top;
        return;
      } else if( freeRect.second.bottom == rect.top - 1 ) {
        freeRect.second.bottom = rect.bottom;
        return;
      }
    }
  }

	this->items.insert( SquareAtlasMap::value_type( _MapKey( rect ), rect ) );
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
