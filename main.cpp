#include "my_lib.h" 

using namespace std;
using namespace cv;

void showImage(const Mat image) {
    imshow("Display window", image);
}

void saveImage(Mat* image, string name) {
    imwrite("/Users/hisoka/Downloads/graphics4/result/" + name, *image);
}

void Bezier() {
    Mat bezierImage = Mat::zeros({ 1000, 1000 }, CV_8UC3);
    
    fillBackground(bezierImage, BLACK);

    MyPoint p0 = { 10, 10 };
    MyPoint p1 = { 200, 100 };
    MyPoint p2 = { 400, 600 };
    MyPoint p3 = { 600, 200 };

    DrawBezierCubic(p0, p1, p2, p3, bezierImage, MAGENTA);

    MyPoint p4 = { 10, 600 };
    MyPoint p5 = { 200, 400 };
    MyPoint p6 = { 600, 400 };
    MyPoint p7 = { 800, 600 };

    DrawBezierCubic(p4, p5, p6, p7, bezierImage, MAGENTA);

    saveImage(&bezierImage, "bezier.png");
}

void CyrusBeck() {
    Mat cyrusBeckImage = Mat::zeros({ 1000, 1000 }, CV_8UC3);

    fillBackground(cyrusBeckImage, BLACK);

    vector<MyPoint> points = {
        {100, 200},
        {100, 500},
        {200, 650},
        {300, 700},
        {400, 600},
        {450, 400},
        {400, 200},
        {200, 150},
        {100, 200}
    };
    Segment s1 = { {50, 250}, {500, 600} }; // Пересекает в 2 точках
    Segment s2 = { {250, 950}, {300, 900} }; // Полностью вне многоугольника
    Segment s3 = { {250, 250}, {250, 350} }; // Полностью лежит внутри
    Segment s4 = { {50, 100}, {200, 300} }; // Пересекает в 1 точке 
    drawCyrusBeckLine(s1.p0, s1.p1, points, cyrusBeckImage);
    drawCyrusBeckLine(s2.p0, s2.p1, points, cyrusBeckImage);
    drawCyrusBeckLine(s3.p0, s3.p1, points, cyrusBeckImage);
    drawCyrusBeckLine(s4.p0, s4.p1, points, cyrusBeckImage);

    drawPolygon(points, cyrusBeckImage, MAGENTA);

    saveImage(&cyrusBeckImage, "cyrus_beck.png");
}

int main()
{
    Bezier();

    CyrusBeck();

    waitKey(0);
}