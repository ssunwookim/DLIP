# Open-cv Code List



## 1. Spatial Filter

#### **1. `blur()`**

> **설명**: 평균값을 사용하여 영상의 노이즈를 제거하는 간단한 필터 (Box Filter).

```
cpp복사편집Size kernelSize(3, 3);  // 커널 크기 설정
blur(src, dst, kernelSize);
```

------

#### **2. `GaussianBlur()`**

> **설명**: 가우시안 분포를 기반으로 한 블러 필터로, 가장자리에서 자연스러운 흐림 효과 제공.

```
cpp


복사편집
GaussianBlur(src, dst, Size(3, 3), 0, 0);
```

------

#### **3. `medianBlur()`**

> **설명**: 주변 픽셀들의 **중앙값**을 사용하여 염색 노이즈(Salt & Pepper Noise) 제거에 효과적.

```
cpp


복사편집
medianBlur(src, dst, 3);  // 필터 크기는 홀수여야 함 (3, 5, 7 등)
```

------

#### **4. `Laplacian()`**

> **설명**: 2차 미분 기반 엣지 감지 필터. 이미지의 경계 부분을 감지하는 데 사용.

```
cpp복사편집int ddepth = CV_16S;
Laplacian(src, dst, ddepth, 3, 1, 0, BORDER_DEFAULT);

// 결과 후처리
Mat src16s, result;
src.convertTo(src16s, CV_16S);
result = src16s + dst;
result.convertTo(result, CV_8U);
```

------

#### **5. `filter2D()`**

> **설명**: 커널(필터)을 직접 정의하여 커스텀 필터링을 수행하는 함수.

```
cpp복사편집Mat kernel = Mat::ones(5, 5, CV_32F) / 25.0f;  // 5x5 평균 필터
filter2D(src, dst, -1, kernel);
```



## 2. Thresholiding & Morphology

### **1. `imread()`**

- **설명**: 이미지 파일을 읽어서 `cv::Mat` 객체로 반환.

- **형식**:

  ```
  cpp
  
  
  복사편집
  Mat img = imread("경로/파일명", IMREAD_COLOR); // 또는 IMREAD_GRAYSCALE
  ```

------

### **2. `cvtColor()`**

- **설명**: 컬러 공간 변환. BGR → GRAY로 자주 사용됨.

- **형식**:

  ```
  cpp
  
  
  복사편집
  cvtColor(src, dst, COLOR_BGR2GRAY);
  ```

------

### **3. `namedWindow()`**

- **설명**: 이미지 출력을 위한 윈도우 생성.

- **형식**:

  ```
  cpp
  
  
  복사편집
  namedWindow("Window Name", WINDOW_NORMAL);
  ```

------

### **4. `createTrackbar()`**

- **설명**: 트랙바 생성. 슬라이더 값 변경 시 콜백 함수 호출됨.

- **형식**:

  ```
  cpp
  
  
  복사편집
  createTrackbar("Trackbar Name", "Window Name", &value, max_value, CallbackFunction);
  ```

------

### **5. `threshold()`**

- **설명**: 이미지 이진화. 픽셀값을 기준으로 흑백 처리.

- **형식**:

  ```
  cpp
  
  
  복사편집
  threshold(src, dst, threshold_value, max_value, threshold_type);
  ```

- **threshold_type 종류**:

  - `THRESH_BINARY` (0)
  - `THRESH_BINARY_INV` (1)
  - `THRESH_TRUNC` (2)
  - `THRESH_TOZERO` (3)
  - `THRESH_TOZERO_INV` (4)

------

### **6. `getStructuringElement()`**

- **설명**: 형태학적 연산을 위한 커널(구조 요소) 생성.

- **형식**:

  ```
  cpp
  
  
  복사편집
  Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
  ```

- **형태 종류**:

  - `MORPH_RECT`
  - `MORPH_ELLIPSE`
  - `MORPH_CROSS`

------

### **7. `erode()`**

- **설명**: 침식 연산. 객체 축소 (잡음 제거).

- **형식**:

  ```
  cpp
  
  
  복사편집
  erode(src, dst, element);
  ```

------

### **8. `dilate()`**

- **설명**: 팽창 연산. 객체 확대 (구멍 채우기).

- **형식**:

  ```
  cpp
  
  
  복사편집
  dilate(src, dst, element);
  ```

------

### **9. `morphologyEx()`**

- **설명**: 고급 형태학 연산 (열기, 닫기 등).

- **형식**:

  ```
  cpp
  
  
  복사편집
  morphologyEx(src, dst, MORPH_OPEN, element);
  ```

- **종류**:

  - `MORPH_OPEN` (침식 → 팽창)
  - `MORPH_CLOSE` (팽창 → 침식)

------

### **10. `imshow()`**

- **설명**: 이미지를 윈도우에 출력.

- **형식**:

  ```
  cpp
  
  
  복사편집
  imshow("Window Name", image);
  ```

------

### **11. `waitKey()`**

- **설명**: 키 입력 대기. 매개값(ms) 동안 대기, 0이면 무한 대기.

- **형식**:

  ```
  cpp
  
  
  복사편집
  int key = waitKey(20);
  ```



## 3. Camera Calibration

---

## 4. Edge Line Detection

### **5. `Canny()`**

- **설명**: Canny 알고리즘을 이용한 엣지(경계) 검출.

- **형식**:

  ```
  cpp
  
  
  복사편집
  Canny(src, dst, lowThreshold, highThreshold, kernel_size);
  ```

- **파라미터 설명**:

  - `lowThreshold`: 경계값 (하한)

  - `highThreshold`: 보통 `lowThreshold * ratio`

  - `kernel_size`: Sobel 커널 크기

  - ### **7. `HoughLines()`**

    - **설명**: Standard Hough Line Transform. 직선을 (rho, theta) 형태로 검출.

    - **형식**:

      ```
      cpp
      
      
      복사편집
      HoughLines(edge_image, lines, 1, CV_PI/180, threshold);
      ```

    - **파라미터 설명**:

      - `lines`: 결과 직선 (rho, theta) 저장할 `Vec2f` 벡터
      - `threshold`: 라인으로 인식되기 위한 최소 교차점 수

    ------

    ### **8. `HoughLinesP()`**

    - **설명**: Probabilistic Hough Transform. 직선을 (x1, y1, x2, y2)로 리턴.

    - **형식**:

      ```
      cpp
      
      
      복사편집
      HoughLinesP(edge_image, linesP, 1, CV_PI/180, threshold, minLineLength, maxLineGap);
      ```

    ------

    ### **9. `line()`**

    - **설명**: 선 그리기 함수.

    - **형식**:

      ```
      cpp
      
      
      복사편집
      line(img, Point(x1, y1), Point(x2, y2), Scalar(B, G, R), thickness);
      ```

------

### **6. `copyTo()`**

- **설명**: 마스크를 이용해 이미지 일부 복사.

- **형식**:

  ```
  cpp
  
  
  복사편집
  src.copyTo(dst, mask); // mask 값이 0이 아닌 영역만 복사
  ```



```cpp
// Edge detection (Canny)
Canny(src, dst, 50, 150, 3);

// 복사해서 컬러 변환 (Canny 출력은 Gray → 컬러 이미지 필요)
cvtColor(dst, cdst, COLOR_GRAY2BGR);
cvtColor(dst, cdstP, COLOR_GRAY2BGR);

// (Option 1) HoughLines - Standard
vector<Vec2f> lines;
HoughLines(dst, lines, 1, CV_PI/180, 150);  // rho=1, theta=1°, threshold=150

// (Option 2) HoughLinesP - Probabilistic
vector<Vec4i> linesP;
HoughLinesP(dst, linesP, 1, CV_PI/180, 50, 50, 10); // threshold=50, minLineLength=50, maxLineGap=10

```



## 5. Corner Detection

----



## 6. Color Image Processing

### ✅ `namedWindow()`

**역할**: 윈도우(창)를 생성하여 이미지를 표시할 수 있게 함.

```
cpp


복사편집
namedWindow("Display", WINDOW_AUTOSIZE);
```

------

### ✅ `imshow()`

**역할**: 이미지(Mat)를 윈도우에 표시.

```
cpp


복사편집
imshow("Display", image);
```

------

### ✅ `waitKey()`

**역할**: 키 입력을 일정 시간 동안 대기.

```
cpp


복사편집
char c = (char)waitKey(100);  // 100ms 대기
```

------

### ✅ `VideoCapture`

**역할**: 카메라 또는 영상 파일을 열고 프레임 단위로 읽음.

```
cpp복사편집VideoCapture cap(0);  // 카메라
VideoCapture cap("video.mp4");  // 영상 파일
```

------

### ✅ `Mat::zeros()`

**역할**: 모든 요소가 0인 행렬을 생성.

```
cpp


복사편집
Mat blank = Mat::zeros(480, 640, CV_8UC3);
```

------

### ✅ `copyTo()`

**역할**: 이미지 데이터를 다른 이미지로 복사.

```
cpp


복사편집
image.copyTo(result);
```

------

### ✅ `cvtColor()`

**역할**: 색상 공간 변환 (예: BGR → HSV).

```
cpp


복사편집
cvtColor(src, hsv, COLOR_BGR2HSV);
```

------

### ✅ `inRange()`

**역할**: 특정 범위에 해당하는 값만 마스킹 (이진 이미지 생성).

```
cpp


복사편집
inRange(hsv, Scalar(30, 100, 100), Scalar(90, 255, 255), mask);
```

------

### ✅ `threshold()`

**역할**: 픽셀 값을 기준으로 이진화 처리.

```
cpp


복사편집
threshold(gray, binary, 100, 255, THRESH_BINARY);
```

------

### ✅ `getStructuringElement()`

**역할**: 형태학 연산을 위한 구조 요소 생성.

```
cpp


복사편집
Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
```

------

### ✅ `dilate()`

**역할**: 팽창 연산. 밝은 영역을 확장하여 잡음 제거에 사용.

```
cpp


복사편집
dilate(binary, dilated, kernel);
```

------

### ✅ `meanStdDev()`

**역할**: 영역의 평균과 표준편차 계산.

```
cpp


복사편집
meanStdDev(roi, meanVal, stddevVal);
```

------

### ✅ `setTrackbarPos()`

**역할**: 트랙바의 현재 위치(값) 변경.

```
cpp


복사편집
setTrackbarPos("Hmin", "Source", hmin);
```

------

### ✅ `bitwise_not()`

**역할**: 이미지의 모든 비트를 반전 (0 ↔ 255).

```
cpp


복사편집
bitwise_not(roi, roi);
```

------

### ✅ `findContours()`

**역할**: 이진 이미지에서 외곽선(윤곽선) 검출.

```
cpp


복사편집
findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
```

------

### ✅ `contourArea()`

**역할**: 외곽선의 면적 계산.

```
cpp


복사편집
double area = contourArea(contours[i]);
```

------

### ✅ `drawContours()`

**역할**: 외곽선을 이미지에 그림.

```
cpp


복사편집
drawContours(output, contours, i, Scalar(0, 255, 0), 2);
```

------

### ✅ `boundingRect()`

**역할**: 외곽선을 둘러싸는 최소 사각형 반환.

```
cpp


복사편집
Rect box = boundingRect(contours[i]);
```

------

### ✅ `rectangle()`

**역할**: 이미지에 사각형 그리기.

```
cpp


복사편집
rectangle(image, box, Scalar(255, 0, 0), 2);
```

------

### ✅ `bitwise_and()`

**역할**: 두 이미지 간 비트 AND 연산 (마스킹에 주로 사용).

```
cpp


복사편집
bitwise_and(img1, img2, result, mask);
```

------

### ✅ `setMouseCallback()`

**역할**: 마우스 이벤트 처리를 위한 콜백 함수 등록.

```
cpp


복사편집
setMouseCallback("Source", onMouse, 0);
```

------

### ✅ `createTrackbar()`

**역할**: 트랙바를 생성하여 변수 값을 실시간 조정.

```
cpp


복사편집
createTrackbar("Hmin", "Source", &hmin, 179, 0);
```