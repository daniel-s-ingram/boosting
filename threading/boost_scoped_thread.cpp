#include <iostream>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/thread/scoped_thread.hpp>

void wait(int seconds)
{
    boost::this_thread::sleep_for(boost::chrono::seconds{seconds});
}

void thread()
{
    for(int i = 0; i < 5; ++i)
    {
        wait(1);
        std::cout << i << '\n';
    }
}

struct repeat
{
    void operator()(boost::thread& t)
    {
        t.join();
        boost::scoped_thread<repeat> u{boost::thread{thread}};
    }
};

int main(int argc, char** argv)
{
    boost::scoped_thread<repeat> t{boost::thread{thread}};

    return 0;
}