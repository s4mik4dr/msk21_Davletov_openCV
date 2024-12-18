#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Загружаем целое изображение
    cv::Mat img = cv::imread("test.jpg");

    // Проверка, что изображение успешно загружено
    if (img.empty()) {
        std::cerr << "Ошибка при загрузке изображения!" << std::endl;
        return -1;
    }

    // Находим высоту и ширину изображения
    int img_height = img.rows;
    int img_width = img.cols;

    // Разделим изображение на 2 равные части по вертикали (горизонтально)
    int mid = img_height / 2;  // Находим середину изображения по высоте

    // Разрезаем изображение на верхнюю и нижнюю части
    cv::Mat top_part = img(cv::Rect(0, 0, img_width, mid));  // Верхняя часть
    cv::Mat bottom_part = img(cv::Rect(0, mid, img_width, img_height - mid));  // Нижняя часть

    // Преобразуем части в одинаковый тип (если они отличаются)
    if (top_part.type() != bottom_part.type()) {
        top_part.convertTo(top_part, bottom_part.type());
    }

    // Отображаем разрезанные части
    cv::imshow("Top Part", top_part);
    cv::imshow("Bottom Part", bottom_part);

    // Склеиваем изображения обратно по вертикали
    cv::Mat stitched_image;
    cv::vconcat(top_part, bottom_part, stitched_image);  // Используем vconcat для вертикальной склейки

    // Отображаем результат склейки
    cv::imshow("Stitched Image", stitched_image);

    // Сохраняем результат в файл
    cv::imwrite("stitched_image.jpg", stitched_image);

    cv::waitKey(0);
    return 0;
}