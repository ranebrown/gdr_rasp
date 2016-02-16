#ifndef SHAPE_DETECT_HPP
    #define SHAPE_DETECT_HPP

    #include <opencv.hpp>
    #include <cmath>
    #include <iostream>

    using namespace cv;
    using namespace std;

    /**
     * Helper function to display text in the center of a contour
     */
    void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour);

    /**
     * Helper function to find a cosine of angle between vectors
     * from pt0->pt1 and pt0->pt2
     */
    double angle(Point pt1, Point pt2, Point pt0);

    /*
     * Detects a hexagon
     *      - 6 vertices
     *      - angles are ~120 degrees
     * param frame: the image frame to process
     * return 0: success
     * return 1: error
     */
    int shapeDetect(Mat frame);

#endif // SHAPE_DETECT_H
