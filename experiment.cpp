#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>

void insertionSort(std::vector<int> &a, int l, int r) {
    for (int i = l + 1; i <= r; i++) {
        int key = a[i];
        int j = i - 1;
        while (j >= l && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

void mergeSort(std::vector<int> &a, std::vector<int> &tmp, int l, int r) {
    if (l >= r) return;
    int m = (l + r) / 2;
    mergeSort(a, tmp, l, m);
    mergeSort(a, tmp, m + 1, r);

    if (a[m] <= a[m + 1]) return;

    int i = l, j = m + 1, k = l;
    while (i <= m && j <= r)
        tmp[k++] = (a[i] <= a[j] ? a[i++] : a[j++]);
    while (i <= m) tmp[k++] = a[i++];
    while (j <= r) tmp[k++] = a[j++];

    for (int t = l; t <= r; t++)
        a[t] = tmp[t];
}

void mergeSortStandard(std::vector<int> &a) {
    if (a.empty()) return;
    std::vector<int> tmp(a.size());
    mergeSort(a, tmp, 0, (int)a.size() - 1);
}


void mergeSortHybrid(std::vector<int> &a, std::vector<int> &tmp, int l, int r, int threshold) {
    if (r - l + 1 <= threshold) {
        insertionSort(a, l, r);
        return;
    }
    int m = (l + r) / 2;
    mergeSortHybrid(a, tmp, l, m, threshold);
    mergeSortHybrid(a, tmp, m + 1, r, threshold);

    if (a[m] <= a[m + 1]) return;

    int i = l, j = m + 1, k = l;
    while (i <= m && j <= r)
        tmp[k++] = (a[i] <= a[j] ? a[i++] : a[j++]);
    while (i <= m) tmp[k++] = a[i++];
    while (j <= r) tmp[k++] = a[j++];
    for (int t = l; t <= r; t++) a[t] = tmp[t];
}

void mergeSortHybrid(std::vector<int> &a, int threshold) {
    if (a.empty()) return;
    std::vector<int> tmp(a.size());
    mergeSortHybrid(a, tmp, 0, (int)a.size() - 1, threshold);
}


class ArrayGenerator {
private:
    std::vector<int> randomBase;
    std::vector<int> reverseBase;
    std::vector<int> sortedBase;
public:
    ArrayGenerator(int maxN = 100000) {
        randomBase.resize(maxN);
        std::mt19937 rng(123);
        std::uniform_int_distribution<int> dist(0, 6000);
        for (int &x : randomBase) x = dist(rng);
        reverseBase = randomBase;
        std::sort(reverseBase.begin(), reverseBase.end(), std::greater<int>());
        sortedBase = randomBase;
        std::sort(sortedBase.begin(), sortedBase.end());
    }

    std::vector<int> getRandom(int n) { return std::vector<int>(randomBase.begin(), randomBase.begin() + n); }
    std::vector<int> getReverse(int n) { return std::vector<int>(reverseBase.begin(), reverseBase.begin() + n); }
    std::vector<int> getAlmost(int n, int swaps = 100) {
        std::vector<int> v(sortedBase.begin(), sortedBase.begin() + n);
        std::mt19937 rng(777);
        std::uniform_int_distribution<int> d(0, n - 1);
        for (int i = 0; i < swaps; i++)
            std::swap(v[d(rng)], v[d(rng)]);
        return v;
    }
};

class SortTester {
private:
    ArrayGenerator gen;

    template<typename Func>
    static long long measure(Func sortFn, std::vector<int> arr) {
        auto start = std::chrono::high_resolution_clock::now();
        sortFn(arr);
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    }

public:
    void runCSV() {
        std::ofstream fout("results.csv");
        fout << "n,rand_std,rand_hyb,rev_std,rev_hyb,alm_std,alm_hyb\n";
        int threshold = 15;

        for (int n = 500; n <= 100000; n += 100) {
            auto r  = gen.getRandom(n);
            auto rv = gen.getReverse(n);
            auto al = gen.getAlmost(n);

            long long r1 = measure(mergeSortStandard, r);
            long long r2 = measure([&](std::vector<int> &x){ mergeSortHybrid(x, threshold); }, r);

            long long r3 = measure(mergeSortStandard, rv);
            long long r4 = measure([&](std::vector<int> &x){ mergeSortHybrid(x, threshold); }, rv);

            long long r5 = measure(mergeSortStandard, al);
            long long r6 = measure([&](std::vector<int> &x){ mergeSortHybrid(x, threshold); }, al);

            fout << n << "," << r1 << "," << r2 << "," << r3 << "," << r4 << "," << r5 << "," << r6 << "\n";
        }
        fout.close();
        std::cout << "Results saved to results.csv\n";
    }
};

int main() {
    SortTester tester;
    tester.runCSV();
    return 0;
}
