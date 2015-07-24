#include <iostream>
#include <string>

using cbtype = int (*)(const char* key, const char* value, int len);

void get_value(const char* key, cbtype cb)
{
  (*cb)(key, "abc", 3);
}

int oldcb(const char* key, const char* value, int len)
{
  std::cout << "old cb: " << "key: " << key << " value: " << value << std::endl;
  return 0;
}

#if defined(__MAIN__)
int main(int argc, char* argv[])
{
  get_value("123", oldcb);
  get_value("456", [](const char* key, const char* value, int len) {
      std::cout << "lambda cb: " << "key: " << key << " value: " << value << std::endl;
      return 0;
    });
  return 0;
}
#endif
