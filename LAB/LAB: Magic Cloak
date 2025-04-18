# LAB: Magic Cloak



## LAB: Color Image Segmentation - Magic Cloak

**Date:** 2025 / 04 / 18

**Author:** SUN-WOO KIM 22000090

**Github:**

**Demo video :** [DLIP_2025/04/18_SUN-WOO KIM_LAB: Magic Cloak](https://studio.youtube.com/video/VqORLpUr_zM/edit)

---

## I. Introduction

### 1. Objective

**Goal**: Invisible Cloak

The goal of this project is to create a basic OpenCV program that makes a person wearing a cloak appear invisible.

This is done by detecting the cloak based on its color and replacing it with the background image, so it looks like the person disappears.

The program uses simple image processing techniques like color detection, masking, and background subtraction to achieve this visual effect. This project helps demonstrate how computer vision can be used to create optical illusions and can be a starting point for more advanced augmented reality systems.

### 2. Preparation

#### Software Installation

- OpenCV 4.9.0 
- Visual Studio 2022

#### Dataset

- Sample video 1
- Sample video 2 : Similarly, you can also make the background disappear by using a plain color background.



## Algorithm

### 1. Overview

![image-20250418103510815](C:\Users\ksw86\AppData\Roaming\Typora\typora-user-images\image-20250418103510815.png)

### 2. Procedure

**1.Video Load**

```cpp
VideoCapture cap("LAB_MagicCloak_Sample1.mp4");
```

- Load the input video.
- Store the first frame as the background image.

------

**2. Convert RGB to HSV**

```cpp
cvtColor(Frame, hsv, COLOR_BGR2HSV);
```

- Convert color space from RGB to HSV.
- HSV is better suited for color-based segmentation.

------

**3. Apply Gaussian Blur**

```
GaussianBlur(hsv, hsv, Size(5, 5), 0);
```

- Reduces image noise.
- Helps minimize small color variations that can cause false detections.

------

**4.  HSV Thresholding (inRange)**

```cpp
inRange(hsv, Scalar(MIN(hmin, hmax), MIN(smin, smax), MIN(vmin, vmax)),
			Scalar(MAX(hmin, hmax), MAX(smin, smax), MAX(vmin, vmax)), dst);
```

- Filters out pixels within the specified HSV range.
- Produces a binary mask image.

------

**5. Morphological Dilation**

```cpp
dilate(binary, dst, kernel);
```

- Expands the white regions in the binary image.
- Helps close small gaps and connect regions.

------

**6. Contour Detection**

```cpp
findContours(dst, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
```

- Detects contours in the binary image.
- The largest contour is selected as the subject area.

------

**7. Mask the Subject Area**

```cpp
Rect boxPoint = boundingRect(contours[maxArea_idx]);
rectangle(Frame, boxPoint, Scalar(0, 0, 0), -1);
```

- Create a rectangular mask over the detected subject.
- Black-out the subject area in the original frame.

------

**8. Makeup Background**

```cpp
mask = Mat::zeros(background.size(), CV_8UC1);
rectangle(mask, boxPoint, Scalar(255, 255, 255), -1);

Mat result = Mat::zeros(mask.size(), CV_8UC3);
bitwise_and(background, background, result, mask);
```

- The `mask` is a grayscale image with the same size as the background, where the region of interest (the person's location) is marked with a white rectangle.
- This mask is used to keep only the white area from the background image and remove the rest.
- Using `bitwise_and`, the white rectangular area from the background is extracted and saved into `result`.

------

**9. Combine Video and Background**

```c++
result = result + Frame;
```

- Overlay the masked background region with the original frame.

------

**10. Display and Save the Result**

```c++
imshow("result", result);
writer.write(result);
```

- Display the output in a window.
- Save the video to a file (e.g., `output1.avi`).

------

**11. Final Output**

- The subject is retained while the background is replaced.
- Output video is saved with the composited result.



## Result and Discussion

### 1. Result

**1. Original Image with HSV Sliders** 

![image-20250418113848188](C:\Users\ksw86\AppData\Roaming\Typora\typora-user-images\image-20250418113848188.png)

- The user adjusts HSV sliders to select the color range (green) to extract from the image.

**2. Contour Detection**

![image-20250418112532081](C:\Users\ksw86\AppData\Roaming\Typora\typora-user-images\image-20250418112532081.png)

-  The red contour outlines the area detected from the mask using HSV thresholding.

**3. Binary Mask Image**

![image-20250418112538588](C:\Users\ksw86\AppData\Roaming\Typora\typora-user-images\image-20250418112538588.png)

- A binary mask where the green area is white (255), and the rest is black (0).

**4. Green Area Removed from Original Image**

![image-20250418112547703](C:\Users\ksw86\AppData\Roaming\Typora\typora-user-images\image-20250418112547703.png)

- The green area is removed using `rectangle()` with the mask.

**5. Cropped Background Only**

![image-20250418112556276](C:\Users\ksw86\AppData\Roaming\Typora\typora-user-images\image-20250418112556276.png)

- The background image portion matching the green area is extracted using the mask.

**6. Final Composite (Green Replaced with Background)**

![image-20250418112605277](C:\Users\ksw86\AppData\Roaming\Typora\typora-user-images\image-20250418112605277.png)

- Final composited image where the green area is replaced by the background image.

### 2. Discussion

 In this project, we implemented the chroma key technique using the HSV color space and replaced a specific screen area with a background image. By adjusting the HSV threshold, we were able to effectively isolate a specific color area on the screen. By using morphological operations such as dilation and an outline detection technique together to increase the accuracy of the mask, we were able to cleanly extract only the desired area. This method is effective in environments with constant lighting such as indoors, but in cases where there are objects with similar colors or where the lighting changes such as outdoors, there is a limitation that the accuracy may decrease because the masking may be done at the same time or the mark key area may not be specified.



## Conclusion

 Through this project, we confirmed that it is possible to perform real-time background substitution using color segmentation based on HSV thresholding. By combining OpenCV functions such as `inRange`, contour detection, and `bitwise_and`, we were able to extract the designated area and naturally replace it with a desired background image. The process was intuitive and computationally efficient, making it suitable for simple real-time applications. Although the system works well under controlled conditions, further improvements such as adaptive thresholding or machine learning-based segmentation could enhance its robustness in dynamic or complex environments.



## Appendix

**1. `inRange()`**

- **Description**: `inRange()` is a function in OpenCV used to create a binary mask by thresholding an image. It checks if each pixel lies between two specified boundaries (lower and upper bounds) and sets the result pixel to 255 (white) if it does, or 0 (black) otherwise.
- **Parameters**: 
  - `src`: The source image (usually HSV or grayscale).
  - `lowerb`: Scalar representing the lower boundary (e.g., HSV minimum).
  - `upperb`: Scalar representing the upper boundary (e.g., HSV maximum).
  - `dst`: Output binary mask (type CV_8UC1) where 255 indicates a match and 0 means no match.
- **Example**:

```c++
Mat hsv, mask;
cvtColor(image, hsv, COLOR_BGR2HSV);

Scalar lower(60, 100, 100);  // H, S, V lower bound (green-ish color)
Scalar upper(90, 255, 255);  // H, S, V upper bound

inRange(hsv, lower, upper, mask);
```

**2. `bitwise_and()`**

- **Description**: `bitwise_and()` performs a **pixel-wise AND operation** between two images.
   It is often used with a mask to isolate or extract specific regions of an image (e.g., tracking an object or removing the background).
- **Parameters**: 
  - `src1`: First input image.
  - `src2`: Second input image (often the same as `src1`).
  - `dst`: Output image where the result is stored.
  - `mask`: *(Optional)* Binary mask image (CV_8UC1). Only pixels corresponding to white (255) in the mask are processed.
- **Example**:

```c++
Mat background, result, mask;
// 'mask' has white in the region of interest and black elsewhere
bitwise_and(background, background, result, mask);
```
