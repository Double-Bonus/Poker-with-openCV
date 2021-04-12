#include <iostream>
#include <string>
#include <sstream>


#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int match(string filename, string templatename);

int main(int argc, char** agrv)
{
    int test  = match("img_poker", "template");
    return 0;
}



int match(string filename, string templatename)
{
    Mat ref = cv::imread(filename + ".png");
    Mat tpl = cv::imread(templatename + ".png");
    if (ref.empty() || tpl.empty())
    {
        cout << "Error reading file(s)!" << endl;
        return -1;
    }

    Mat gref, gtpl;
    cvtColor(ref, gref, COLOR_BGRA2GRAY);
    cvtColor(tpl, gtpl, COLOR_BGRA2GRAY);

    const int low_canny = 110;
    Canny(gref, gref, low_canny, low_canny * 3);
    Canny(gtpl, gtpl, low_canny, low_canny * 3);

    imshow("file", gref);
    imshow("template", gtpl);

    Mat res_32f(ref.rows - tpl.rows + 1, ref.cols - tpl.cols + 1, CV_32FC1);
    matchTemplate(gref, gtpl, res_32f, NORM_MINMAX);

    Mat res;
    res_32f.convertTo(res, CV_8U, 255.0);
    imshow("result", res);

    int size = ((tpl.cols + tpl.rows) / 4) * 2 + 1; //force size to be odd
    adaptiveThreshold(res, res, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, size, -64);
    imshow("result_thresh", res);

    while (1)
    {
        double minval, maxval;
        Point minloc, maxloc;
        minMaxLoc(res, &minval, &maxval, &minloc, &maxloc);

        if (maxval > 0)
        {
            rectangle(ref, maxloc, Point(maxloc.x + tpl.cols, maxloc.y + tpl.rows), Scalar(0, 255, 0), 2);
            floodFill(res, maxloc, 0); //mark drawn blob
        }
        else
            break;
    }

    imshow("final", ref);
    waitKey(0);

    return 0;
}