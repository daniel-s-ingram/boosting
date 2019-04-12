#include <cstdint>
#include <iostream>
#include <vector>
#include <boost/thread/thread.hpp>
#include <boost/timer/timer.hpp>

const int N = 1000000000;
const int NTHREADS = boost::thread::hardware_concurrency();
const int N_PER_THREAD = N/NTHREADS;

class Adder
{
    friend std::uint64_t operator+(std::uint64_t lhs, const Adder& rhs);

    private:
        std::uint64_t sum = 0;
        int start, stop;

    public:
        Adder(int start, int stop) : start(start), stop(stop) {}

        void operator()()
        {
            for (int i = start; i < stop; ++i)
                sum += i;
        }
};

std::uint64_t operator+(std::uint64_t lhs, const Adder& rhs)
{
    return lhs + rhs.sum;
}

int main(int argc, char** argv)
{
    boost::timer::cpu_timer timer;

    boost::thread_group threads;
    std::vector<Adder> adders;
    adders.reserve(NTHREADS);
    for (int i = 0; i < NTHREADS; ++i)
    {
        int start = i * N_PER_THREAD;
        int stop = i == NTHREADS - 1 ? N + 1 : (i + 1) * N_PER_THREAD;
        adders.push_back(Adder(start, stop));
        threads.create_thread(boost::ref(adders[i]));
    }
    threads.join_all();
    std::uint64_t sum = 0;
    for (int i = 0; i < NTHREADS; ++i)
        sum = sum + adders[i];

    std::cout << sum << '\n';
    std::cout << timer.format() << '\n';
    return 0;
}