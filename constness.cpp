#include "util.h"
#include <vector>

int main(int argc, char* argv[])
{
  std::vector<int> vec = { 1 };

  for (std::vector<int>::const_iterator iter = vec.begin(); iter != vec.end(); ++iter)
  {
    PRINT_TYPE(iter);
    PRINT_TYPE(vec);
  }

  for (std::vector<int>::iterator iter = vec.begin(); iter != vec.end(); ++iter)
  {
    std::vector<int>::const_iterator citer = iter;
    PRINT_TYPE(citer);
    PRINT_TYPE(iter);
    PRINT_TYPE(vec);
  }

  const std::vector<int>& cvec = vec;
  for (auto iter = cvec.begin(); iter != cvec.end(); ++iter)
  {
    PRINT_TYPE(iter);
    PRINT_TYPE(cvec);
  }
}
