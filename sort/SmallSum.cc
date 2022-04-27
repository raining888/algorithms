#include <iostream>
#include <stdbool.h>
#include <random>

using namespace std;

//在一个数组中，每一个数左边比当前数小的数累加起来，叫做这个数组的小和。求一个给定数组的小和。
//
// 思路：找每一个数右边比当前数大的个数，(个数 * 当前数) 的累加和就是结果。
//
// 这咋和归并排序联系上的呢？在左组和右组merge的时候，会比较数的大小，这时就可以在右组找到比左组当前数大的个数。

class SmallSum
{
  public:
    static int getRandom(int min, int max)
    {
      random_device seed;  // 硬件生成随机数种子
      ranlux48 engine(seed());  // 利用种子生成随机数引
      uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
      int res = distrib(engine);  // 随机数
      return res;
    }

    static int smallSum(int arr[], int len)
    {
      if (arr == NULL || len < 2)
      {
        return 0;
      }
      return process(arr, 0, len - 1);
    }

    // arr[L..R]既要排好序，也要求小和返回
    // 所有merge时，产生的小和，累加
    // 左 排序   merge
    // 右 排序  merge
    // merge
    static int process(int arr[], int l, int r)
    {
      if (l == r)
      {
        return 0;
      }
      // l < r
      int mid = l + ((r - l) >> 1);
      return process(arr, l, mid)
        + process(arr, mid + 1, r)
        + merge(arr, l, mid, r);
    }

    static int merge(int arr[], int L, int m, int r)
    {
      int len = r - L + 1;
      int* help = new int[len];
      int i = 0;
      int p1 = L;
      int p2 = m + 1;
      int res = 0;
      while (p1 <= m && p2 <= r)
      {
        // 当左组的数小于右组的数时， 当前右组的个数*当前数 的累加和 即是小和的结果
        // 仔细和归并排序比较，发现就多了此处的代码。唯一的区别是，
        // 等于的时候拷贝右组的元素，因为要在右组中找出比左组大的个数，肯定不能先拷贝左组的，不然会少算
        res += arr[p1] < arr[p2] ? (r - p2 + 1) * arr[p1] : 0;  // r - p2 + 1为>=arr[p2]的个数
        help[i++] = arr[p1] < arr[p2] ? arr[p1++] : arr[p2++];  // MergeSort里用<=，这里用<
      }
      while (p1 <= m)
      {
        help[i++] = arr[p1++];
      }
      while (p2 <= r)
      {
        help[i++] = arr[p2++];
      }
      for (i = 0; i < len; i++)
      {
        arr[L + i] = help[i];
      }
      return res;
    }

    // for test
    static int comparator(int arr[], int len)
    {
      if (arr == NULL || len < 2)
      {
        return 0;
      }
      int res = 0;
      for (int i = 1; i < len; i++)
      {
        for (int j = 0; j < i; j++)
        {
          res += arr[j] < arr[i] ? arr[j] : 0;
        }
      }
      return res;
    }

    // for test
    static int* generateRandomArray(int maxSize, int maxValue, int* len)
    {
      *len = getRandom(0, maxSize);
      int* arr = new int[*len];
      for (int i = 0; i < *len; i++)
      {
        arr[i] = getRandom(0, maxValue) - getRandom(0, maxValue);
      }
      return arr;
    }

    // for test
    static int* copyArray(int* arr, int len)
    {
      if (arr == NULL)
      {
        return NULL;
      }
      int* res = new int[len];
      for (int i = 0; i < len; i++)
      {
        res[i] = arr[i];
      }
      return res;
    }

    // for test
    static bool isEqual(int arr1[], int arr2[], int len1, int len2)
    {
      if ((arr1 == NULL && arr2 != NULL) || (arr1 != NULL && arr2 == NULL))
      {
        return false;
      }
      if (arr1 == NULL && arr2 == NULL)
      {
        return true;
      }
      if (len1 != len2)
      {
        return false;
      }
      for (int i = 0; i < len1; i++)
      {
        if (arr1[i] != arr2[i])
        {
          return false;
        }
      }
      return true;
    }

    // for test
    static void printArray(int arr[], int len)
    {
      if (arr == NULL)
      {
        return;
      }
      for (int i = 0; i < len; i++)
      {
        cout<< arr[i] << " ";
      }
      cout << endl;
    }
};

int main()
{
  int testTime = 100;
  int maxSize = 100;
  int maxValue = 100;
  bool succeed = true;
  int len = 0;
  for (int i = 0; i < testTime; i++)
  {
    int* arr1 = SmallSum::generateRandomArray(maxSize, maxValue, &len);
    int* arr2 = SmallSum::copyArray(arr1, len);
    if (SmallSum::smallSum(arr1, len) != SmallSum::comparator(arr2, len))
    {
      succeed = false;
      SmallSum::printArray(arr1, len);
      SmallSum::printArray(arr2, len);
      break;
    }
  }
  cout << (succeed ? "Nice!" : "Fucking fucked!") << endl;
}
