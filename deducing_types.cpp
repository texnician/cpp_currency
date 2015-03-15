#include <iostream>
#include <boost/type_index.hpp>

using boost::typeindex::type_id_with_cvr;
using std::cout;

#define DEDUCE_TL_TYPE(tl, expr) \
  cout << #expr << " => [" << type_id_with_cvr<decltype(expr)>().pretty_name() << "], "; tl(expr)

template<typename T>
void lvref(T& param)
{
  cout << "T => ["
       << type_id_with_cvr<T>().pretty_name() << "], ";
  cout << "param => ["
       << type_id_with_cvr<decltype(param)>().pretty_name()
       << "]\n";
}

template<typename T>
void clvref(const T& param)
{
  cout << "T => ["
       << type_id_with_cvr<T>().pretty_name() << "], ";
  cout << "param => ["
       << type_id_with_cvr<decltype(param)>().pretty_name()
       << "]\n";
}

template<typename T>
void ptr(T* param)
{
  cout << "T => ["
       << type_id_with_cvr<T>().pretty_name() << "], ";
  cout << "param => ["
       << type_id_with_cvr<decltype(param)>().pretty_name()
       << "]\n";
}

template<typename T>
void uref(T&& param)
{
  cout << "T => ["
       << type_id_with_cvr<T>().pretty_name() << "], ";
  cout << "param => ["
       << type_id_with_cvr<decltype(param)>().pretty_name()
       << "]\n";
}

// Case1: 参数是引用或者指针
// 1. 如果expr是引用，忽略引用
// 2. 用expr匹配param，根据param推导T的类型

void case1()
{
  std::cout << "Case1: param type is reference or pointer\n";
  int x = 27;
  const int cx = x;
  const int& rx = cx;
  DEDUCE_TL_TYPE(lvref, x);
  DEDUCE_TL_TYPE(lvref, cx);
  DEDUCE_TL_TYPE(lvref, rx);

  DEDUCE_TL_TYPE(clvref, x);
  DEDUCE_TL_TYPE(clvref, cx);
  DEDUCE_TL_TYPE(clvref, rx);
  
  const int *px = &x;
  DEDUCE_TL_TYPE(ptr, &x);
  DEDUCE_TL_TYPE(ptr, px);
}

template<typename T>
class TD;

void case2()
{
  std::cout << "Case2: param type is universal reference\n";
  int x = 27;
  const int cx = x;
  const int& rx = cx;

  DEDUCE_TL_TYPE(uref, x);
  DEDUCE_TL_TYPE(uref, cx);
  DEDUCE_TL_TYPE(uref, rx);
  DEDUCE_TL_TYPE(uref, std::move(rx));
  DEDUCE_TL_TYPE(uref, 27);
}

#if defined(__MAIN__)
int main(int argc, char* argv[])
{
  case1();
  case2();
}
#endif
