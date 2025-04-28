#include <opencv2/highgui.hpp>
#include <Windows.h>

using namespace cv;

int main(int argc, char* argv[]) {
    FreeConsole();
    std::string s = argv[1];
    size_t pos = s.find_last_of("\\");
    s = s.substr(pos + 1);
    Mat input = imread(argv[1]);
    namedWindow(s, WINDOW_AUTOSIZE);
    imshow(s, input);
    while (getWindowProperty(s, WND_PROP_VISIBLE)) {
        char c = waitKeyEx(50);
        if (c == ' ') break;
    }
    destroyWindow(s);
    return 0;
}
