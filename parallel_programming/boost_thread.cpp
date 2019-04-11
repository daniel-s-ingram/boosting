#include <iostream>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

void wait(int seconds)
{
    boost::this_thread::sleep_for(boost::chrono::seconds{seconds});
}

void thread()
{
    for(int i = 1;; i+= 2)
    {
        wait(1);
        std::cout << i << '\n';
        wait(1);
    }
}

int main(int argc, char** argv)
{
    boost::thread t{thread};
    for(int i = 0;; i += 2)
    {
        std::cout << i << '\n';
        wait(2);
    }
}