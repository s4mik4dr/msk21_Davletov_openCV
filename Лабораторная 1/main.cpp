#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Функция для вычисления и отображения гистограммы
void calculateHistogram(const Mat& image, vector<Mat>& bgr_planes, vector<Mat>& histograms) {
    // Разделение изображения на 3 цветовых канала
    split(image, bgr_planes);

    // Параметры для вычисления гистограммы
    int histSize = 256;
    float range[] = { 0, 256 };
    const float* histRange = { range };

    // Вычисление гистограммы для каждого канала
    for (int i = 0; i < 3; i++) {
        calcHist(&bgr_planes[i], 1, 0, Mat(), histograms[i], 1, &histSize, &histRange);
    }
}

// Функция для выравнивания контраста
Mat equalizeContrast(Mat& image) {
    // Разделяем изображение на каналы
    vector<Mat> bgr_planes;
    split(image, bgr_planes);

    // Выравнивание гистограммы для каждого канала
    for (int i = 0; i < 3; i++) {
        equalizeHist(bgr_planes[i], bgr_planes[i]);
    }

    // Объединение каналов обратно в одно изображение
    Mat equalizedImage;
    merge(bgr_planes, equalizedImage);
    return equalizedImage;
}

// Функция для вертикальной проекции
void verticalProjection(Mat& image) {
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);

    // Проекция по вертикали
    Mat projection = Mat::zeros(1, image.cols, CV_32F);
    for (int y = 0; y < gray.rows; y++) {
        for (int x = 0; x < gray.cols; x++) {
            projection.at<float>(0, x) += gray.at<uchar>(y, x);
        }
    }

    // Визуализация проекции
    Mat projectionImage(256, gray.cols, CV_8U, Scalar(255));
    normalize(projection, projection, 0, 255, NORM_MINMAX);
    for (int x = 0; x < gray.cols; x++) {
        line(projectionImage, Point(x, 255), Point(x, 255 - (int)projection.at<float>(0, x)), Scalar(0), 1);
    }

    imshow("Vertical Projection", projectionImage);
    waitKey(0);
}

// Функция для профиля изображения
void imageProfile(Mat& image) {
    Mat gray;
    cvtColor(image, gray, COLOR_BGR2GRAY);

    // Профиль по центральной строке
    Mat profile = gray.row(gray.rows / 2);
    Mat profileImage(256, profile.cols, CV_8U, Scalar(255));

    for (int x = 0; x < profile.cols; x++) {
        line(profileImage, Point(x, 255), Point(x, 255 - profile.at<uchar>(0, x)), Scalar(0), 1);
    }

    imshow("Image Profile", profileImage);
    waitKey(0);
}

int main() {
    // Загрузка изображения
    Mat image = imread("test.jpg");
    if (image.empty()) {
        cout << "Error loading image" << endl;
        return -1;
    }

    // 1. Вычисление и отображение гистограммы
    vector<Mat> bgr_planes(3);
    vector<Mat> histograms(3);
    calculateHistogram(image, bgr_planes, histograms);

    // 2. Выравнивание контраста изображения
    Mat equalizedImage = equalizeContrast(image);

    // 3. Отображение оригинального и выровненного изображения
    imshow("Original Image", image);
    imshow("Equalized Image", equalizedImage);

    // 4. Вертикальная проекция
    verticalProjection(image);

    // 5. Профиль изображения
    imageProfile(image);

    waitKey(0);
    return 0;
}