#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

bool use_mask;
cv::Mat img;
cv::Mat templ;
cv::Mat mask;
cv::Mat result;
const char* image_window = "Source Image";
const char* result_window = "Result window";
int match_method;
int max_Trackbar = 5;
std::string card_name;

void MatchingMethod(int, void*);

int main(int argc, char** argv)
{
    if (argc < 3) {
        std::cout << "Not enough parameters" << std::endl;
        std::cout << "Usage:\n" << argv[0] << " <image_name> <template_name> [<mask_name>]" << std::endl;
        return -1;
    }
    img = cv::imread(argv[1], cv::IMREAD_COLOR);
    templ = cv::imread(argv[2], cv::IMREAD_COLOR);
    //cvtColor(img, img, COLOR_BGR2GRAY);  // to grayscale
    //cvtColor(templ, templ, COLOR_BGR2GRAY);

    if (argc > 3) {
        use_mask = true;
        mask = cv::imread(argv[3], cv::IMREAD_COLOR);
    }
    if (img.empty() || templ.empty() || (use_mask && mask.empty())) {
        std::cout << "Can't read one of the images" << std::endl;
        return EXIT_FAILURE;
    }
    card_name = std::string(argv[2]).substr(0, std::string(argv[2]).size() - 4); // get card name from file and remove *.png
    cv::namedWindow(image_window, cv::WINDOW_AUTOSIZE);
    cv::namedWindow(result_window, cv::WINDOW_AUTOSIZE);
    const char* trackbar_label = "Method: \n 0: SQDIFF \n 1: SQDIFF NORMED \n 2: TM CCORR \n 3: TM CCORR NORMED \n 4: TM COEFF \n 5: TM COEFF NORMED";
    cv::createTrackbar(trackbar_label, image_window, &match_method, max_Trackbar, MatchingMethod);
    MatchingMethod(0, 0);
    cv::waitKey(0);
    return EXIT_SUCCESS;
}

void MatchingMethod(int, void*)
{
    cv::Mat img_display;
    img.copyTo(img_display);
    int result_cols = img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;
    result.create(result_rows, result_cols, CV_32FC1);
    bool method_accepts_mask = (cv::TM_SQDIFF == match_method || match_method == cv::TM_CCORR_NORMED);
    if (use_mask && method_accepts_mask) {
        matchTemplate(img, templ, result, match_method, mask);
    } else {
        matchTemplate(img, templ, result, match_method);
    }
    cv::Scalar testt = cv::sum(result);
    normalize(result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());
    double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;
    cv::Point matchLoc;
    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());
    if (match_method == cv::TM_SQDIFF || match_method == cv::TM_SQDIFF_NORMED) {
        matchLoc = minLoc;
    } else {
        matchLoc = maxLoc;
    }
    rectangle(img_display, matchLoc, cv::Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), cv::Scalar(0, 125, 255), 2, 8, 0);
    cv::putText(img_display, card_name, cv::Point(matchLoc.x, matchLoc.y - 10), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 255, 0), 2, false);
    rectangle(result, matchLoc, cv::Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), cv::Scalar(125,125,125,0), 2, 8, 0);
    imshow(image_window, img_display);
    imshow(result_window, result);
    return;
}