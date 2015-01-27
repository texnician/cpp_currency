#include <iostream>
#include <thread>

class background_task
{
public:
  void operator() () const
  {
    std::cout << "background function object" << std::endl;
  }
};

#if defined(__MAIN__)
int main(int argc, char* argv[])
{
  std::thread my_thread([](){
      std::cout << "background lambda" << std::endl;
    });
  my_thread.join();
}
#endif
