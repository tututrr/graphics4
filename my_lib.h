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

struct MyPoint {
	int x;
	int y;
};

struct Segment {
	MyPoint p0;
	MyPoint p1;
};

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

void DrawBezierCubic(MyPoint& p0, MyPoint& p1, MyPoint& p2, MyPoint& p3, Mat& image, Vec3b color);

void drawPolygon(const vector<MyPoint>& points, Mat& image, Vec3b borderColor);

void drawLine(int x1, int y1, int x2, int y2, Mat& image, Vec3b color);

void drawCyrusBeckLine(MyPoint& p1, MyPoint& p2, vector<MyPoint>& points, Mat& image);

#endif