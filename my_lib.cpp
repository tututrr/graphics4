#include "my_lib.h"

using namespace std;
using namespace cv;

void setPixel(int x, int y, Mat& image, Vec3b color) {
    if (x >= 0 && x < image.cols && y >= 0 && y < image.rows) {
        image.at<Vec3b>(y, x) = color;
    }
}

void fillBackground(Mat& image, Vec3b color) {
    for (int y = 0; y < image.rows; ++y) {
        for (int x = 0; x < image.cols; ++x) {
            setPixel(x, y, image, color);
        }
    }
}

void drawLine(int x1, int y1, int x2, int y2, Mat& image, Vec3b color) {
    int x = x1, y = y1;
    int dx = x2 - x1, dy = y2 - y1;
    int e, i;
    int ix = (dx > 0) - (dx < 0);  // -1, 0, or 1
    int iy = (dy > 0) - (dy < 0);  // -1, 0, or 1
    dx = abs(dx);
    dy = abs(dy);

    if (dx >= dy) {  // Основной алгоритм для наклонов с меньшим dx
        e = 2 * dy - dx;
        if (iy >= 0) {
            for (i = 0; i <= dx; i++) {
                setPixel(x, y, image, color);
                if (e >= 0) {
                    y += iy;
                    e -= 2 * dx;
                }
                x += ix;
                e += 2 * dy;
            }
        } else {
            for (i = 0; i <= dx; i++) {
                setPixel(x, y, image, color);
                if (e > 0) {
                    y += iy;
                    e -= 2 * dx;
                }
                x += ix;
                e += 2 * dy;
            }
        }
    } else {  // Основной алгоритм для наклонов с меньшим dy
        e = 2 * dx - dy;
        if (ix >= 0) {
            for (i = 0; i <= dy; i++) {
                setPixel(x, y, image, color);
                if (e >= 0) {
                    x += ix;
                    e -= 2 * dy;
                }
                y += iy;
                e += 2 * dx;
            }
        } else {
            for (i = 0; i <= dy; i++) {
                setPixel(x, y, image, color);
                if (e > 0) {
                    x += ix;
                    e -= 2 * dy;
                }
                y += iy;
                e += 2 * dx;
            }
        }
    }
}


MyPoint linePoint(MyPoint p0, MyPoint p1, double t)
{
    int x = static_cast<int>(p0.x * (1.0 - t) + p1.x * t);
    int y = static_cast<int>(p0.y * (1.0 - t) + p1.y * t);
    return { x, y };
}

MyPoint BezierQuadratic(MyPoint& p0, MyPoint& p1, MyPoint p2, double t)
{
    return linePoint(linePoint(p0, p1, t), linePoint(p1, p2, t), t);
}

MyPoint BezierCubic(MyPoint& p0, MyPoint& p1, MyPoint& p2, MyPoint& p3, double t)
{
    return linePoint(BezierQuadratic(p0, p1, p2, t), BezierQuadratic(p1, p2, p3, t), t);
}

double Dist(MyPoint p0) {
    return abs(p0.x) + abs(p0.y);
}

MyPoint GetDistV(MyPoint p0, MyPoint p1, MyPoint p2) {
    int x = p0.x - 2 * p1.x + p2.x;
    int y = p0.y - 2 * p1.y + p2.y;
    return { x, y };
}

double getN(MyPoint p0, MyPoint p1, MyPoint p2, MyPoint p3) {
    double H = max(Dist(GetDistV(p0, p1, p2)), Dist(GetDistV(p1, p2, p3)));
    return 1 + sqrt(3 * H);
}

void DrawBezierCubic(MyPoint& p0, MyPoint& p1, MyPoint& p2, MyPoint& p3, Mat& image, Vec3b color) {
    drawPolygon({ p0, p1, p2, p3 }, image, PINK);
    
    double t = 0;
    double step = 1 / getN(p0, p1, p2, p3);

    MyPoint prev;

    while (t <= 1 + step) {
        MyPoint point = BezierCubic(p0, p1, p2, p3, t);

        if (t != 0) {
            drawLine(prev.x, prev.y, point.x, point.y, image, color);
        }

        prev = point;
        t += step;
    }
}

void drawPolygon(const vector<MyPoint>& points, Mat& image, Vec3b borderColor) {
    for (int i = 0; i < points.size() - 1; ++i) {
        drawLine(points[i].x, points[i].y, points[i + 1].x, points[i + 1].y, image, borderColor);
    }
}

CLPointType Classify(double x1, double y1, double x2, double y2, double x, double y) {
    double ax = x2 - x1;
    double ay = y2 - y1;
    double bx = x - x1;
    double by = y - y1;
    double s = ax * by - bx * ay;
    if (s > 0) return LEFT;
    if (s < 0) return RIGHT;
    if ((ax * bx < 0) || (ay * by < 0)) 
        return BEHIND;
    if ((ax * ax + ay * ay) < (bx * bx + by * by))
        return BEHIND; 
    if (x1 == x && y1 == y) 
        return ORIGIN;
    if (x2 == x && y2 == y) 
        return DESTINATION;
    return BETWEEN;
}

int CyrusBeckClipLine(int& x1, int& y1, int& x2, int& y2, double& t1, double& t2, vector<MyPoint>& points, int n) {
    double t;
    int sx = x2 - x1, sy = y2 - y1;
    int nx, ny, denom, num, x1_new, y1_new, x2_new, y2_new;
    for (int i = 0; i < n; i++) {
        nx = points[(i + 1) % n].y - points[i].y; ny = points[i].x - points[(i + 1) % n].x; 
        denom = nx * sx + ny * sy; 
        num = nx * (x1 - points[i].x) + ny * (y1 - points[i].y);

        if (denom != 0) {
            t = -static_cast<double>(num) / denom;
            if (denom > 0) {
                if (t > t1) t1 = t;
            }
            else { 

                if (t < t2) t2 = t;
            }

        }
        else { if (Classify(points[i].x, points[i].y, points[(i + 1) % n].x, points[(i + 1) % n].y, x1, y1) == LEFT) return 0; }
    } 
    if (t1 <= t2) { 
        x1_new = x1 + t1 * (x2 - x1); y1_new = y1 + t1 * (y2 - y1);
        x2_new = x1 + t2 * (x2 - x1); y2_new = y1 + t2 * (y2 - y1);
        x1 = x1_new; y1 = y1_new; x2 = x2_new; y2 = y2_new;
        return 1;
    }
    return 0;
}

void drawCyrusBeckLine(MyPoint& p1, MyPoint& p2, vector<MyPoint>& points, Mat& image) {
    int x1 = p1.x;
    int y1 = p1.y;
    int x2 = p2.x;
    int y2 = p2.y;
    double t1 = 0;
    double t2 = 1;

    if (CyrusBeckClipLine(x1, y1, x2, y2, t1, t2, points, points.size() - 1)) {

        if (t1 != 0) {
            drawLine(p1.x, p1.y, x1, y1, image, PINK);
        }

        if (t2 != 1) {
            drawLine(x2, y2, p2.x, p2.y, image, PINK);
        }

        drawLine(x1, y1, x2, y2, image, CYAN);
    }
    else {
        drawLine(x1, y1, x2, y2, image, PINK);
    }
}
