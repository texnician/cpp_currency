#include "util.h"
#include <functional>
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

static std::vector<bool> get_flags(const Creative& cr)
{
  return { 1, 0, 1, 1, 0 };
}

static void process_creative(bool flag, Creative& cr)
{
  std::cout << "cr flag: " << flag << '\n';
}

#if defined(__MAIN__)
int main(int argc, char* argv[])
{
  // 用auto生命变量必须初始化
  auto i = 0;

  // 编译器生成的closure必须用auto初始化
  auto dref_less =
    [](const std::unique_ptr<int>& p1,
       const std::unique_ptr<int>& p2)
    { return *p1 < *p2; };

  PRINT_TYPE(dref_less);

  // functional 比 closure更慢，占用更多内存
  std::function<bool(const std::unique_ptr<int>&,
                      const std::unique_ptr<int>&)>
     deref_less_func = [](const std::unique_ptr<int>& p1,
                          const std::unique_ptr<int>& p2)
    { return *p1 < *p2; };
  PRINT_TYPE(deref_less_func);

  // 用auto可以避免类型隐式转化
  // ex: auto now = CommonUtil::GetTimestamp()
  std::vector<int> v1;
  int s1 = v1.size();
  auto s2 = v1.size();
  PRINT_TYPE(s1);
  PRINT_TYPE(s2);
  
  const int* p1{ nullptr };
  std::unordered_map<std::string, int> m = { { "a", 1 }, { "b", 2 } };
    
  PRINT_TYPE(*m.begin());

  // 类型不对, 没有const编译不过, 因为类型不匹配
  for (const std::pair<std::string, int>& p : m)
  {
    // p1不安全，指向临时对象
    p1 = &(p.second);
  }
  std::cout <<"p1 @" << std::hex << intptr_t(p1) << std::endl;
  const int* p2{ nullptr };
  for (const auto& p : m)
  {
    // p2指向map真正的地址
    p2 = &(p.second);
  }
  std::cout << "p2 @" << std::hex << intptr_t(p2) << std::endl;
  std::cout << "m[\"a\"]" << " @" << std::hex << intptr_t(&m["a"]) << std::endl;

  Creative cr;
  {
    bool disabled = get_flags(cr)[4];
    process_creative(disabled, cr);
    std::cout << "cr disabled: " << disabled << '\n';
  }
  
  {
    auto disabled = get_flags(cr)[4];
    process_creative(disabled, cr);
    PRINT_TYPE(disabled);
  }
}
#endif
