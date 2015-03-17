#include "util.h"

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

template<typename T>
void byval(T param)
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
  std::cout << std::endl;
}

template<typename T>
class TD;

// Case2: 参数是universal reference
// 1. 如果expr是lvalue或者reference，T和param都解释为引用; 唯一的情况T被推导成引用，虽然param声明为lvalue，但是仍然推导为lvalue引用
// 2. 如果exprt是rvalue，按照Case1处理 （忽略引用）
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
  std::cout << std::endl;
}

// Case3: 既不是指针也不是引用（传值）
// 1. 如果expr是引用，忽略引用
// 2. 如果exprt去掉引用以后有const或者volitile，也全部忽略
void case3()
{
  std::cout << "Case3: param type is neither a pointer nor a reference\n";
  int x = 27;
  const int cx = x;
  const int& rx = cx;
  const char* ptr = "aaa";
  const char* const cptr = ptr;

  DEDUCE_TL_TYPE(byval, x);
  DEDUCE_TL_TYPE(byval, cx);
  DEDUCE_TL_TYPE(byval, rx);
  DEDUCE_TL_TYPE(byval, ptr);
  DEDUCE_TL_TYPE(byval, cptr);
  std::cout << std::endl;
}

// 数组和函数都被推导为指针类型，除非param声明为引用
void array()
{
  std::cout << "Array Argument deduce\n";
  const char name[] = "Tang";
  const char* ptr = name;
  
  DEDUCE_TL_TYPE(byval, name);
  DEDUCE_TL_TYPE(byval, ptr);
  DEDUCE_TL_TYPE(lvref, name);
  DEDUCE_TL_TYPE(lvref, ptr);
  std::cout << std::endl;
}

template<typename T>
std::string value_type(T&& val)
{
  return type_id_with_cvr<decltype(val)>().pretty_name();
}

#define DEDUCE_AUTO_TYPE(expr) cout << #expr << " => [" \
  << type_id_with_cvr<decltype(expr)>().pretty_name() \
  << "]\n";

void auto_deduce()
{
  std::cout << "auto type deduce\n";
  auto x = 27;
  const auto cx = x;
  const auto& rx = x;
  auto&& uref1 = x;
  auto&& uref2 = cx;
  auto&& uref3 = 27;
  auto x1 = 27; 
  auto x2(27); 
  auto x3 = { 27 };
  auto x4{ 27 };
  int x5 = { 27 };
  int x6{ 27 };
  DEDUCE_AUTO_TYPE(x);
  DEDUCE_AUTO_TYPE(cx);
  DEDUCE_AUTO_TYPE(rx);
  DEDUCE_AUTO_TYPE(uref1);
  DEDUCE_AUTO_TYPE(uref2);
  DEDUCE_AUTO_TYPE(uref3);
  DEDUCE_AUTO_TYPE(x1);
  DEDUCE_TL_TYPE(byval, x1);
  DEDUCE_AUTO_TYPE(x2);
  DEDUCE_TL_TYPE(byval, x2);
  DEDUCE_AUTO_TYPE(x3);
  DEDUCE_AUTO_TYPE(x4);
  DEDUCE_TL_TYPE(byval, x5);
  DEDUCE_TL_TYPE(byval, x6);
  std::cout << std::endl;
}

#if defined(__MAIN__)
int main(int argc, char* argv[])
{
  case1();
  case2();
  case3();
  array();
  auto_deduce();
}
#endif
