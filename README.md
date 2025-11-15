Садиков Амир БПИ244
#АиСД #Set

ID Задачи: [349073969](https://dsahse25.contest.codeforces.com/group/SLdI1pWUpC/contest/647790/submission/349073969 "Исходный код")

# Задача

Сравнить стандартную рекурсивную реализацию Merge Sort и гидрибдную реализацию Merge + Insertion Sort, которая для маленьких подмассиов переключается на Insertion sort. Провести измерения на трех видов входных массивов.

- Числа в диапозоне `[0; 6000]`
- массивы отсортированные по невозрастанию
- почти отсортированные массивы

Параметры: размеры массивов от 500 до 100000 с шагом 100

# Решение

Время измерялось с использованием `std::chrono::high_resolution_clock` и переводилось в миллисекунды.

Для гибридного алгоритма в эксперименте использован порог `threshold = 15`

Графики построены с помощью Python (`pandas`, `matplotlib`) единицы измерения миллисекунды

CSV-формат выходных данных: `n,rand_std,rand_hyb,rev_std,rev_hyb,alm_std,alm_hyb`.

Insertion и Merge sort проходили ранее.

Реализация

`ArrayGenerator` генерирует базовые массивы длины 100000. Случайный отсортированным и перевернутым методом `getAlmost(n, swaps)` делает `swaps` обменов в отсортированном префиксе

```c++
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
```

`mergeSortHybrid` та же реализация но при размере блока threshold` вызывается `insertionSort`.

`SortTester::runCSV` собирает `results.csv` замеряет медиану нескольких прогонов.

```c++
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
```

Посмотрим графики

[график 1](almost_sorted_array.png)
[график 2](random_array.png)
[график 3](reverse_array.png)


Наблюдения

1) На малых n гибрид часто выигрывает из-за того, что insertion sort имеет меньшую константу и локальность.
2) На больших n стандартный merge sort стабильно проявляет сложность $O(n \space log n)$ и чаще становится быстрее (или сравним) с гибридом как только overhead от рекурсии и переключений уходит на второй план.
3) На почти-отсортированных массивах гибрид может дать существенное ускорение.
4) На обратных массивах (worst-case для insertion) гибрид даёт меньше выигрыша и иногда проигрывает стандартному merge (если threshold слишком большой).

# Вывод

- Порог переключения (`threshold`) критически влияет на производительность.
- Его стоит подбирать экспериментально Разумный диапазон: 5, 10, 15, 20, 30, 50.
- Для достоверности необходимо:
    - использовать медиану или среднее по нескольким прогонам
    - минимизировать сторонние нагрузки системы
- Гибридный алгоритм действительно ускоряет сортировку  на малых и почти отсортированных массивах.
- Стандартный Merge Sort лучше на больших размерах и на «тяжёлых» входах вроде обратных массивов.