#include "shapeDetect.hpp"

/**
 * Helper function to display text in the center of a contour
 */
void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour)
{
    int fontface = cv::FONT_HERSHEY_SIMPLEX;
    double scale = 0.4;
    int thickness = 1;
    int baseline = 0;

    cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
    cv::Rect r = cv::boundingRect(contour);

    cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
    cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255,255,255), CV_FILLED);
    cv::putText(im, label, pt, fontface, scale, CV_RGB(0,0,0), thickness, 8);
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
 *      - angles are ~135 degrees
 * param frame: the image frame to process
 * return 0: success
 * return 1: error
 */
// TODO squares example has additional filtering and detects multiple shapes rather than 1 per image
int shapeDetect(Mat frame)
{
     if (frame.empty())
     {
        cout<<"bad frame \n";
        return 1;
     }

    // Convert to grayscale
    Mat gray;
    cvtColor(frame, gray, CV_BGR2GRAY);
    while(1)
    {
        imshow("grayscale",gray);
        if(waitKey(30) >= 0) break;
    }

    // down-scale and upscale the image to filter out the noise
    Mat pyr, timg;
    pyrDown(frame, pyr, Size(frame.cols/2, frame.rows/2));
    pyrUp(pyr, timg, frame.size());
    while(1)
    {
        imshow("filter",timg);
        if(waitKey(30) >= 0) break;
    }

    // Convert to binary image using Canny
    Mat bw;
    Canny(gray, bw, 0, 50, 5);
    while(1)
    {
        imshow("binary canny",bw);
        if(waitKey(30) >= 0) break;
    }

    // Find contours
    vector<vector<Point> > contours;
    findContours(bw.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    // vector approx will contain the vertices of the polygonal approximation for the contour
    vector<Point> approx;
    // copy source image to display result XXX
    Mat dst = frame.clone();

    // Loop through all the contours and get the approximate polygonal curves for each contour
    for (unsigned int i = 0; i < contours.size(); i++)
    {
        // Approximate contour with accuracy proportional to the contour perimeter
        approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

        // Skip small or non-convex objects
        if (fabs(contourArea(contours[i])) < 100 || !isContourConvex(approx))
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
            if (vtc == 8 && mincos >= -0.75 && maxcos <= -0.65)
            {
                // found a hexagon
                setLabel(dst, "stopsign", contours[i]);
            }
        }
    }
    /* // draw outline of shape (testing only) XXX */
    /* const Point* p = &approx[0]; */
    /* int n = (int)approx.size(); */
    /* polylines(dst, &p, &n, 1, true, Scalar(0,255,0), 3, LINE_AA); */
    /* while(1) */
    /* { */
    /*     imshow("original",frame); */
        imshow("final", dst);
    /*     if(waitKey(30) >= 0) break; */
    /* } */
    return 0;
}
