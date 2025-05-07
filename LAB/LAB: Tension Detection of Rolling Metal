# LAB: Tension Detection of Rolling Metal



# LAB: Tension Detection of Rolling Metal 

**Date:** 2025 / 05 / 07

**Author:** SUN-WOO KIM 22000090

**Github:**

**Demo video :** [LAB: Tension Detection of Rolling Metal Sheet](https://www.youtube.com/watch?v=alJ1Kg1YLZI)

------



# Introduction

## 1. Objective

**Goal**: Tension Detection of Rolling Metal 

This is a simplified industrial problem for designing a machine vision system that can detect the level of tension in the rolling metal sheet.

The tension in the rolling process can be derived by measuring the curvature level of the metal sheet with the camera.

The surface of the chamber and the metal sheet are both specular reflective that can create virtual objects in the captured images. You need to design a series of machine vision algorithms to clearly detect the edge of the metal sheet and derive the curvature and tension level.

## 2. Preparation

### Software Installation

- Python 3.9.21
- CUDA 11.8
- cuDNN 7.6
- PyTorch 2.0.x
- Anaconda for Python 3.9
- Microsoft VS code

### Dataset

- `LAB3_Video.mp4`
-  **Dataset link** : [Download the test video](https://github.com/ykkimhgu/DLIP-src/blob/main/LAB_Tension_Detection_dataset/LAB3_Video.mp4)



# Algorithm

## 1. Overview

![image-20250507111125047](C:\Users\ksw86\AppData\Roaming\Typora\typora-user-images\image-20250507111125047.png)

 The flow chart of this experiment first separates the input image into RED channels to emphasize the brightness information of the region of interest, reduces noise with Gaussian Blur, and then binarizes the image using the inRange() function to separate the object. Next, the residual noise is removed using the Morphology operation, the area to be analyzed is limited with the ROI mask, and the main curve is detected using findContours(). The second-order polynomial regression is performed based on the coordinates of the detected curves to calculate the vertices of the curves, and the tension score and level are calculated using the distance from the floor of these vertices. The final result is then displayed on the screen and saved as a video file.

## 2. Procedure

### **2-1 Input video**

- Read the video frame by frame using`cv2.VideoCapture`

### **2-2 RED Channel Separation****

- Separate the RGB channels from each frame, then extract only the R channel and use it as a grayscale image.

```python
 # Using R channel
 b, g, r = cv2.split(frame)
 gray = r
 cv2.imshow("Grayscale", gray)
```

### **2-3 Gaussian Blur**

- `cv2.GaussianBlur(gray, (5,5), 0)` removes noise and improves the stability of edge detection and binarization.

```python
 blur = cv2.GaussianBlur(gray, (5, 5), 0)
```

### **2-4 inRange() Binarization**

- Apply `cv2.inRange(blur, 60, 240)` to the blurred image to generate a binarized mask.

```python
 binary = cv2.inRange(blur, 60, 240)
```

### **2-5 Morphology Operation**

- Remove small noise by performing `cv2.morphologyEx(binary, cv2.MORPH_OPEN, kernel)` with a 15×15 square kernel.

```python
    kernel = np.ones((15, 15), np.uint8)
    binary = cv2.morphologyEx(binary, cv2.MORPH_OPEN, kernel)
```

- Additionally, `erode` and `dilate` are repeatedly applied to refine the object's outline.

```python
    for _ in range(iters_erode):
        binary = cv2.erode(binary, kernel, iterations=1)
    for _ in range(iters_dilate):
        binary = cv2.dilate(binary, kernel, iterations=1)
```

### **2-6 ROI Masking**

- Limit the analysis to the region y:49h∼h,  x:0∼516wy: \frac{4}{9}h \sim h,\; x: 0 \sim \frac{5}{16}wy:94h∼h,x:0∼165w and apply the mask with `cv2.bitwise_and`.

```python
# Create and apply ROI mask
roi_mask = np.zeros((h, w), np.uint8)
x_end   = int(w * 5 / 16)
y_start = int(h * 4 / 9)
roi_mask[y_start:h, 0:x_end] = 255

# Use a mask to leave only the area of ​​interest
masked = cv2.bitwise_and(binary, binary, mask=roi_mask)
cv2.imshow("ROI Masked", masked)
```

### **2-7 Contour Detection**

- Detect all contours in the ROI mask image with `cv2.findContours(..., cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)`
- Select the contour (curve) with the largest area with `max(..., key=cv2.contourArea)`.

```python
# Outline detection
contours, _ = cv2.findContours(
    masked,
    cv2.RETR_EXTERNAL,
    cv2.CHAIN_APPROX_SIMPLE
)

# Select the largest of the detected contours
longest_contour = max(contours, key=cv2.contourArea)

# Draw only contours on a blank image for visualization
contour_img = np.zeros_like(frame)
cv2.drawContours(contour_img, [longest_contour], -1, (0, 255, 0), 2)
cv2.imshow("Contours", contour_img)
```

### **2-8 Quadratic Curve Fitting**

- Reshape the chosen contour to a list of (x,y)(x,y)(x,y) points and fit a quadratic curve y=ax2+bx+cy = ax^2 + bx + cy=ax2+bx+c.

```python
# Contour point extraction
pts = longest_contour.reshape(-1, 2)
x = pts[:, 0]
y = pts[:, 1]

# Quadratic curve fitting only when at least 5 points are available
if len(x) >= 5:
    coeffs = np.polyfit(x, y, 2)
    a, b, c = coeffs
```

### **2-9 Vertex Calculation**

- Compute the parabola’s vertex coordinates using

![image-20250507113125282](C:\Users\ksw86\AppData\Roaming\Typora\typora-user-images\image-20250507113125282.png)



```python
# Vertex Calculation
x_vertex = -b / (2 * a)
y_vertex = a * x_vertex**2 + b * x_vertex + c
```

### **2-10 Tension Score & Level Calculation**

- Compute the distance score=h−yvertexscore = h - y_{vertex}score=h−yvertex from the floor of the vertex,
- The level (1~3) is determined using the `get_tension_level()` function.

```python
# Calculating distance (score) and level from the floor
y_from_bottom = h - y_vertex
level = get_tension_level(y_vertex, h)
```

### **2-11 Visualization & File Saving**

- Draw curves and vertices with `c v2.polylines` and `cv2.circle`, and display the Level and Score with `cv2.putText`.
- The two reference lines (y=h–250, y=h–120) are visualized as dotted lines using `draw_dashed_line()` and output in real time using `cv2.imshow`.
- Save the resulting video as `output_tension_curve.mp4` via `VideoWriter`.

```python
# Copy original frame
final = frame.copy()

# Visualizing the 2nd curve
curve_pts = [
    (xd, int(a * xd**2 + b * xd + c))
    for xd in range(w)
    if 0 <= int(a * xd**2 + b * xd + c) < h
]
if curve_pts:
    cv2.polylines(final, [np.array(curve_pts, dtype=np.int32)],
                  False, (0, 255, 0), thickness=3)

# Marked with a circle at the top
cv2.circle(final, (int(x_vertex), int(y_vertex)), 6, (255, 255, 255), -1)

# Level and Score text
cv2.putText(final, f"Level : {level}", (50, 50),
            cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
cv2.putText(final, f"Score : {y_from_bottom:.2f}", (50, 100),
            cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)

# Baseline dotted line visualization
draw_dashed_line(final, (0, h - 250), (w, h - 250), (0, 255, 0), 2)
draw_dashed_line(final, (0, h - 120), (w, h - 120), (255, 255, 0), 2)

# Output and save results
cv2.imshow("Output", final)
out.write(final)
```



# Result and Discussion

## 1. Final Result

- **Test video**

![image-20250507115044741](C:\Users\ksw86\AppData\Roaming\Typora\typora-user-images\image-20250507115044741.png)

- **Processing Image**

| RED Channel Separation                                       | Gaussian Blur**                                              |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| ![image-20250507120047696](C:\Users\ksw86\AppData\Roaming\Typora\typora-user-images\image-20250507120047696.png) | ![image-20250507120051252](C:\Users\ksw86\AppData\Roaming\Typora\typora-user-images\image-20250507120051252.png) |
| **Binarization & Morphology**                                | **ROI Masking**                                              |
| ![image-20250507120119594](C:\Users\ksw86\AppData\Roaming\Typora\typora-user-images\image-20250507120119594.png) | ![image-20250507120123166](C:\Users\ksw86\AppData\Roaming\Typora\typora-user-images\image-20250507120123166.png) |
| **Contour Detection**                                        |                                                              |
| ![image-20250507120127669](C:\Users\ksw86\AppData\Roaming\Typora\typora-user-images\image-20250507120127669.png) |                                                              |

- **Final Image**

![image-20250507114908521](C:\Users\ksw86\AppData\Roaming\Typora\typora-user-images\image-20250507114908521.png)

- The tension level is divided into levels 1 to 3 and a score representing the y-position from the bottom of the image to the curvature peak is output, and you can see that the curvature edge is well expressed in the final result as an overlay.

## 2. Discussion

![image-20250507115301916](C:\Users\ksw86\AppData\Roaming\Typora\typora-user-images\image-20250507115301916.png)

-  In this experiment, after effectively removing noise through RED channel separation, Gaussian Blur, inRange binarization, and morphological operation, and then selecting only the region of interest by applying the ROI mask, we were able to reliably detect the main curve with findContours. The number of repeated erosion and dilation was experimentally obtained, and unnecessary parts were removed while maintaining the continuity of the curve in general, but parameter readjustment is necessary when the image resolution or lighting conditions change. In addition, since noise due to lighting cannot be completely removed even if some are removed as in the photo above, additional noise removal methods are also needed. Quadratic curve fitting using np.polyfit is reliable only when there are sufficient contour points (≥5), so errors in vertex calculation may occur for short or irregular curves. The tension stage is intuitively classified by clearly visualizing the level baseline (y=h–250, y=h–120) with the draw_dashed_line function, but the dash interval and baseline value should be reset according to the application environment. The final result video shows the curves, vertices, levels and scores overlaid accurately, making it useful for monitoring the warpage of metal plates in real time.



# Conclusion

 In this study, RED channel separation, Gaussian Blur, inRange binarization, morphological operation, ROI masking, contour detection, and second-order polynomial curve fitting were configured as a series of pipelines to detect and visualize the bending curve of a rolling metal plate in real time, and the tension score and level were accurately calculated based on the vertex positions of the curve. The processing results are saved as screen and video files and can be immediately used for subsequent analysis and quality control. In the future, we plan to continuously optimize the morphological operation parameters and curve fitting algorithm to ensure stable performance under various resolutions and lighting conditions.



# Appendix

#### 1.`np.polyfit`

```python
# 1) Extract (x, y) coordinates from the selected contour:
pts = longest_contour.reshape(-1, 2)
x = pts[:, 0]   # all x-coordinates
y = pts[:, 1]   # all y-coordinates

# 2) Fit a 2nd-degree polynomial y = a·x² + b·x + c to the data:
#    np.polyfit returns the coefficients [a, b, c]
a, b, c = np.polyfit(x, y, 2)

# 3) You can now compute the curve’s y for any x:
y_at_100 = a * 100**2 + b * 100 + c

```

- `np.polyfit(x, y, 2)` performs a least-squares regression of degree 2 on your point set, returning the best-fit coefficients `a`, `b`, and `c`.
- From these, the analytic curve equation y=a x2+b x+cy = a\,x^2 + b\,x + cy=ax2+bx+c is defined and can be evaluated at any xxx.

#### 2.`cv2.polylines`

```python
# 1) Sample points along the fitted curve within image bounds:
curve_pts = [
    (xd, int(a*xd**2 + b*xd + c))
    for xd in range(0, w, 5)            # step by 5 pixels for efficiency
    if 0 <= a*xd**2 + b*xd + c < h      # only keep points inside the frame
]

# 2) Convert to a NumPy array of shape (N, 1, 2) or (N, 2):
pts_array = np.array(curve_pts, dtype=np.int32)

# 3) Draw the polyline:
cv2.polylines(
    final,             # destination image
    [pts_array],       # list of point arrays
    isClosed=False,    # do not connect last point back to first
    color=(0, 255, 0), # BGR green
    thickness=3        # line thickness in pixels
)
```

- You generate a sequence of (x,y)(x,y)(x,y) points along the polynomial curve.

- `cv2.polylines` then connects these points in order, rendering a smooth approximation of the fitted parabola.
- By tuning the sampling step (here 5 px) and thickness, you balance visual smoothness against rendering cost.
