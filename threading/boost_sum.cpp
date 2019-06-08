#include <cstdint>
#include <iostream>
#include <vector>
#include <memory>
#include <boost/thread/thread.hpp>
#include <boost/timer/timer.hpp>

const int N = 1000000000;
const int NTHREADS = boost::thread::hardware_concurrency();
const int N_PER_THREAD = N/NTHREADS;

class Adder
{
    friend inline std::uint64_t operator+(std::uint64_t lhs, const Adder& rhs);

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

inline std::uint64_t operator+(std::uint64_t lhs, const Adder& rhs)
{
    return lhs + rhs.sum;
}

int main(int argc, char** argv)
{
    std::uint64_t sum = 0;

    boost::timer::cpu_timer st_timer;

    for (int i = 0; i <= N; ++i)
        sum += i;

    std::cout << "Single thread calculated sum of " << sum << " in\n";
    std::cout << st_timer.format() << '\n';

    sum = 0;
    boost::timer::cpu_timer mt_timer;

    boost::thread_group threads;
    std::vector<std::unique_ptr<Adder> > adders;
    adders.reserve(NTHREADS);
    for (int i = 0; i < NTHREADS; ++i)
    {
        int start = i * N_PER_THREAD;
        int stop = i == NTHREADS - 1 ? N + 1 : (i + 1) * N_PER_THREAD;
        adders.emplace_back(new Adder(start, stop));
        threads.add_thread(new boost::thread(boost::ref(*adders[i])));
    }
    threads.join_all();
    for (int i = 0; i < NTHREADS; ++i)
        sum = sum + *adders[i];

    std::cout << NTHREADS << " threads calculated sum of " << sum << " in\n";
    std::cout << mt_timer.format() << '\n';
    return 0;
}