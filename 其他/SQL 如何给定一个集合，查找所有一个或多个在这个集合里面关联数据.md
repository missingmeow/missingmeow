# SQL 如何给定一个集合，查找所有一个或多个在这个集合里面关联数据

当前遇到这么一种情况，我有一个基础的表，记录一些基本类型，然后定义了一些权限，每个权限都关联了一个或多个基础表里的元素，然后给出一个集合，集合中有若干基础表中的值，要求找出所有的权限，找出的权限所能管理的基础表中的元素，都必须在给定的集合里。

以上情况简单表现为以下例子。假设有个班级表，有数据如下：

class表：

id | name
---|---
1 | Class1
2 | Class2
3 | Class3
4 | Class4

另外，我定义了一些管理角色，这些角色包含班级表中的一个或者多个，表示该角色能管理班级：

role表：

id | name
---|---
1 | Master
2 | Teacher1
3 | Teacher2
4 | Teacher3

角色跟班级的对应关系在另外一个表中：

roleclass关系表：

roleid | classid
---|---
1 | 1
1 | 2
1 | 3
1 | 4
2 | 1
2 | 2
2 | 3
3 | 2
3 | 3
4 | 3
4 | 4

现在，给出一个班级集合，找出所有角色，要求角色关联的classid都必须出现在这个集合中？

比如有个班级集合`{1, 2, 3}`，那么找出的角色ID应该是`{2,3}`；如果班级集合为`{2,3}`，那么角色ID为`{3}`；如果是`{4}`，那么角色ID为空。

目前我的实现方案如下：

1. 统计当前所有角色ID对应的能管理的班级数量

```sql
select id,count(*) count from role,roleclass where id=roleid group by id
as total
```

2. 根据给定的集合，统计roleClass表每个roleid对应的班级数

```sql
select roleid,count(*) count from roleclass where classid in (/*这里是给定集合*/) group by roleid
as sel
```

3. 比较上面两个相同角色ID，它们统计出来的班级数量是否一致

```sql
select id from total,sel where total.id=sel.roleid and total.count=sel.total
```

以上，总结成一个sql语句的话就是：

```sql
select id from 
  (select id,count(*) count from role,roleclass where id=roleid group by id)
as total,
  (select roleid,count(*) count from roleclass where classid in (/*这里是给定集合*/) group by roleid)
as sel 
where total.id=sel.roleid and total.count=sel.total
```

不知道大家还有没有更简单的方法？
