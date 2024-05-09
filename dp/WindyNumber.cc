// 数位dp
// windy数
// 不含前导零且相邻两个数字之差至少为2的正整数被称为windy数
// windy想知道[a,b]范围上总共有多少个windy数
// 测试链接 : https://www.luogu.com.cn/problem/P2657
class Code01_WindyNumber
{
 public:
  static const int MAXLEN = 11;

  static int dp[MAXLEN][11][2];

  void build(int len)
  {
    for (int i = 0; i <= len; i++)
    {
      for (int j = 0; j <= 10; j++)
      {
        dp[i][j][0] = -1;
        dp[i][j][1] = -1;
      }
    }
  }

  int compute(int a, int b) { return cnt(b) - cnt(a - 1); }

  // 求0~num范围上，windy数的个数
  int cnt(int num)
  {
    if (num == 0)
    {
      return 1;
    }
    int len = 1;
    int offset = 1;
    int tmp = num / 10;
    while (tmp > 0)
    {
      len++;
      offset *= 10;
      tmp /= 10;
    }
    build(len);
    return f(num, offset, len, 10, 0);
  }

  // offset完全由len决定，为了方便提取num中某一位数字（上节课内容）
  // 从num的高位开始，还剩下len位没有决定
  // 前一位的数字是pre，如果pre == 10，表示从来没有选择过数字
  // 如果之前的位已经确定比num小，那么free == 1，表示接下的数字可以自由选择
  // 如果之前的位和num一样，那么free == 0，表示接下的数字不能大于num当前位的数字
  // 返回<=num的windy数有多少个
  int f(int num, int offset, int len, int pre, int free)
  {
    if (len == 0)
    {
      return 1;
    }
    if (dp[len][pre][free] != -1)
    {
      return dp[len][pre][free];
    }
    int cur = num / offset % 10;
    int ans = 0;
    if (free == 0)
    {
      if (pre == 10)
      {
        // 之前的位和num一样，此时不能随意选择数字
        // 也从来没有选择过数字
        // 就表示：来到的是num的最高位
        ans += f(num, offset / 10, len - 1, 10, 1);  // 一个数字也不要
        for (int i = 1; i < cur; i++)
        {
          ans += f(num, offset / 10, len - 1, i, 1);
        }
        ans += f(num, offset / 10, len - 1, cur, 0);
      }
      else
      {
        // 之前的位和num一样，此时不能随意选择数字，
        // 之前选择过数字pre
        for (int i = 0; i <= 9; i++)
        {
          if (i <= pre - 2 || i >= pre + 2)
          {
            if (i < cur)
            {
              ans += f(num, offset / 10, len - 1, i, 1);
            }
            else if (i == cur)
            {
              ans += f(num, offset / 10, len - 1, cur, 0);
            }
          }
        }
      }
    }
    else
    {
      if (pre == 10)
      {
        // free == 1，可以自由选择数字，前面的状况 < num
        // 从来没有选择过数字
        ans += f(num, offset / 10, len - 1, 10, 1);  // 还是可以不选择数字
        for (int i = 1; i <= 9; i++)
        {
          ans += f(num, offset / 10, len - 1, i, 1);
        }
      }
      else
      {
        // free == 1，可以自由选择数字，前面的状况 < num
        // 选择过数字pre
        for (int i = 0; i <= 9; i++)
        {
          if (i <= pre - 2 || i >= pre + 2)
          {
            ans += f(num, offset / 10, len - 1, i, 1);
          }
        }
      }
    }
    dp[len][pre][free] = ans;
    return ans;
  }
};
