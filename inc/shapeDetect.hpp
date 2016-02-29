#ifndef SHAPE_DETECT_HPP
    #define SHAPE_DETECT_HPP

    #include <cmath>
    #include "master.hpp"
    #include "dist2obj.hpp"

    /**
     * Helper function to display text in the center of a contour
     */
    void setLabel(Mat& im, const string label, vector<Point>& contour);

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
     * param dist: pointer to the distance to stop sign
     * return 0: success
     * return 1: error
     */
    Mat shapeDetect(Mat frame, float *dist);

#endif // SHAPE_DETECT_HPP
