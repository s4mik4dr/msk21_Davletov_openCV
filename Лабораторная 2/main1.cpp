#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

// Функция для полиномиального искажения
Mat applyPolynomialDistortion(const Mat& src) {
    Mat dst = src.clone();
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            float x = j;
            float y = i;
            // Полиномиальное искажение: x' = x + a * x^2, y' = y + b * y^2
            float a = 0.001, b = 0.001;
            x += a * pow(x, 2);
            y += b * pow(y, 2);
            if (x >= 0 && x < src.cols && y >= 0 && y < src.rows) {
                dst.at<Vec3b>(i, j) = src.at<Vec3b>(y, x);
            }
        }
    }
    return dst;
}

// Функция для синусоидального искажения
Mat applySinusoidalDistortion(const Mat& src) {
    Mat dst = src.clone();
    for (int i = 0; i < src.rows; ++i) {
        for (int j = 0; j < src.cols; ++j) {
            float x = j + 30 * sin(2 * M_PI * i / 100);  // Синусоидальное искажение по X
            float y = i;
            if (x >= 0 && x < src.cols) {
                dst.at<Vec3b>(i, j) = src.at<Vec3b>(y, x);
            }
        }
    }
    return dst;
}

int main() {
    // Загрузка изображения
    Mat img = imread("test.jpg");
    if (img.empty()) {
        cout << "Не удалось загрузить изображение!" << endl;
        return -1;
    }

    // Показ изображения
    imshow("Original Image", img);

    // 1. Конформные преобразования

    // Сдвиг (Translation)
    Mat translationMatrix = (Mat_<double>(2, 3) << 1, 0, 50, 0, 1, 30); // Сдвиг на 50 по X и 30 по Y
    Mat shiftedImg;
    warpAffine(img, shiftedImg, translationMatrix, img.size());
    imshow("Shifted Image", shiftedImg);

    // Однородное масштабирование (Uniform scaling)
    double scaleX = 1.5, scaleY = 1.5;
    Mat scaledImg;
    resize(img, scaledImg, Size(), scaleX, scaleY);
    imshow("Scaled Image", scaledImg);

    // Поворот (Rotation)
    Point2f center(img.cols / 2.0, img.rows / 2.0);  // Центр изображения
    double angle = 45.0;  // Угол поворота
    double scale = 1.0;   // Масштаб (оставляем без изменений)
    Mat rotationMatrix = getRotationMatrix2D(center, angle, scale);
    Mat rotatedImg;
    warpAffine(img, rotatedImg, rotationMatrix, img.size());
    imshow("Rotated Image", rotatedImg);

    // Отражение (Reflection)
    Mat reflectedImg;
    flip(img, reflectedImg, 0); // 0 — отражение относительно горизонтальной оси
    imshow("Reflected Image", reflectedImg);

    // 2. Аффинные преобразования

    // Скос (Shearing)
    Mat shearingMatrix = (Mat_<double>(2, 3) << 1, 0.5, 0, 0.5, 1, 0); // Горизонтальный и вертикальный скос
    Mat shearedImg;
    warpAffine(img, shearedImg, shearingMatrix, img.size());
    imshow("Sheared Image", shearedImg);

    // Неоднородное масштабирование (Non-uniform scaling)
    Mat nonUniformScaledImg;
    resize(img, nonUniformScaledImg, Size(img.cols * 2, img.rows / 2)); // Увеличение по горизонтали и уменьшение по вертикали
    imshow("Non-uniformly Scaled Image", nonUniformScaledImg);

    // 3. Нелинейные преобразования

    // Проективное преобразование (Projective transformation)
    Point2f srcPoints[4] = {Point2f(0, 0), Point2f(img.cols - 1, 0), Point2f(0, img.rows - 1), Point2f(img.cols - 1, img.rows - 1)};
    Point2f dstPoints[4] = {Point2f(50, 50), Point2f(img.cols - 1 - 50, 50), Point2f(50, img.rows - 1 - 50), Point2f(img.cols - 1 - 50, img.rows - 1 - 50)};
    Mat projectiveMatrix = getPerspectiveTransform(srcPoints, dstPoints);
    Mat projectiveImg;
    warpPerspective(img, projectiveImg, projectiveMatrix, img.size());
    imshow("Projective Image", projectiveImg);
    // Кусочно-линейное отображение (Piecewise linear transformation)
    Mat piecewiseImg = img.clone();
    for (int i = img.cols / 2; i < img.cols; ++i) {
        for (int j = 0; j < img.rows; ++j) {
            // Увеличиваем правую половину изображения
            piecewiseImg.at<Vec3b>(j, i) = img.at<Vec3b>(j, i / 2);
        }
    }
    imshow("Piecewise Linear Transformation", piecewiseImg);

    // Полиномиальное отображение (Polynomial distortion)
    Mat polynomialDistortedImg = applyPolynomialDistortion(img);
    imshow("Polynomial Distortion", polynomialDistortedImg);

    // Синусоидальное искажение (Sinusoidal distortion)
    Mat sinusoidalDistortedImg = applySinusoidalDistortion(img);
    imshow("Sinusoidal Distortion", sinusoidalDistortedImg);

  // Ожидание нажатия клавиши
    cout << "Нажмите 'Q' для выхода." << endl;
    while (true) {
        int key = waitKey(0);
        if (key == 'q' || key == 'Q') {
            break;
        }
    }

    return 0;
}