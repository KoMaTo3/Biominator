#include "tools.h"

std::deque< std::string > Explode( const std::string& source, const std::string& separator )
{
  if( !source.length() )
    return std::deque< std::string >();
  std::deque< std::string > res;
  std::deque< std::string >::iterator iter;
  for( unsigned long w, q = 0; q < source.length(); )
  {
    w = source.find( separator, q );
    if(w)
      res.push_back( source.substr( q, w - q ) );
    else  //last
      res.push_back( source.substr( q ) );
    iter = --res.end();
    q += iter->length() + separator.length();
  }
  return res;
}//Explode

