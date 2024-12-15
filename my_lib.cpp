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

MyPoint::MyPoint(double x, double y) : x(x), y(y) {}

MyPoint MyPoint::add(const MyPoint& p) const {
    return MyPoint(x + p.x, y + p.y);
}

MyPoint MyPoint::sub(const MyPoint& p) const {
    return MyPoint(x - p.x, y - p.y);
}

MyPoint MyPoint::multiply(double scalar) const {
    return MyPoint(scalar * x, scalar * y);
}

double MyPoint::getX() const {
    return x;
}

double MyPoint::getY() const {
    return y;
}

Polygon::Polygon(const std::vector<int>& xCoords, const std::vector<int>& yCoords)
    : xCoords(xCoords), yCoords(yCoords) {}

int Polygon::getVertexNum() const {
    return xCoords.size();
}

MyPoint Polygon::getVertexCoords(int index) const {
    return MyPoint(xCoords[index], yCoords[index]);  
}

Polygon switchOrientation(const Polygon& polygon) {
    int n = polygon.getVertexNum();

    std::vector<int> xCoords(n);
    std::vector<int> yCoords(n);

    for (int i = 0; i < n; ++i) {
        auto coords = polygon.getVertexCoords(n - 1 - i);
        xCoords[i] = coords.getX();
        yCoords[i] = coords.getY();
    }

    return Polygon(xCoords, yCoords);
}

bool isClockWiseOriented(const Polygon& polygon) {
    int n = polygon.getVertexNum();
    bool hasPositiveRotation = false;

    for (int i = 0; i < n; ++i) {
        auto a = polygon.getVertexCoords(i);
        auto b = polygon.getVertexCoords((i + 1) % n);
        auto c = polygon.getVertexCoords((i + 2) % n);

        double abx = b.getX() - a.getX();
        double aby = b.getY() - a.getY();
        double bcx = c.getX() - b.getX();
        double bcy = c.getY() - b.getY();

        double product = abx * bcy - aby * bcx; 

        if (product > 0)
            hasPositiveRotation = true;

        if (hasPositiveRotation)
            return false;
    }

    return true;
}

bool isConvex(const Polygon& polygon) {
    int n = polygon.getVertexNum(); 
    if (n < 3) return false; 

    int res = 0; 

    for (int i = 0; i < n; ++i) {

        MyPoint p0 = polygon.getVertexCoords(i);
        MyPoint p1 = polygon.getVertexCoords((i + 1) % n);
        MyPoint p2 = polygon.getVertexCoords((i + 2) % n);

        double dx1 = p1.x - p0.x;
        double dy1 = p1.y - p0.y;
        double dx2 = p2.x - p1.x;
        double dy2 = p2.y - p1.y;

        double cross = dx1 * dy2 - dy1 * dx2;

        if (res == 0) {
            res = (cross > 0) ? 1 : (cross < 0) ? -1 : 0; 
        } else {
            if (res * (cross > 0 ? 1 : (cross < 0 ? -1 : 0)) < 0) {
                return false;
            }
        }
    }

    return true; 
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
    return {static_cast<double>(x), static_cast<double>(y)}; 
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
    return {static_cast<double>(x), static_cast<double>(y)};
}

double getN(MyPoint p0, MyPoint p1, MyPoint p2, MyPoint p3) {
    double H = max(Dist(GetDistV(p0, p1, p2)), Dist(GetDistV(p1, p2, p3)));
    return 1 + sqrt(3 * H);
}

void DrawBezierCubic(MyPoint& p0, MyPoint& p1, MyPoint& p2, MyPoint& p3, Mat& image, Vec3b color) {
    drawPolygon({ p0, p1, p2, p3 }, image, PINK);
    
    double t = 0;
    double step = 1 / getN(p0, p1, p2, p3);

    MyPoint prev(0.0, 0.0);

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

void drawPolygon_v2(const Polygon& poly, Mat& image, Vec3b borderColor) {
    int vertexNum = poly.getVertexNum();

    if (vertexNum == 0)
        return;


    if (vertexNum == 1) {
        MyPoint coords = poly.getVertexCoords(0); 
        setPixel(coords.x, coords.y, image, borderColor); 
        return;
    }

    if (vertexNum == 2) {
        MyPoint coords1 = poly.getVertexCoords(0);
        MyPoint coords2 = poly.getVertexCoords(1);
        drawLine(coords1.x, coords1.y, coords2.x, coords2.y, image, borderColor);
        return;
    }

    MyPoint coordsPrev = poly.getVertexCoords(0); 
    MyPoint coordsNext(0, 0);
    for (int i = 1; i < vertexNum; ++i) {
        coordsNext = poly.getVertexCoords(i);
        drawLine(coordsPrev.x, coordsPrev.y, coordsNext.x, coordsNext.y, image, borderColor);
        coordsPrev = coordsNext;
    }

    coordsNext = poly.getVertexCoords(0);
    drawLine(coordsPrev.x, coordsPrev.y, coordsNext.x, coordsNext.y, image, borderColor);
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

void drawCyrusBeckClippedLine(MyPoint p1, MyPoint p2, Polygon polygon, Mat& image) {
    if (!isClockWiseOriented(polygon)) {
        polygon = switchOrientation(polygon); 
    }

    if (!isConvex(polygon)) {
        throw std::invalid_argument("Polygon must be convex");
    }

    int n = polygon.getVertexNum();
    double t1 = 0, t2 = 1, t;
    double sx = p2.getX() - p1.getX(), sy = p2.getY() - p1.getY();
    for (int i = 0; i < n; ++i) {
        MyPoint v1 = polygon.getVertexCoords(i);
        MyPoint v2 = polygon.getVertexCoords((i + 1) % n);

        double nx = v2.getY() - v1.getY();
        double ny = v1.getX() - v2.getX();
        double denom = nx * sx + ny * sy;
        double num = nx * (p1.getX() - v1.getX()) + ny * (p1.getY() - v1.getY());

        if (denom != 0) {
            t = -num / denom;
            if (denom > 0) {
                if (t > t1)
                    t1 = t;
            }
            else {
                if (t < t2)
                    t2 = t;
            }
    } else {
        if (Classify(v1.getX(), v1.getY(), v2.getX(), v2.getY(),
            p1.getX(), p1.getY()) == CLPointType::LEFT) {
            return;
            }
        }

        if (t1 > t2)
            return;
    }

    if (t1 <= t2) {
        MyPoint p1Cut = p1.add(p2.sub(p1).multiply(t1));
        MyPoint p2Cut = p1.add(p2.sub(p1).multiply(t2));
        drawLine(static_cast<int>(p1Cut.getX()), static_cast<int>(p1Cut.getY()),
         static_cast<int>(p2Cut.getX()), static_cast<int>(p2Cut.getY()), image, CYAN);
    }
}
