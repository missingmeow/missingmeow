# javascript基础汇总

1.相等运算符
==，它会自动转换数据类型再比较，很多时候，会得到非常诡异的结果；
===，它不会自动转换数据类型，如果数据类型不一致，返回false，如果一致，再比较。
JavaScript的设计缺陷，不要使用==比较，始终坚持使用===比较。

2.字符串String
ES6标准新增了一种多行字符串的表示方法，用反引号 ` ... ` 表示，可多行，支持模板输出alert(`你好, ${name}, 你今年${age}岁了!`);
.toUpperCase()把一个字符串全部变为大写
.toLowerCase()把一个字符串全部变为小写
.indexOf()会搜索指定字符串出现的位置
.substring()返回指定索引区间的子串

3.数组Array
.indexOf()搜索一个指定的元素的位置
.slice()截取Array的部分元素，然后返回一个新的Array，不传参当复制数组用
.push()向Array的末尾添加若干元素
.pop()则把Array的最后一个元素删除掉
.unshift()向Array的头部添加若干元素
.shift()则把Array的第一个元素删除掉
.sort()对当前Array进行排序，直接修改当前Array的元素位置，直接调用时，按照默认顺序排序
.reverse()把整个Array的元素给掉个个，也就是反转
.splice()指定的索引开始删除若干元素，然后再从该位置添加若干元素
.concat()把当前的Array和另一个Array连接起来，并返回一个新的Array
.join()把当前Array的每个元素都用指定的字符串连接起来，然后返回连接后的字符串

4.对象
要判断一个属性是否是对象自身拥有的，而不是继承得到的，可以用hasOwnProperty()方法

5.条件判断
JavaScript把null、undefined、0、NaN和空字符串''视为false，其他值一概视为true

6.函数
JavaScript有一个免费赠送的关键字arguments，它只在函数内部起作用，并且永远指向当前函数的调用者传入的所有参数。arguments类似Array但它不是一个Array。

7.循环
for ... in循环由于历史遗留问题，它遍历的实际上是对象的属性名称。
for ... of循环则完全修复了这些问题，它只循环集合本身的元素。

不要使用new Number()、new Boolean()、new String()创建包装对象；
用parseInt()或parseFloat()来转换任意类型到number；
用String()来转换任意类型到string，或者直接调用某个对象的toString()方法；
通常不必把任意类型转换为boolean再判断，因为可以直接写if (myVar) {...}；
typeof操作符可以判断出number、boolean、string、function和undefined；
判断Array要使用Array.isArray(arr)；
判断null请使用myVar === null；
判断某个全局变量是否存在用typeof window.myVar === 'undefined'；
函数内部判断某个变量是否存在用typeof myVar === 'undefined'。
