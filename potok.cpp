#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

class MyClass {
public:
    std::vector<int> vec;

    MyClass() {
        vec.resize(1000000);
        srand(static_cast<unsigned>(time(0)));

        for (int i = 0; i < 1000000; i++) {
            vec[i] = rand() % 100;
        }
    }

    long long sum_single_thread() {
        long long sum = 0;
        for (int i = 0; i < 1000000; i++) {
            sum += vec[i];
        }
        return sum;
    }

    long long sum_part(int start, int end) {
        long long local_sum = 0;
        for (int i = start; i < end; i++) {
            local_sum += vec[i];
        }
        return local_sum;
    }

    long long sum_four_threads() {
        int part_size = 250000;
        long long results[4] = {0};

        std::thread threads[4];

        for (int i = 0; i < 4; i++) {
            int start = i * part_size;
            int end = start + part_size;
            threads[i] = std::thread([&, i, start, end]() {
                results[i] = sum_part(start, end);
            });
        }

        for (int i = 0; i < 4; i++) {
            threads[i].join();
        }

        long long total = 0;
        for (int i = 0; i < 4; i++) {
            total += results[i];
        }
        return total;
    }
};

int main() {
    MyClass obj;

    //Однопоточная sum
    auto start1 = std::chrono::high_resolution_clock::now();
    long long sum1 = obj.sum_single_thread();
    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff1 = end1 - start1;

    std::cout << "Sum (1 potok): " << sum1 << "\n";
    std::cout << "Time (1 potok): " << diff1.count() << " sec\n\n";

    //Четырёхпоточная
    auto start2 = std::chrono::high_resolution_clock::now();
    long long sum2 = obj.sum_four_threads();
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff2 = end2 - start2;

    std::cout << "Sum (4 potok): " << sum2 << "\n";
    std::cout << "Time (4 potok): " << diff2.count() << " sec\n";

    return 0;
}
