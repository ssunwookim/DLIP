# Assignment: Lane Detection



## Assignment: Lane Detection

**Date:** 2025 / 04 / 08

**Author:** SUN-WOO KIM 22000090

**Github:** 

------



## Introduction

### 1. Objective

**Goal**: Lane identification and vanishing point detection for vehicle driving images

 This code implements an image processing-based lane recognition system. Its purpose is to receive an image in front of a vehicle, identify the left and right lanes, and visually display the vanishing point where they meet.

### 2. Preparation

#### Software Installation

- OpenCV 4.9.0 
- Visual Studio 2022

#### Dataset

- Data Image 1

![Lane_center.jpg](https://github.com/ssunwookim/DLIP/blob/main/Image/Assignment:%20Lane%20Detection/Lane_center.jpg?raw=true)

- Data Image 2

![Lane_changing.jpg](https://github.com/ssunwookim/DLIP/blob/main/Image/Assignment:%20Lane%20Detection/Lane_changing.jpg?raw=true)



## Algorithm

### 1. Overview

![1.png](https://github.com/ssunwookim/DLIP/blob/main/Image/Assignment:%20Lane%20Detection/1.png?raw=true)

### 2. Procedure

**1. Load Image**

```c++
Out1 = imread("Lane_center.jpg");
Out2 = imread("Lane_changing.jpg");
```

- Load two road images.
- After that, lane detection is performed using the `LaneDetection()` function.

**2. Noise Removal**

```c++
GaussianBlur(src, src, Size(5, 5), 0);
```

- Apply Gaussian blur to the image to remove noise.
- This is to detect lane edges more clearly.

**3. Apply ROI**

```c++
Mat roi = applyROI(edge);
```

- In the ROI function, only the trapezoid-shaped area is left and the rest is masked.
- Filters only those sections of the road where actual lanes are likely to exist.

**4. Edge Detection (Canny)**

```c++
Canny(src, edge, 150, 200, 3);
```

- Detect edges in an image using the Canny algorithm.
- Find a line with a clear boundary, like a lane.

**5. Curve Fitting** 

```c++
vector<Point> left_pts, right_pts;

for (const auto& line : lines) {
        Point p1(line[0], line[1]);
        Point p2(line[2], line[3]);

        double dx = p2.x - p1.x;
        if (dx == 0) continue;

        double slope = (double)(p2.y - p1.y) / dx; // 기울기 계산

        if (abs(slope) < 0.3) continue; // 수평선 제외

        if (slope < 0) {                // 왼쪽 차선 저장
            left_pts.push_back(p1);
            left_pts.push_back(p2);
        }
        else {                          // 오른쪽 차선 저장
            right_pts.push_back(p1);
            right_pts.push_back(p2);
        }
    }
// 직선 피팅
// line[0] : x 방향 단위 벡터
// line[1] : y 방향 단위 벡터
// line[2] : 직선 위 한 점의 x좌표
// line[3] : 직선 위 한 점의 y좌표
Vec4f left_line, right_line;
fitLine(left_pts, left_line, DIST_L2, 0, 0.01, 0.01);
fitLine(right_pts, right_line, DIST_L2, 0, 0.01, 0.01);
```

- Divide the straight lines detected by HoughLinesP into left and right according to their slopes.
- Based on each point, we fit a straight line using `fitLine()`.

**6. Compute Vanishing Point**

```c++
    // 소실점 계산
    double l_slope = left_line[1] / left_line[0];                 // 왼쪽 기울기
    double l_intercept = left_line[3] - l_slope * left_line[2];   // 왼쪽 직선 y 절편
    double r_slope = right_line[1] / right_line[0];               // 오른쪽 직선 기울기
    double r_intercept = right_line[3] - r_slope * right_line[2]; // 오른쪽 직선 y 절편


    int px = (int)((r_intercept - l_intercept) / (l_slope - r_slope)); // 소실점 x좌표
    int py = (int)(l_slope * px + l_intercept);                        // 소실점 y좌표
    circle(src, Point(px, py), 8, Scalar(255, 200, 255), 2); // 소실점
```

- Calculate the coordinates of the **vanishing point** using the slope and intercept of the left/right straight lines.
- Find the point where the two lanes converge.

**7.Visualize Lanes** 

```c++
    // 왼쪽 차선 그리기
    int y1_l = py;                              //  소실점 py
    int y2_l = height;                          //  화면 아래쪽 끝
    int x1_l = (y1_l - l_intercept) / l_slope;  //  소실점에서의 x값
    int x2_l = (y2_l - l_intercept) / l_slope;  //  화면 아래쪽 x값

    line(src, Point(x1_l, y1_l), Point(x2_l, y2_l), Scalar(0, 0, 255), 3); // 왼쪽 차선 그림

    // 오른쪽 차선 그리기
    int y1_r = py;
    int y2_r = height;
    int x1_r = (y1_r - r_intercept) / r_slope;
    int x2_r = (y2_r - r_intercept) / r_slope;

    line(src, Point(x1_r, y1_r), Point(x2_r, y2_r), Scalar(0, 255, 0), 3);

    // 소실점 기준 바닥까지 수직선
    line(src, Point(px, src.rows), Point(px, py), Scalar(255, 0, 0), 2);
```

- Find the point where the two lanes converge.
- The vanishing point is indicated by a light pink, open circle in the middle.
- Also visualize vertical lines dropping down from the vanishing point.



## Result and Discussion

### 1. Result

- Output image 1

![OUT1.png](https://github.com/ssunwookim/DLIP/blob/main/Image/Assignment:%20Lane%20Detection/OUT1.png?raw=true)

- Output image 2

![OUT2.png](https://github.com/ssunwookim/DLIP/blob/main/Image/Assignment:%20Lane%20Detection/OUT2.png?raw=true)

### 2. Discussion

 The two images demonstrate that the proposed lane detection algorithm works effectively in various driving environments. In the first image, despite the complex situation of a branching road, the left and right lanes are accurately detected as red and green lines, respectively, and the vanishing point formed by the extensions of the two lines is also stably displayed. In the second image, the lanes are closer to straight lines, and even long lanes are clearly extracted, and the vehicle's direction of travel is also intuitively expressed as a blue line. In this way, the proposed system demonstrates robust performance in that it can stably recognize lanes and determine the driving direction through vanishing points even when the road shape changes.

## Conclusion

 The proposed lane recognition system stably implements the lane-centered driving function essential for autonomous driving through the entire pipeline of image-based preprocessing, ROI setting, edge detection, curve fitting, vanishing point calculation, and visualization. In particular, the ability to recognize lanes well even on curved lanes or at junctions and to determine the vehicle's direction of travel based on the vanishing point is a very useful feature for real-time driving control systems. Overall, this algorithm can be evaluated as a lane recognition-based direction judgment system with practicality and accuracy applicable to real road environments.

## Appendix

**1. `cv2.Canny()`**

- **Description**: Detects edges in an image using the Canny edge detection algorithm.
- **Parameters**: Input image, lower threshold, upper threshold.
- **Example**:

```c++
Mat img = imread("road.jpg", IMREAD_GRAYSCALE);
Mat edges;
Canny(img, edges, 50, 150);
imshow("Canny Edge", edges);
waitKey(0);
```

**2. `cv2.HoughLinesP()`**

- **Description**: Detects line segments using the Probabilistic Hough Transform.
- **Parameters**: Edge image, distance resolution, angle resolution, threshold, minimum line length, maximum line gap.
- **Example**:

```c++
vector<Vec4i> lines;
HoughLinesP(edges, lines, 1, CV_PI/180, 50, 50, 10);
for (auto line : lines) {
    line(img, Point(line[0], line[1]), Point(line[2], line[3]), Scalar(0, 255, 0), 2);
}

```

**3. `cv2.fitLine()`**

- **Description**: Fits a line to a set of 2D or 3D points using a least-squares method.
- **Returns**: Direction vector `(vx, vy)` and a point `(x, y)` on the line.
- **Example**:

```c++
vector<Point> points = { Point(100, 200), Point(150, 250), Point(200, 300) };
Vec4f line;
fitLine(points, line, DIST_L2, 0, 0.01, 0.01);
// line = [vx, vy, x0, y0]

```

**4. `cv2.line()`**

- **Description**: Draws a straight line on an image.
- **Parameters**: Image, start point, end point, color (BGR), thickness.
- **Example**:

```c++
line(img, Point(100, 100), Point(300, 300), Scalar(255, 0, 0), 2);
```

**5. `cv2.fillPoly()`**

- **Description**: Fills a polygonal area with a specific color. Commonly used to define and highlight regions of interest (ROI).
- **Parameters**: Image, array of vertices, color.
- **Example**:

```c++
Point pts[1][4] = {
    { Point(100, 400), Point(250, 200), Point(350, 200), Point(500, 400) }
};
const Point* ppt[1] = { pts[0] };
int npt[] = { 4 };
fillPoly(img, ppt, npt, 1, Scalar(0, 255, 0));
```
