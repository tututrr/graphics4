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

    vector<int> xCoords = {100, 100, 200, 300, 400, 450, 400, 200, 100};
    vector<int> yCoords = {200, 500, 650, 700, 600, 400, 200, 150, 200};
    Polygon poly(xCoords, yCoords);

    drawLine(50, 600, 500, 600, cyrusBeckImage, MAGENTA); 
    drawLine(250, 950, 300, 900, cyrusBeckImage, MAGENTA);
    drawLine(250, 250, 250, 350, cyrusBeckImage, MAGENTA);
    drawLine(50, 100, 200, 300, cyrusBeckImage, MAGENTA);

    drawCyrusBeckClippedLine(MyPoint(50, 600), MyPoint(500, 600), poly, cyrusBeckImage);// Пересекает в 2 точках
    drawCyrusBeckClippedLine(MyPoint(250, 950), MyPoint(300, 900), poly, cyrusBeckImage);// Полностью вне многоугольника
    drawCyrusBeckClippedLine(MyPoint(250, 250), MyPoint(250, 350), poly, cyrusBeckImage);// Полностью лежит внутри
    drawCyrusBeckClippedLine(MyPoint(50, 100), MyPoint(200, 300), poly, cyrusBeckImage);// Пересекает в 1 точке 
    
    drawPolygon_v2(poly, cyrusBeckImage, MAGENTA);
    saveImage(&cyrusBeckImage, "cyrus_beck.png");

}

int main()
{
    Bezier();

    CyrusBeck();

    waitKey(0);
}