#include <dist2obj.hpp>

// know parameters
#define KN_FOCAL 1      // know focal length
#define KN_WIDTH 1      // know width of actual object
#define KN_DIST 1       // know distance to object

/*
 * Formula: F = (P x  D) / W
 * F = focal length of camera
 * P = width of object in pixels
 * D = distance to object
 * W = know width of actual object
 *
 * area of stop sign at 6 feet = 25070 pixels
 * actual area ~ 459 in^2
 */
static const float F = 5.362;       // focal length
static const float P = 175;         // width in pixels
static const float D = 72;          // know distance to object inches
static const float W = 23.5;        // know width inches

float dist2obj(vector<Point> contour)
{
    // variables
    float dist = 0;
    float currP = 0;

    // find the width in pixels
    RotatedRect rectangle = minAreaRect(contour);

    // current width in pixels
    currP = rectangle.size.width;

    // calculate distance
    dist = ((F*W)/currP)*100;

    return dist;
}
