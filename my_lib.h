#ifndef LIB_H 
#define LIB_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <limits>

using namespace cv;
using namespace std;

enum CLPointType { LEFT, RIGHT, BEYOND, BEHIND, BETWEEN, ORIGIN, DESTINATION };

class MyPoint {
public:
    double x;
    double y; 

    MyPoint(double x, double y);

    MyPoint add(const MyPoint& p) const;

    MyPoint sub(const MyPoint& p) const;

    MyPoint multiply(double scalar) const;

    double getX() const;

    double getY() const;
};

class Polygon {
public:
    Polygon(const std::vector<int>& xCoords, const std::vector<int>& yCoords);

    // Method to get the number of vertices
    int getVertexNum() const;

    MyPoint getVertexCoords(int index) const;

private:
    std::vector<int> xCoords; 
    std::vector<int> yCoords;  
};

Polygon switchOrientation(const Polygon& polygon);

bool isClockWiseOriented(const Polygon& polygon);

const Vec3b BLACK  = { 0, 0, 0 };
const Vec3b WHITE  = { 255, 255, 255 };   
const Vec3b BLUE   = { 255, 0, 0 };
const Vec3b MAGENTA   = { 255, 0, 255 }; 
const Vec3b CYAN      = { 255, 255, 0 };
const Vec3b PURPLE    = { 128, 0, 128 };
const Vec3b PINK      = { 203, 192, 255 };
const Vec3b SILVER    = { 192, 192, 192 };


void setPixel(int x, int y, Mat& image, Vec3b color);

void fillBackground(Mat& image, Vec3b color);

bool isConvex(const Polygon& polygon);

void DrawBezierCubic(MyPoint& p0, MyPoint& p1, MyPoint& p2, MyPoint& p3, Mat& image, Vec3b color);

void drawPolygon_v2(const Polygon& poly, Mat& image, Vec3b borderColor);

void drawPolygon(const vector<MyPoint>& points, Mat& image, Vec3b borderColor);

void drawLine(int x1, int y1, int x2, int y2, Mat& image, Vec3b color);

MyPoint linePoint(MyPoint p0, MyPoint p1, double t);

void drawCyrusBeckClippedLine(MyPoint p1, MyPoint p2, Polygon polygon, Mat& image);

CLPointType Classify(double x1, double y1, double x2, double y2, double x, double y);

#endif