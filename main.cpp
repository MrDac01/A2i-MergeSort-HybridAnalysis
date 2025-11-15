#include "iostream"
#include "vector"

void insertionSort(std::vector<int> &arr, int left, int right) {
     for (int i = left + 1; i <= right; ++i) {
          int key = arr[i];
          int j = i - 1;

          while (j >= left && arr[j] > key) {
               arr[j + 1] = arr[j];
               j--;
          }
          arr[j + 1] = key;
     }
}

void mergeSort(std::vector<int> &arr, std::vector<int> &tmpArr, int left, int right) {
     if (right - left <= 15) {
          insertionSort(arr, left, right);
          return;
     }

     int mid = (left + right) / 2;
     mergeSort(arr, tmpArr, left, mid);
     mergeSort(arr, tmpArr, mid + 1, right);

     if (arr[mid] <= arr[mid + 1]) {
          return;
     }

     int i = left;
     int j = mid + 1;
     int k = left;

     while (i <= mid && j <= right) {
          if (arr[i] <= arr[j]) {
               tmpArr[k++] = arr[i++];
          } else {
               tmpArr[k++] = arr[j++];
          }
     }

     while (i <= mid) {
          tmpArr[k++] = arr[i++];
     }
     while (j <= right) {
          tmpArr[k++] = arr[j++];
     }

     for (int g = left; g <= right; ++g) {
          arr[g] = tmpArr[g];
     }
}

int main() {
     std::ios::sync_with_stdio(false);
     std::cin.tie(nullptr);

     int n;
     std::cin >> n;
     std::vector<int> arr(n);
     for (int i = 0; i < n; ++i) {
          std::cin >> arr[i];
     }
     if (n > 1) {
          std::vector<int> tmp(n);
          mergeSort(arr, tmp, 0, n - 1);
     }

     for (int i = 0; i < n; ++i) {
          std::cout << arr[i] << " ";
     }
     return 0;
}