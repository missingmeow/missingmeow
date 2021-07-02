# SQL 语句

## 查询

select * from Table
where col=val or col is null
group by col1
having col1=val1
order by id

## 插入

insert into Table (column) values (val); select @@identify; // 返回插入行的主键id值

## 更新

update Table set col1=val1 where col2=val2

## 删除

delete from Table where col=val

## 条件判断与去掉空字符

select RTRIM(case when val1 is null then val2 else val1 end) from Table
// 分页逻辑
select count(distinct id) as count from Table where col=val
select * from Table where id in (
  select top page_size id from (
    select top page * page_size from Table where col=val order by id desc
  )
)
order by id asc

## 字符串修改

select stuff(
  '待修改字符串',
  1, // 起始位置
  1, // 修改长度
  '' // 替换成这个值
) value

## 类型转换

select cast(c.url+p.url as nvarchar(255)) as url from web c, web2 p

## with，父子节点的问题

with cte as (
  select id, url from web where pid is null
  union all
  select p.id,cast(c.url + p.url as nvarchar(255)) from cte c, web p where p.pid=c.id
)
select * from cte

