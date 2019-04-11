#include <iostream>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

void wait(int seconds)
{
    boost::this_thread::sleep_for(boost::chrono::seconds{seconds});
}

void thread()
{
    try
    {
        for(int i = 0; i < 5; ++i)
        {
            std::cout << i << '\n';
            wait(1);
        }
    }
    catch(boost::thread_interrupted&)
    {
        std::cout << "Interrupted!\n";
    }
}

int main(int argc, char** argv)
{
    boost::thread t{thread};
    wait(3);
    t.interrupt();
    t.join();

    return 0;
}