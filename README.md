# BlurApp-Qt

BlurApp Qt
Based on Opencv

There are two classes which can zoom in or zoom out images and cut the image.

头一次写了一个类。ImageWidget是从网上（抱歉！忘了作者和出处。）找到的一个显示图片的继承QtWidget类，有放大、拖动和另存为的功能。我就想给他加个截图的功能，写成了一个selectrect类。也是继承Widget类，作为一个半透明蒙板返回选取矩形的左上坐标和长宽，在ImageWidget中进行截取图片。要实现全功能需要两个类同时使用。应该可以合成一个类

2017.11.11:

improved selectrect class & ImageWidget class, removed useless parameters.

Improved interaction logic.

改进了selectrect类和ImageWidget类，去除不必要参数。在ImageWidget对象中单击另存为时会自己创建selectrect子对象，不用在外部用信号槽手动链接两个类。

改进交互逻辑。

2017.11.17

fixed zoom in & zoom out limitation, this function needs huge computer memories when the scale is big, so the max scale is fixed to 12.

修正放大缩小限制，此功能当放大倍数过大时会占用很大内存，故将最大放大倍数限制为12

2017.11.29

fixed a pointer bug

修复了当建立ImageWidget子对象时，父对象销毁后mp_img指针被多次delete的问题

2017.12.03

updated ImageWidget

更新了ImageWidget控件

2017.12.12

updated ImageWidget, recoded load image function, you can set image by the pointer(not copy the memory) or the data(copy the memory), and added other small options.

更新了ImageWidget,重写了载入图像函数，可以用指针传入（不复制内存）和数据传入（复制内存），添加了载入图像时的一些额外选项。
