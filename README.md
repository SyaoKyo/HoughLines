# HoughLines

​	使用OpenCV的以下函数 [HoughLines](http://opencv.willowgarage.com/documentation/cpp/imgproc_feature_detection.html?#cv-houghlines) 和 [HoughLinesP](http://opencv.willowgarage.com/documentation/cpp/imgproc_feature_detection.html?#cv-houghlinesp) 来检测图像中的直线。

## 原理

### 霍夫曼变线

1. 霍夫线变换是一种用来寻找直线的方法.
2. 是用霍夫线变换之前, 首先要对图像进行边缘检测的处理，也即霍夫线变换的直接输入只能是边缘二值图像.



#### 如何实现

1. 众所周知, 一条直线在图像二维空间可由两个变量表示. 例如:

   1. 在 **笛卡尔坐标系:** 可由参数: ![(m,b)](./pic/1881b126c9a42331f05b6b61e650a11fba03e74d.png) 斜率和截距表示.
   2. 在 **极坐标系:** 可由参数: ![(r,\theta)](./pic/f07080129914ac008f0eb45ed5f7efa28bb1e7c6.png) 极径和极角表示

   
   <div align=center>
      <img src="./pic/Hough_Lines_Tutorial_Theory_0.jpg" alt="Line variables">
   </div>
   

   对于霍夫变换, 我们将用 *极坐标系* 来表示直线. 因此, 直线的表达式可为:

   <div align=center>
      <img src="./pic/3050cf05bd6f4d9b2623357fb1cc399905d75943.png" alt="y = \left ( -\dfrac{\cos \theta}{\sin \theta} \right ) x + \left ( \dfrac{r}{\sin \theta} \right )">
   </div>

   化简得: ![r = x \cos \theta + y \sin \theta](./pic/0fcd77fdfa1c3bb6a179aa0d02ea617e870ec3c4.png)

2. 一般来说对于点 ![(x_{0}, y_{0})](./pic/25e3648ce4903aa24b129162eeaec2330fed4645.png), 我们可以将通过这个点的一族直线统一定义为:

   <div align=center>
      <img src="./pic/3af8e1a6da7bbaddb157c6371676051128164089.png" alt="r_{\theta} = x_{0} \cdot \cos \theta  + y_{0} \cdot \sin \theta">
   </div>

   这就意味着每一对 ![(r_{\theta},\theta)](./pic/79ac4156f482a4985cfc0438daa976270cc86d83.png) 代表一条通过点 ![(x_{0}, y_{0})](./pic/25e3648ce4903aa24b129162eeaec2330fed4645.png) 的直线.

3. 如果对于一个给定点 ![(x_{0}, y_{0})](./pic/25e3648ce4903aa24b129162eeaec2330fed4645.png) 我们在极坐标对极径极角平面绘出所有通过它的直线, 将得到一条正弦曲线. 例如, 对于给定点 ![x_{0} = 8](./pic/2cf5e45d33850ec55abfe091b5d189c439a99832.png) and ![y_{0} = 6](./pic/b5614ac3d2973e0d94e4bcac101d80c7dacf01eb.png) 我们可以绘出下图 (在平面 ![\theta](./pic/52e8ed7a3ba22130ad3984eb2cd413406475a689.png) - ![r](./pic/b55ca7a0aa88ab7d58f4fc035317fdac39b17861.png)):

   
   <div align=center>
      <img src="./pic/Hough_Lines_Tutorial_Theory_1.jpg" alt="Polar plot of a the family of lines of a point">
   </div>

   只绘出满足下列条件的点 ![r > 0](./pic/22ff7db3e755a8164f2344e0d3d84ea3a0fe93b3.png) and ![0< \theta < 2 \pi](./pic/99a1c7ae0e28e6ab32a11edc1d4d9be3da55c91e.png).

4. 我们可以对图像中所有的点进行上述操作. 如果两个不同点进行上述操作后得到的曲线在平面 ![\theta](./pic/52e8ed7a3ba22130ad3984eb2cd413406475a689.png) - ![r](./pic/b55ca7a0aa88ab7d58f4fc035317fdac39b17861.png) 相交, 这就意味着它们通过同一条直线. 例如, 接上面的例子我们继续对点: ![x_{1} = 9](./pic/d5e243dd84f48f005022ca6db06ebbd5df8e14ca.png), ![y_{1} = 4](./pic/0fd43a421096cc99759477319d149c08aac343fd.png) 和点 ![x_{2} = 12](./pic/a1256f1b04a9895e2f28f40f08eb5eed5f670f27.png), ![y_{2} = 3](./pic/f09a7f1a339c6cd9ccd3bc1b8d8bf1b79a257910.png) 绘图, 得到下图:

   
   <div align=center>
      <img src="./pic/Hough_Lines_Tutorial_Theory_2.jpg" alt="Polar plot of the family of lines for three points">
   </div>

   这三条曲线在 ![\theta](./pic/52e8ed7a3ba22130ad3984eb2cd413406475a689.png) - ![r](./pic/b55ca7a0aa88ab7d58f4fc035317fdac39b17861.png) 平面相交于点 ![(0.925, 9.6)](./pic/ee5ec5d602f8544803b6e1164d66bf0a434691dd.png), 坐标表示的是参数对 (![\theta, r](./pic/41c5151da6eb0499332d0b45df78e63788aa845e.png)) 或者是说点 ![(x_{0}, y_{0})](./pic/25e3648ce4903aa24b129162eeaec2330fed4645.png), 点 ![(x_{1}, y_{1})](./pic/76859fcc37c8dd5e7f3f5b2a070a0c269a285690.png) 和点 ![(x_{2}, y_{2})](./pic/f806732d21b52d738268efd8c8de5e8c1c648e86.png) 组成的平面内的的直线.

5. 那么以上的材料要说明什么呢? 这意味着一般来说, 一条直线能够通过在平面 ![\theta](./pic/52e8ed7a3ba22130ad3984eb2cd413406475a689.png) - ![r](./pic/b55ca7a0aa88ab7d58f4fc035317fdac39b17861.png) 寻找交于一点的曲线数量来 *检测*. 越多曲线交于一点也就意味着这个交点表示的直线由更多的点组成. 一般来说我们可以通过设置直线上点的 *阈值* 来定义多少条曲线交于一点我们才认为 *检测* 到了一条直线.

6. 这就是霍夫线变换要做的. 它追踪图像中每个点对应曲线间的交点. 如果交于一点的曲线的数量超过了 *阈值*, 那么可以认为这个交点所代表的参数对 ![(\theta, r_{\theta})](./pic/7e065ce45efb160a92773b6c4aad65a309d7535e.png) 在原图像中为一条直线.



#### 标准霍夫线变换和统计概率霍夫线变换

OpenCV实现了以下两种霍夫线变换:

1. **标准霍夫线变换**

   - 原理在上面的部分已经说明了. 它能给我们提供一组参数对 ![(\theta, r_{\theta})](./pic/7e065ce45efb160a92773b6c4aad65a309d7535e.png) 的集合来表示检测到的直线
   - 在OpenCV 中通过函数 [HoughLines](http://opencv.willowgarage.com/documentation/cpp/imgproc_feature_detection.html?#cv-houghlines) 来实现

2. **统计概率霍夫线变换**

   - 这是执行起来效率更高的霍夫线变换. 它输出检测到的直线的端点 ![(x_{0}, y_{0}, x_{1}, y_{1})](./pic/20eb30037d99342a5887bea2a086e1b39e78904a.png)
   - 在OpenCV 中它通过函数 [HoughLinesP](http://opencv.willowgarage.com/documentation/cpp/imgproc_feature_detection.html?#cv-houghlinesp) 来实现



## 代码解释

1. 首先申明变量：

   ```c++
   // 全局变量
   Mat src;
   Mat dst, detected_edges, cdst;
   //Canny参数
   int lowThreshold_Canny = 20;
   int const max_lowThreshold_Canny = 150;
   int ratio = 3;
   int kernel_size = 3;
   int edgeThresh = 1;
   //Hough参数
   int lowThreshold_Hough = 100;
   int const max_lowThreshold_Hough = 200;
   
   char* window_name = "Edge Map";
   bool flag = false;// true:HoughLines  false:HoughLinesP
   ```

2. 加载图片 ：

   ```c++
   // 装载图像
   src = imread("house.jpg");
   if (!src.data)
   {
   	return -1;
   }
   ```

3. 创建显示窗口：

```c++
   // 创建显示窗口
   namedWindow(window_name, CV_WINDOW_AUTOSIZE);
```

4. 创建trackbar，获取阀值：

   ```c++
   // 创建trackbar
   createTrackbar("Canny Threshold", window_name, 
                     &lowThreshold_Canny, max_lowThreshold_Canny, Operations);
   createTrackbar("Hough Threshold", window_name, 
                     &lowThreshold_Hough, max_lowThreshold_Hough, Operations);
   ```

5. Canny处理+灰度处理：

   ```c++
   //Canny处理&灰度处理
   Canny(src, dst, lowThreshold_Canny, lowThreshold_Canny*ratio, kernel_size);
   cvtColor(dst, cdst, CV_GRAY2BGR);
   ```


6. HoughLines（或HoughLinesP）画线：

   ```c++
   if (flag)
   {
   	vector<Vec2f> lines;
   	if (lowThreshold_Hough == 0)
   		HoughLines(dst, lines, 1, CV_PI / 180, 1, 0, 0);
   	else
   		HoughLines(dst, lines, 1, CV_PI / 180, lowThreshold_Hough, 0, 0);
       
   	for (size_t i = 0; i < lines.size(); i++)
   	{
   		float rho = lines[i][0], theta = lines[i][1];
   		Point pt1, pt2;
   		double a = cos(theta), b = sin(theta);
   		double x0 = a*rho, y0 = b*rho;
   		pt1.x = cvRound(x0 + 1000 * (-b));
   		pt1.y = cvRound(y0 + 1000 * (a));
   		pt2.x = cvRound(x0 - 1000 * (-b));
   		pt2.y = cvRound(y0 - 1000 * (a));
   		line(cdst, pt1, pt2, Scalar(0, 0, 255), 1, CV_AA);
   	}
   }
   else
   {
   	vector<Vec4i> lines;
   	if (lowThreshold_Hough == 0)
   		HoughLinesP(dst, lines, 1, CV_PI / 180, 1, 1, 10);
   	else
   		HoughLinesP(dst, lines, 1, CV_PI / 180, 
                       lowThreshold_Hough, lowThreshold_Hough, 10);
   
   	for (size_t i = 0; i < lines.size(); i++)
   	{
   		Vec4i l = lines[i];
   		line(cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 1, CV_AA);
   	}
   }
   ```


7. 显示图像：

   ```c++
   imshow(window_name, cdst);
   ```

   