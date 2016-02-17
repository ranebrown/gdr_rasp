#include <colorDetect.hpp>

Mat colorDetect(Mat frame)
{
    // blur image to avoid false positives
    medianBlur(frame, frame, 3);

    // convert image to HSV
    Mat hsv_image;
    cvtColor(frame, hsv_image, cv::COLOR_BGR2HSV);

    // Threshold the HSV image, keep only the red pixels
    Mat lower_red_hue_range;
    Mat upper_red_hue_range;
    inRange(hsv_image, Scalar(0, 100, 100), Scalar(10, 255, 255), lower_red_hue_range);
    inRange(hsv_image, Scalar(160, 100, 100), Scalar(179, 255, 255), upper_red_hue_range);

    // Combine the above two images
    Mat red_hue_image;
    addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);

    // apply filter
    GaussianBlur(red_hue_image, red_hue_image, cv::Size(9, 9), 2, 2);

    // copy results back to frame
    frame = red_hue_image.clone();

    // return processed image
    return red_hue_image;
}
