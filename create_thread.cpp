#include <iostream>

class A
{};

class B
{
public:
    B(A a)
        {}
    B(B& other) = delete;
};

B b(A());

#if defined(__MAIN__)
int main(int argc, char* argv[])
{
    
}
#endif
