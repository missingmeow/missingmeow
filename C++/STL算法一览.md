# STL 算法一览

本文介绍的是STL中算法，主要是方便查看，可能部分算法你已经忘记了，但当你看到简单介绍或者就是个函数名就能用起来，没有太多的细节东西，需要看具体实现的话还是需要自己搜索具体的算法或者看源代码是如何实现的。

本文的内容主要参考《STL源码剖析》。

加`*`表示可能STL里没有该函数。

## #include \<numeric>

### 1. `accumulate` 元素累计

```C
// 计算 init 和 [first, last) 内所有元素的和
T accumulate(InputIterator first, InputIterator last, T init);
T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation binary_op);
```

### 2. `adjacent_difference` 相邻元素的差额

```C
// *result = *first, 然后计算相邻元素的差，并把它输出到对应的 result + i 中
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result);
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation binary_op);
```

### 3. `inner_product` 内积

```C
// 针对 [first1, last1) 中的每个迭代器 i, init = init + (*i) * (*(first2 + (i - first1)))
T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init);
T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init, BinaryOperation binary_op);
```

### 4. `partial_sum` 局部求和

```C
// *result = *first, 然后计算相邻元素的和，并把它输出到对应的 result + i 中
OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result);
OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation binary_op);
```

### 5. `power`* 幂次方

```C
inline T power(T x, Integer n);
// 幂次方的实现算法还是有点绕的，敲下来参考一下
T power(T x, Integer n, MonoidOperation op)
{
    if(n == 0)
        return identity_element(op); // 取出“证同元素”
    else
    {
        while ((n & 1) == 0) // n % 2 == 0
        {
            n >>= 1; // n /= 2
            x = op(x, x); // 类似于 x^(yz) = (x^y)^z, 这里 y=2, z=n
        }
        
        T result = x; // 经过前面的计算，这里一定有 (n & 1) != 0，默认 n == 1
        n >>= 1; 
        while (n != 0) // true, 证明前面的 n >= 3
        {
            x = op(x, x);
            if((n & 1) != 0) // n 如果为奇数，先去掉一个 x, 剩下偶数个n可以继续 x = op(x, x)，或者最后 n == 1, 必须op最后的结果
                result = op(result, x); 
            n >>= 1;
        }
        
        return result;
    }
}
```

### 6. `iota`* 在区间内填入指定值的递增序列

```C
void iota(ForwardIterator first, ForwardIterator last, T value)
{
    while (first != last) *first++ = value++;
}
```

## #include \<algorithm>

### 1. `equal` 判断两个区间是否相等

```C
// 注意，如果第二序列的元素比较多，多出来的元素不予考虑，所以最好先判断元素个数是否相同
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2);
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate binary_pred);
```

### 2. `fill` 改填元素值

```C
void fill(ForwardIterator first, ForwardIterator last, const T& value);
```

### 3. `fill_n` 改填元素值 n 次

```C
OutputIterator fill_n(OutputIterator first, Size n, const T& value);
```

### 4. `iter_swap` 将两个 ForwardIterator 所指对象对调

```C
inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b);
```

### 5. `lexicographical_compare` 以字典顺序进行比较

```C
// 默认的比较函数是 less<>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2);
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp);
```

### 6. `max` 最大值

```C
inline const T& max(const T& a, const T& b);
inline const T& max(const T& a, const T& b, Compare comp);
```

### 7. `min` 最小值

```C
inline const T& min(const T& a, const T& b);
inline const T& min(const T& a, const T& b, Compare comp);
```

### 8. `mismatch` 找出不匹配点

```C
// 返回一对迭代器，分别指向两序列的不匹配点。如果第二序列元素个数比第一序列少，行为不确定
pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2);
pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate binary_pred);
```

### 9. `swap` 交换

```C
inline void iter_swap(T& a, T& b);
```

### 10. `copy` 复制

```C
OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result);
```

### 11. `copy_backward` 逆向复制

```C
BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result);
```

### ====== set 相关算法

### 1. `set_union` 并集 S1 U S2

```C
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result);
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp);
```

### 2. `set_intersection` 交集 S1 n S2

```C
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result);
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp);
```

### 3. `set_difference` 差集 S1 - S2

```C
// 出现在 S1 但不出现于 S2 
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result);
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp);
```

### 4. `set_symmetric_difference` 对称差集 (S1 - S2) U (S2 - S1)

```C
// "出现在 S1 但不出现于 S2 "以及"出现在 S2 但不出现于 S1 "
OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result);
OutputIterator set_symmetric_difference(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp);
```

## =====数据处理

### 1. `adjacent_find` 查找第一组相邻而重复（或符合条件）的元素

```C
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last);
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last, BinaryPredicate binary_pred);
```

### 2. `count`/`count_if` 计数

```C
typename iterator_traits<InputIterator>::difference_type count(InputIterator first, InputIterator last, const T& value);
typename iterator_traits<InputIterator>::difference_type count_if(InputIterator first, InputIterator last, Predicate pred);
// 以下是旧版
void count(InputIterator first, InputIterator last, const T& value, Size& n);
void count_if(InputIterator first, InputIterator last, Predicate pred, Size& n);
```

### 3. `find`/`find_if` 循序查找

```C
InputIterator find(InputIterator first, InputIterator last, const T& value);
InputIterator find_if(InputIterator first, InputIterator last, Predicate pred);
```

### 4. `find_end` 查找某个子序列最后一个出现点

```C
// 这个是需要整个序列匹配
ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2);
ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2, BinaryPredicate binary_pred);
```

### 5. `find_first_of` 查找某些元素的首次出现点

```C
// 这个是序列中某个元素匹配即可
InputIterator find_first_of(InputIterator first1, InputIterator last1, ForwardIterator first2, ForwardIterator last2);
InputIterator find_first_of(InputIterator first1, InputIterator last1, ForwardIterator first2, ForwardIterator last2, BinaryPredicate binary_pred);
```

### 6. `for_each` 对区间内的每一个元素施行某操作

```C
// 这个函数不能改变元素内容，如需要修改元素应用 transform()
Function for_each(InputIterator first, InputIterator last, Function f);
```

### 7. `generate`/`generate_n` 以特定操作之运算结果填充特定区间内的元素

```C
void generate(ForwardIterator first, ForwardIterator last, Generator gen);
OutputIterator generate_n(OutputIterator first, Size n, Generator gen);
```

### 8. `includes` 是否涵盖于某序列中（应用于有序区间）

```C
// 元素不必连续
bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2);
bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp);
```

### 9. `max_element`/`min_element` 最大值/最小值所在位置

```C
ForwardIterator max_element(ForwardIterator first, ForwardIterator last);
ForwardIterator max_element(ForwardIterator first, ForwardIterator last, Compare comp);
ForwardIterator min_element(ForwardIterator first, ForwardIterator last);
ForwardIterator min_element(ForwardIterator first, ForwardIterator last, Compare comp);
```

### 10. `merge` 合并两个序列（应用于有序区间）

```C
OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result);
OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp);
```

### 11. `partition` 分割

```c
// 所有被一元条件运算 pred 判定为 true 的元素，都会被放在区间的前段
// 被判定为 false 都会被放在区间的后段
BidirectionalIterator partition(BidirectionalIterator first, BidirectionalIterator last, Predicate pred);
```

### 12. `remove`/`remove_if` 移除某类元素（但不删除）

```c
ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T& value);
ForwardIterator remove_if(ForwardIterator first, ForwardIterator last, Predicate pred);
```

### 13. `remove_copy`/`remove_copy_if` 移除某类元素并将结果复制到另一容器

```c
OutputIterator remove_copy(InputIterator first, InputIterator last, OutputIterator result, const T& value);
OutputIterator remove_copy_if(InputIterator first, InputIterator last, OutputIterator result, Predicate pred);
```

### 14. `replace`/`replace_if` 替换某类元素

```c
void replace(ForwardIterator first, ForwardIterator last, const T& old_value, const T& new_value);
void replace_if(ForwardIterator first, ForwardIterator last, Predicate pred, const T& new_value);
```

### 15. `replace_copy`/`replace_copy_if` 替换某类元素并将结果复制到另一容器

```c
void replace_copy(InputIterator first, InputIterator last, OutputIterator result, const T& old_value, const T& new_value);
void replace_copy_if(InputIterator first, InputIterator last, OutputIterator result, Predicate pred, const T& new_value);
```

### 16. `reverse`/`reverse_copy` 反转元素次序（并将结果复制到另一容器）

```c
void reverse(BidirectionalIterator first, BidirectionalIterator last);
void reverse_copy(BidirectionalIterator first, BidirectionalIterator last, OutputIterator result);
```

### 17. `rotate`/`rotate_copy` 旋转（并将结果复制到另一容器）

```c
// 将 [first, middle) 内的元素和 [middle, last) 内的元素互换
void rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last);
void rotate_copy(ForwardIterator first, ForwardIterator middle, ForwardIterator last, OutputIterator result);
```

### 18. `search`/`search_n` 查找某个子序列（必须完全一致）

```C
ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2);
ForwardIterator search_n(ForwardIterator first, ForwardIterator last, Integer n, const T& value);
// 连续 count 个元素皆满足指定条件
ForwardIterator search_n(ForwardIterator first, ForwardIterator last, Integer count, const T& value, BinaryPredicate binary_pred);
```

### 19. `swap_range` 交换指定区间

```C
// 如果第二序列元素个数比第一序列少，行为不确定
ForwardIterator2 swap_range(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2);
```

### 20. `transform` 以两个序列为基础，交互作用产生第三个序列

```c
OutputIterator transform(InputIterator first, InputIterator last, OutputIterator result, UnaryOperation op);
OutputIterator transform(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, BinaryOperation binary_op);
```

### 21. `unique`/`unique_copy` 去掉重复的元素

```c
// 注意，重复的元素会被移到最后，总的元素个数并没有改变，如要删除需要调用erase
ForwardIterator unique(ForwardIterator first, ForwardIterator last);
ForwardIterator unique(ForwardIterator first, ForwardIterator last, BinaryPredicate binary_pred);
OutputIterator unique_copy(ForwardIterator first, ForwardIterator last, OutputIterator result);
OutputIterator unique_copy(ForwardIterator first, ForwardIterator last, OutputIterator result, BinaryPredicate binary_pred);
```

## 实现复杂一点的算法

### 1. `lower_bound` 查找元素(应用于有序区间)

```c
// 在已排序的[first, last)中寻找value，如果相等，便返回第一元素的迭代器，否则，返回"假如该元素存在时应该出现的位置"
ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value);
ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value, Compare comp);
```

### 2. `upper_bound` 查找元素(应用于有序区间)

```c
// 在已排序的[first, last)中寻找value，返回"可插入 value 的最后一个合适的位置"，或者说 value 值的下一个位置
ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& value);
ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& value, Compare comp);
```

### 3. `binary_search` 二分查找(应用于有序区间)

```c
bool binary_search(ForwardIterator first, ForwardIterator last, const T& value);
bool binary_search(ForwardIterator first, ForwardIterator last, const T& value, Compare comp);
```

### 4. `next_permutation` 获得下一个排列组合

```c
// 实现：
// 从最尾端开始往前寻找两个相邻元素，令第一元素为 *i, 第二元素为 *ii, 且满足 *i < *ii, 找到这样一组元素后，
// 再从尾端开始往前检验，找出第一个大于 *i 的元素，令为 *j, 将 i,j 元素对调，再将 ii 之后的所有元素颠倒排列
bool next_permutation(BidirectionalIterator first, BidirectionalIterator last);
bool next_permutation(BidirectionalIterator first, BidirectionalIterator last, Compare comp);
```

### 5. `prev_permutation` 获得前一个排列组合

```c
// 实现：
// 从最尾端开始往前寻找两个相邻元素，令第一元素为 *i, 第二元素为 *ii, 且满足 *i > *ii, 找到这样一组元素后，
// 再从尾端开始往前检验，找出第一个小于 *i 的元素，令为 *j, 将 i,j 元素对调，再将 ii 之后的所有元素颠倒排列
bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last);
bool prev_permutation(BidirectionalIterator first, BidirectionalIterator last, Compare comp);
```

### 6. `random_shuffle` 随机重排元素

```c
void random_shufle(RandomAccessIterator first, RandomAccessIterator last);
void random_shufle(RandomAccessIterator first, RandomAccessIterator last, RandomNumberGenerator& rand);
```

### 7. `partial_sort`/`partial_sort_copy` 局部排序(并复制结果到另一序列)

```c
// [first,middle) 内的元素以递增顺序排序，[middle,last) 内元素不确定
void partial_sort(RandomAccessIterator first, RandomAccessIterator middle,  RandomAccessIterator last);
void partial_sort(RandomAccessIterator first, RandomAccessIterator middle,  RandomAccessIterator last, Compare comp);
void partial_sort_copy(InputIterator first, InputIterator last,  RandomAccessIterator result_first, RandomAccessIterator result_last);
void partial_sort_copy(InputIterator first, InputIterator last,  RandomAccessIterator result_first, RandomAccessIterator result_last, Compare comp);
```

### 8. `sort` 排序

```c
void sort(RandomAccessIterator first, RandomAccessIterator last);
void sort(RandomAccessIterator first, RandomAccessIterator last, Compare comp);
```

### 9. `equal_range` 试图在有序区间中寻找某值（返回一个上下限区间）

```c
// 返回值区间即为 pair<lower_bound, upper_bound>
pair<ForwardIterator, ForwardIterator> equal_range(ForwardIterator first, ForwardIterator last, const T& value);
pair<ForwardIterator, ForwardIterator> equal_range(ForwardIterator first, ForwardIterator last, const T& value, Compare comp);
```

### 10. `inplace_merge` 合并并就地替换（覆写上去）

```c
// [first, middle) 和 [middle, last) 都已排序，合并成一个有序序列 [first, last)
void inplace_merge(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last);
void inplace_merge(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last, Compare comp);
```

### 11. `nth_element` 重新安排序列中的第 n 个元素的左右两端

```c
void nth_element(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last);
void nth_element(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last, Compare comp);
```

### 12. `mergesort` 排序

```c
void mergesort(BidirectionalIter first, BidirectionalIter last);
```
