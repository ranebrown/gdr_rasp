#include "shapeDetect.hpp"

/**
 * Helper function to display text in the center of a contour
 */
void setLabel(Mat& im, const string label, vector<Point>& contour)
{
    int fontface = FONT_HERSHEY_SIMPLEX;
    double scale = 0.4;
    int thickness = 1;
    int baseline = 0;

    Size text = getTextSize(label, fontface, scale, thickness, &baseline);
    Rect r = boundingRect(contour);

    Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
    rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255,255,255), CV_FILLED);
    putText(im, label, pt, fontface, scale, CV_RGB(0,0,0), thickness, 8);
}

/**
 * Helper function to find a cosine of angle between vectors
 * from pt0->pt1 and pt0->pt2
 */
double angle(Point pt1, Point pt2, Point pt0)
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

/*
 * Detects a octagon (stop sign)
 *      - 8 vertices
 *      - angles are ~135 degrees -> cos(135)~-.70
 * param frame: the image frame to process
 * param dist: pointer to the distance to stop sign
 * return 0: success
 * return 1: error
 */
Mat shapeDetect(Mat frame, float *dist)
{
    if (frame.empty())
    {
       cout<<"bad frame \n";
       return Mat();
    }

    // filter image
    GaussianBlur(frame, frame, Size(7,7), 1.5, 1.5);

    // Convert to binary image using Canny
    Mat bw;
    Canny(frame, bw, 50, 200, 5);

    // increase detected pixels
    dilate(bw, bw, Mat(), Point(-1,-1));

    // Find contours
    vector<vector<Point> > contours;
    findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    // vector approx will contain the vertices of the polygonal approximation for the contour
    vector<Point> approx;

    // Loop through all the contours and get the approximate polygonal curves for each contour
    for (unsigned int i = 0; i < contours.size(); i++)
    {
        // Approximate contour with accuracy proportional to the contour perimeter
        approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

        // Skip small or non-convex objects
        if (fabs(contourArea(contours[i])) < 200 || !isContourConvex(approx))
            continue;

        // possible octagon
        if (approx.size() == 8)
        {
            // Number of vertices of polygonal curve
            int vtc = approx.size();

            // Get the cosines of all corners
            vector<double> cos;
            for (int j = 2; j < vtc+1; j++)
                cos.push_back(angle(approx[j%vtc], approx[j-2], approx[j-1]));

            // Sort ascending the cosine values
            sort(cos.begin(), cos.end());

            // Get the lowest and the highest cosine
            double mincos = cos.front();
            double maxcos = cos.back();

            // Use the degrees obtained above and the number of vertices to determine the shape of the contour
            // angle are pretty relaxed in case camera isn't straight on
            if (vtc == 8 && mincos >= -0.85 && maxcos <= -0.55)
            {
                // found a octagon (stop sign) -> caclulate distance
                *dist = dist2obj(contours[i]);
                setLabel(bw, "stopsign", contours[i]);
            }
        }
    }
    return bw;
}
