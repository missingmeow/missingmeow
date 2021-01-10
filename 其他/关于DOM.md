# 关于DOM

HTML文档被浏览器解析后就是一棵DOM树，要改变HTML的结构，就需要通过JavaScript来操作DOM。

获取DOM节点
document.getElementById()以直接定位唯一的一个DOM节点
document.getElementsByTagName()返回一组DOM节点
以及CSS选择器document.getElementsByClassName()返回一组DOM节点，要精确地选择DOM，可以先定位父节点，再从父节点开始选择，以缩小范围。
document.querySelector()/document.querySelectorAll()，需要了解selector语法，然后使用条件来获取节点，更加方便。

更新DOM节点，获取DOM节点后
.innerHTML
.innerText 不返回隐藏元素的文本
.textContent 返回所有文本
另外DOM节点的style属性对应所有的CSS，可以直接获取或设置，如：.style.fontSize = '20px';

插入DOM
.appendChild，把一个子节点添加到父节点的最后一个子节点。
parentElement.insertBefore(newElement, referenceElement);，子节点会插入到referenceElement之前

删除DOM
要删除一个节点，首先要获得该节点本身以及它的父节点，然后，调用父节点的removeChild把自己删掉
