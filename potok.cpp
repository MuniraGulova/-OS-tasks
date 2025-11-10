#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

class ThreadSum {
private:
    std::vector<int> vec;
public:
    ThreadSum() {
        vec.resize(100000000);
        srand(static_cast<unsigned>(time(0)));

        for (int i = 0; i < 10000000; i++) {
            vec[i] = rand() % 100;
        }
    }

    int sum_single_thread() {
       int sum = 0;
        for (int i = 0; i < 100000000; i++) {
            sum += vec[i];
        }
        return sum;
    }

    int sum_part(int start, int end) {
        int local_sum = 0;
        for (int i = start; i < end; i++) {
            local_sum += vec[i];
        }
        return local_sum;
    }

    int sum_four_threads() {
        int part_size = 25000000;
        int results[4] = {0};

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

        int total = 0;
        for (int i = 0; i < 4; i++) {
            total += results[i];
        }
        return total;
    }
};

int main() {
    ThreadSum obj;

    //Однопоточная sum
    auto start1 = std::chrono::high_resolution_clock::now();
    int sum1 = obj.sum_single_thread();
    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff1 = end1 - start1;

    std::cout << "Sum (1 potok): " << sum1 << "\n";
    std::cout << "Time (1 potok): " << diff1.count() << " sec\n\n";

    //Четырёхпоточная
    auto start2 = std::chrono::high_resolution_clock::now();
    int sum2 = obj.sum_four_threads();
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff2 = end2 - start2;

    std::cout << "Sum (4 potok): " << sum2 << "\n";
    std::cout << "Time (4 potok): " << diff2.count() << " sec\n";

    return 0;
}

/*Result
Sum (1 potok): 494568461
Time (1 potok): 0.520452 sec

Sum (4 potok): 494568461
Time (4 potok): 0.124251 sec
 **/
