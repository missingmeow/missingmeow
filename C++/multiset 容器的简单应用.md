# multiset 容器的简单应用

1. find() 返回和参数匹配的第一个元素的迭代器，如果都不匹配，则返回容器的结束迭代器。
2. equal_range() 返回一个包含迭代器的pair对象，它定义了一个和参数匹配的元素段。如果没有元素匹配的话，pair的第一个成员是容器的结束迭代器；在这种情况下，第二个成员是比参数大的第一个元素，如果都没有的话，它也是容器的结束迭代器。
3. lower_bound() 返回和参数匹配的第一个元素的迭代器，如果没有匹配的元素，会返回容器的结束迭代器。返回的迭代器和range()返回的pair的第一个成员相同。
4. upper_bound() 返回的迭代器和equal_range()返回的pair的第二个成员相同。
5. count() 返回和参数匹配的元素的个数。

```c++
int main()
{
  string str;
  getline(std::cin, str, '*'); // 读取任意段数据，直到输入 * 号结束输入
  std::replace_if(str.begin(), str.end(), [](const char& ch) { return !isalpha(ch); }, ' '); // 把所有不是字母的字符替换成空格

  multiset<string> setString;

  std::istringstream text(str); // 把一张段数据分解成单个单词
  std::istream_iterator<string> begin(text);
  std::istream_iterator<string> end;
  for_each(begin, end, [&setString](const string& word){ setString.emplace(word); }); // 把每个单词放进 set 容器中

  int count = 0;
  for (auto iter = setString.begin(); iter != setString.end(); iter = setString.upper_bound(*iter)) // 输出每个单词的个数
  {
    std::cout << std::left << setw(10) << *iter << std::right << setw(3) << setString.count(*iter) << " ";
    if (++count % 6 == 0) 
        cout << endl;
  }

    // 查找 apple 字符串并遍历的两种做法，multimap 查找相同的键用得比较多，也是同样的做法
    auto pr = setString.equal_range("apple");
    if(pr.first != setString.end())
    {
        for(auto iter = pr.first; iter != pr.second; ++iter)
            ;
    }
    
    auto iter1 = setString.lower_bound("apple");
    auto iter2 = setString.upper_bound("apple");
    if(iter1 != setString.end())
    {
        for(auto iter = iter1; iter != iter2; ++iter)
            ;
    }

    system("pause");
    return 0;
}
```
