////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
template <typename T>
std::ostream &operator<<( std::ostream &s, const std::vector<T> &v ){
  s << "[";
  if( !v.empty() ){
    std::copy( v.begin(), v.end(), std::ostream_iterator<T>( s, "," ));
    s << " \b\b"; }
  return s << "]"; }
