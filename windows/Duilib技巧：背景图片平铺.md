# Duilib技巧：背景图片平铺

贴图的描述方式有两种，其实看  

1. aaa.jpg
2. file='aaa.jpg' res='' restype='0' dest='0,0,0,0' source='0,0,0,0' corner='0,0,0,0' mask='#FF0000' fade='255' hole='false' xtiled='false' ytiled='false'

第一种是最简单的，加载图片文件并将整张图片拉伸铺满整个控件

第二种既支持从文件中加载图片，也可以从资源中加载

1、如果是从文件加载，设置file属性，如file='XXX.png'，不要写res和restype属性

2、如果从资源加载，设置res和restype属性，不要设置file属性

3、dest属性的作用是指定图片绘制在控件的一部分上面（绘制目标位置）

4、source属性的作用是指定使用图片的一部分

5、corner属性是指图片安装scale9方式绘制（scale9是什么意思，自行google，一般flash设计用的比较多）

6、mask属性是给不支持alpha通道的图片格式（如bmp）指定透明色

7、fade属性是设置图片绘制的透明度

8、hole属性是指定scale9绘制时要不要绘制中间部分（有些地方对提升性能比较有用）

9、xtiled属性设置成true就是指定图片在x轴不要拉伸而是平铺，ytiled属性设置成true就是指定图片在y轴不要拉伸而是平铺

让背景图片平铺要写：xtiled='true' ytiled='true'

这个UI设计器是不识别的.
