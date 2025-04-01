# LAB: Grayscale Image Segmentation -Gear



## LAB: Grayscale Image Segmentation



**Date:** 2025-3-31

**Author:** SUN-WOO KIM 22000090

**Github:** 



## **Introduction**

### 1. Objective

**Goal**: Defective Gear Inspection System

 Plastic gears are used in various fields, including toys, RC cars, and plastic-based hardware. Since they are made of plastic, the teeth of the gears can easily break, which requires a machine vision system to detect defects in advance. The goal is to apply thresholding and morphological operations, find contours, and identify and extract the target objects from the background.



### 2. Preparation

**Software Installation**

- OpenCV 4.9.0
- Visual Studio 2022

**Dataset**

- 4 images of damaged and undamaged gears

| ![Gear1.jpg](https://github.com/ssunwookim/DLIP/blob/main/Image/LAB%20:%20Grayscale%20Image%20Segmentation-Gear/Gear1.jpg?raw=true) | ![Gear2.jpg](https://github.com/ssunwookim/DLIP/blob/main/Image/LAB%20:%20Grayscale%20Image%20Segmentation-Gear/Gear2.jpg?raw=true) |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| ![Gear3.jpg](https://github.com/ssunwookim/DLIP/blob/main/Image/LAB%20:%20Grayscale%20Image%20Segmentation-Gear/Gear3.jpg?raw=true) | ![Gear4.jpg](https://github.com/ssunwookim/DLIP/blob/main/Image/LAB%20:%20Grayscale%20Image%20Segmentation-Gear/Gear4.jpg?raw=true) |



## Algorithm



### 1.Overview

![image-20250401091744083](C:\Users\ksw86\AppData\Roaming\Typora\typora-user-images\image-20250401091744083.png)

**Flow chart**

1. Input Gear Sample Image
2. Use the findContours() function to find the gear contour and the minEnclosingCircle() function to calculate the enclosing circle of the gear.
3. Find the root diameter, which is 80% of the diameter of the enclosing circle, and mask the gear with a circle of this diameter using the circle() and subtract() functions.
4. Use the findContours() function again to draw the contours of the gear.
5. Use the contourArea() function to calculate the area of the gear's contour, and use the sort() function to find the median area of the gears, setting min and max values within a 20% error rate.
6. Highlight the gears that deviate from the error rate in red to indicate their quality.





## Result and Discussion



### 1. Final Result

Sample gear #1

![image-20250401084559257](C:\Users\ksw86\AppData\Roaming\Typora\typora-user-images\image-20250401084559257.png)

![image-20250401084738936](C:\Users\ksw86\AppData\Roaming\Typora\typora-user-images\image-20250401084738936.png)

Sample gear #2![image-20250401084435646](C:\Users\ksw86\AppData\Roaming\Typora\typora-user-images\image-20250401084435646.png)

![image-20250401084703948](C:\Users\ksw86\AppData\Roaming\Typora\typora-user-images\image-20250401084703948.png)

Sample gear #3

![image-20250401084340005](C:\Users\ksw86\AppData\Roaming\Typora\typora-user-images\image-20250401084340005.png)

![image-20250401084629881](C:\Users\ksw86\AppData\Roaming\Typora\typora-user-images\image-20250401084629881.png)

Sample gear #4

<img src="C:\Users\ksw86\AppData\Roaming\Typora\typora-user-images\image-20250401084132334.png" alt="image-20250401084132334" style="zoom: 67%;" />

![image-20250401084645783](C:\Users\ksw86\AppData\Roaming\Typora\typora-user-images\image-20250401084645783.png)



### 2. Discussion

|                           | Sample1                                                      | Sample2                                                      | Sample3                                                      | Sample4                                                      |
| ------------------------- | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| Output                    | ![image-20250401085214861](C:\Users\ksw86\AppData\Roaming\Typora\typora-user-images\image-20250401085214861.png) | ![image-20250401085230590](C:\Users\ksw86\AppData\Roaming\Typora\typora-user-images\image-20250401085230590.png) | ![image-20250401085155303](C:\Users\ksw86\AppData\Roaming\Typora\typora-user-images\image-20250401085155303.png) | ![image-20250401085128355](C:\Users\ksw86\AppData\Roaming\Typora\typora-user-images\image-20250401085128355.png) |
| Number of defective teeth | 3                                                            | 0                                                            | 5                                                            | 3                                                            |
| Diameter of the gear      | 169.267                                                      | 169.365                                                      | 187.62                                                       | 188.652                                                      |
| Quality Inspection        | Fail                                                         | Pass                                                         | Fail                                                         | Fail                                                         |

 The goal of this project was to determine the quality of gears through image processing. Using the algorithm described in the flow chart above, we found broken or incorrectly printed gear teeth and accurately separated the quality of the gears through this. In this study, we used an algorithm that separates the outline of gear teeth by setting 80% of the gear circumference diameter as the root diameter and masking the center circle. Therefore, if the gear tooth ratio is constant, it accurately identifies the incorrect parts of the gear teeth as in this project, but if the gear tooth ratio is not constant, this algorithm has the disadvantage that it may not work accurately.



## Conclusion

 In this project, we developed a defect detection system for plastic gears using image segmentation techniques. Using methods such as thresholding, contour detection, and morphological operations, we were able to accurately separate and identify gear defects such as broken teeth from a variety of sample images. The system successfully determined the quality of the gears by comparing the contour area and diameter to the set thresholds and highlighting the gears with defects based on a 20% error margin. 

 However, the performance of the system varied depending on the consistency of the gear tooth ratio. When the gear tooth ratio varied, the algorithm had difficulty accurately identifying defects, suggesting that future improvements could focus on accounting for such variations. 

 Overall, this approach demonstrates the potential of machine vision in quality control processes, especially in plastic gear manufacturing, and provides a cost-effective solution for automated defect detection. Future work could include strengthening the robustness of the algorithm to handle a wider variety of gear designs and improve accuracy in real-world applications.



## Appendix

1. `minEnclosingCircle()`

This function finds the minimum circumcircle of a given contour.

```c++
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace std; 

int main() {

    Mat image = cv::imread("gear.jpg", cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cerr << "Image not found!" << std::endl;
        return -1;
    }

    Mat thresh;
    threshold(image, thresh, 127, 255, cv::THRESH_BINARY);

    vector<std::vector<cv::Point>> contours;
    findContours(thresh, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    for (size_t i = 0; i < contours.size(); i++) {
        Moments moments = cv::moments(contours[i]);
        Point2f center(moments.m10 / moments.m00, moments.m01 / moments.m00);
        float radius;
        minEnclosingCircle(contours[i], center, radius);

        circle(image, center, (int)radius, cv::Scalar(0, 255, 0), 2);
    }

    imshow("Min Enclosing Circle", image);
    waitKey(0);
    return 0;
}

```

2. `circle()`

This function is used to draw a circle on an image.

```c++
#include <opencv2/opencv.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace std; 

int main() {
    
    Mat image = cv::Mat::zeros(500, 500, CV_8UC3);

    
    Point center(250, 250);
    int radius = 100;
    Scalar color(0, 255, 0); 
    int thickness = 2;

    circle(image, center, radius, color, thickness);

    imshow("Circle", image);
    waitKey(0);
    return 0;
}

```

3. `subtract()`

This function is used to subtract two images.

```c++
#include <opencv2/opencv.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace std; 

int main() {
    Mat image1 = cv::Mat::zeros(500, 500, CV_8UC1);
    Mat image2 = cv::Mat::ones(500, 500, CV_8UC1) * 255;

    Mat result;
    subtract(image2, image1, result);

    imshow("Subtracted Image", result);
    waitKey(0);
    return 0;
}

```

4. `contourArea()`

This function calculates the area of a given contour.

```c++
#include <opencv2/opencv.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace std; 

int main() {
    Mat image = cv::imread("gear.jpg", cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cerr << "Image not found!" << std::endl;
        return -1;
    }

    Mat thresh;
    threshold(image, thresh, 127, 255, cv::THRESH_BINARY);

    vector<std::vector<cv::Point>> contours;
    findContours(thresh, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    for (size_t i = 0; i < contours.size(); i++) {
        double area = cv::contourArea(contours[i]);
        cout << "Contour Area: " << area << std::endl;
    }

    return 0;
}
```

5. `putText()`

This function is used to add text to an image.

```c++
#include <opencv2/opencv.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace std; 


int main() {
    Mat image = cv::Mat::zeros(500, 500, CV_8UC3);

    string text = "Defective Gear";
    Point position(100, 250);
    int font = cv::FONT_HERSHEY_SIMPLEX;
    double fontScale = 1;
    Scalar color(0, 255, 0);
    int thickness = 2;

    putText(image, text, position, font, fontScale, color, thickness);

    imshow("Text on Image", image);
    waitKey(0);
    return 0;
}
```









