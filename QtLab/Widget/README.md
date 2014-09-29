#关于Widget的探究

##Widget的几种边框.

- frameGeometry: 包含了其父亲(window frame)的边框.
- geometry: 可以理解为当前widget真正的边框.
- rect: == (0, 0, width(), height()); 注意与geometry的区别, 它是从左上角开始的边框.
```cpp
geometry().width() == width();
geometry().height() == height();
```
其示意图如下图所示:

![image](https://cloud.githubusercontent.com/assets/1147451/4418117/01f0ba80-455c-11e4-87f2-13fed4328e77.png)

- contentRect: 边缘内部的边框, 在geometry的内部.
- normalGeometry: 这个指的是当前widget在正常情况下(**没有最大化或全屏**)的边框.
- childrenRect: 获取**子widget**的边框, 如果没有孩子, 长宽皆为0.

see the debug out at [here](../../../907969c2aac80c1d4a71cedaf59ef3c49e879530/QtLab/Widget/widget.cpp#L16-L41)
