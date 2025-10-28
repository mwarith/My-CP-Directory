//
// Created by Mohamed on 6/23/2024.
//
#include "bits/stdc++.h"
using namespace std;
#define ll long long
const int oo = 0X3F3F3F3F;
const ll OO = 0X3F3F3F3F3F3F3F3F, mod = 1e9 + 7;
int dx[] = {0, 0, 1, -1, 1, 1, -1, -1};
int dy[] = {1, -1, 0, 0, 1, -1, 1, -1};

const double PI = acos(-1);
#define EPS 1e-8

/*****************************   Angles   ****************************/

double toRad(double d) {
    return d * PI / 180.0;
}

double toDeg(double rad) {
    if (rad < 0)
        rad += 2 * PI;
    return rad * 180.0 / PI;
}

/*****************************   Triangles   ****************************/

/*
    A is a result of sin or cos
    we fix it if is not in the interval [-1, 1]
    mainly used with asin or acos
*/
double fixAngle(double A) {
    return A > 1 ? 1 : (A < -1 ? -1 : A);
}

double fix_360(double rad) {
    while (rad < 0)
        rad += 2 * PI;
    while (rad > 2 * PI)
        rad -= 2 * PI;
    return rad;
}

/*****************************   Triangle Areas   ****************************/
// Finding Area by the basic formula
double area(double b, double h) {
    return 0.5 * b * h;
}

// Finding Area by Heron's formula
// s = 0.5 * (a + b + c)
// Area = sqrt(s * (s - a) * (s - b) * (s - c))
double areaHeron(double a, double b, double c) {
    double s = 0.5 * (a + b + c);
    return sqrt(s * (s - a) * (s - b) * (s - c));
}

// Finding Area by making medians as sides
// Area of triangle  = 4 / 3 * area of medians triangle
double areaByMedians(double m1, double m2, double m3) {
    if (m1 <= 0 || m2 <= 0 || m3 <= 0) {
        return -1;
    }
    double mediansArea = areaHeron(m1, m2, m3);
    double triArea = 4.0 / 3.0 * mediansArea;
    if (mediansArea <= 0 || triArea <= 0)
        return -1;
    return triArea;
}

/*
    Uses the Law of Sines to calculate
    the length of the opposite side in a triangle.
    sin(A)/a = sin(B)/b = sin(C)/c
*/
double getSide_a_bAB(double b, double A, double B) {
    return (sin(A) * b) / sin(B);
}

//  have sides a, b and angle B and want angle A
//  sin(A)/a = sin(B)/b = sin(C)/c
double getAngle_A_abB(double a, double b, double B) {
    return asin(fixAngle((a * sin(B)) / b));
}

// have 3 sides and want any angle
// a^2 = b^2 + c^2 - 2 * b * c * cos(A)
double getAngle_A_abc(double a, double b, double c) {
    return acos(fixAngle((b * b + c * c - a * a) / (2 * b * c)));
}

/*******************************   Complex Numbers   ****************************/
// Using Complex Numbers to represent vectors and 2D points.
#define double long double
typedef complex<double> point;
#define X real()
#define Y imag()
#define EPS 1e-8

double angle(point a) {
    // Angle of vector (x, y), or angle of point (x, y) with origin
    return atan2(a.Y, a.X);
}

point vec(point a, point b) {
    // Vector from point a to point b
    return b - a; // from a to b
}

double length(point a) {
    return hypot(a.Y, a.X);
}

point normalize(point a) {
    return a / length(a);
}

// compare two double numbers
int cmp(double a, double b) {
    //   0 =, 1 >, -1 <
    return fabs(a - b) <= EPS ? 0 : a > b ? 1
                                          : -1;
}

// Cross and Dot product using complex
// Conj(a) * b = (dot product) + (cross product) * i
double dot(point a, point b) {
    return (conj(a) * b).X;
}

double cross(point a, point b) {
    return (conj(a) * b).Y;
}

bool same(point p1, point p2) {
    return dot(vec(p1, p2), vec(p1, p2)) < EPS;
}

/***************************** Lines ********************************/
/*
    check if point lies on a line or that 3 points are on the same line
    point C on line AB
    cross(line ab, line ac) = 0
*/
bool isCollinear(point a, point b, point c) {
    double x = cross((b - a), (c - a));
    return cmp(cross((b - a), (c - a)), 0) == 0;
}

// ray AB , point C
bool isPointOnRay(point a, point b, point c) {
    if (!isCollinear(a, b, c))
        return false;
    // dot > 0
    return cmp(dot((b - a), (c - a)), 0) == 1;
}

/*
    is Point C on segment AB
*/
bool isPointOnSegment(point a, point b, point c) {
    return isPointOnRay(a, b, c) && isPointOnRay(b, a, c);
    //    return cmp(length(a - b), length(a - c) + length(c - b)) == 0;
}

/******************* Distance from point C to Line AB *********************/
double distToLine(point a, point b, point c) {
    return fabs(cross(b - a, c - a) / length(a - b));
}

/******************* Distance from point C to segment AB ***********************/
double distToSegment(point p0, point p1, point p2, point &x) {
    // segment p0, p1 => point p2
    double d1, d2;
    point v1 = p1 - p0, v2 = p2 - p0;
    if ((d1 = dot(v1, v2)) <= 0) {
        x = p0;
        return length(p2 - p0);
    } else if ((d2 = dot(v1, v1)) <= 0) {
        x = p1;
        return length(p2 - p1);
    }
    double t = d1 / d2;
    x = (p0 + v1 * t);
    return length(p2 - (p0 + v1 * t));
}

/***********************   Intersecting Lines  ******************/

// get implicit equation for the line
void getImplicitEq(point p1, point p2, double &a, double &b, double &c) {
    // ax + by = c
    a = p1.Y - p2.Y;
    b = p2.X - p1.X;
    c = p2.X * p1.Y - p1.X * p2.Y;
}

/*
    a1 * x + b1 * y = c1
    a2 * x + b2 * y = c2
 */
point intersect(double a1, double b1, double c1, double a2, double b2, double c2) {
    point x;
    x.real((c1 * b2 - c2 * b1) / (a1 * b2 - a2 * b1));
    x.imag((a1 * c2 - a2 * c1) / (a1 * b2 - a2 * b1));
    return x;
}

// intersect segment ab with cd
bool intersectSegments(point a, point b, point c, point d, point &intersect) {
    double d1 = cross(a - b, d - c), d2 = cross(a - c, d - c), d3 = cross(a - b, a - c);
    if (fabs(d1) < EPS)
        return false;
    double t1 = d2 / d1, t2 = d3 / d1;
    intersect = a + (b - a) * t1;
    if (t1 < -EPS || t2 < -EPS || t2 > 1 + EPS)
        return false; // ab is ray, cd is segment
    return true;
}

/******************************* Circles ***********************************************/
/******************** Find Circle with 3 points representing it ************************/
pair<double, point> findCircle(point a, point b, point c) {
    // vectors ab, bc and thier perpendiculars
    // medians of the two chords ab, bc
    point m1 = (a + b) * (double)0.5, v1 = (b - a), pv1 = (v1.Y, -v1.X);
    point m2 = (c + b) * (double)0.5, v2 = (b - c), pv2 = (v2.Y, -v2.X);
    point end1 = m1 + pv1, end2 = m2 + pv2, center;
    intersectSegments(m1, end1, m2, end2, center);
    return {length(vec(a, center)), center};
}

/******************************* Circle Line Intersection *******************************/
/*
    Circle Line intersecton => line p0,  p1, center C
    no intersection     b*b-4*a*c  < 0
    one point           b*b-4*a*c == 0
    two points          b*b-4*a*c  > 0
    (p1-p0) * (p1-p0) * t * t   + 2 * (p1-p0) * (p0 - c0)* t  + (p0 - c0) * (p0 - c0) - r * r = 0
                        a * x * x  +                   b * x  +   c
 */
vector<point> circleLineIntersect(point p0, point p1, point c0, double r) {
    double a = dot(p1 - p0, p1 - p0),
           b = 2 * dot(p1 - p0, p0 - c0),
           c = dot(p0 - c0, p0 - c - r * r);
    double sq = b * b - 4 * a * c;
    vector<point> v;
    if (cmp(sq, 0) >= 0) {
        if (cmp(sq, 0) == 0)
            sq = 0;
        double t1 = (-b + sqrt(sq)) / (2 * a);
        double t2 = (-b - sqrt(sq)) / (2 * a);
        v.emplace_back(p0 + t1 * (p1 - p0));
        if (cmp(sq, 0) != 0)
            v.emplace_back(p0 + t2 * (p1 - p0));
    }
    return v;
}

/******************************* Circle Circle Intersection *******************************/
vector<point> circleCircleIntersect(point c1, double r1, point c2, double r2) {
    // Handle infinity case first: same center/radius and r > 0
    if (same(c1, c2) && cmp(r1, r2) == 0 && cmp(r1, 0) > 0)
        return vector<point>(3, c1);
    // infinity 2 same circles (not points)

    // Compute 2 intersection case and handle 0, 1, 2 cases
    double ang1 = angle(c2 - c1), ang2 = getAngle_A_abc(r2, r1, length(c2 - c1));
    if (::isnan(ang2)) // if r1 or d = 0 => nan in getAngle_A_abc (/0)
        ang2 = 0;      // fix corruption
    vector<point> v(1, polar(r1, ang1 + ang2) + c1);

    // if point NOT on the 2 circles = no intersection
    if (cmp(dot(v[0] - c1, v[0] - c1), r1 * r1) != 0 ||
        cmp(dot(v[0] - c2, v[0] - c2), r2 * r2) != 0)
        return vector<point>();
    v.push_back(polar(r1, ang1 - ang2) + c1);
    if (same(v[0], v[1])) // if same, then 1 intersection only
        v.pop_back();
    return v;
}

/*********************** MEC - Minimal Enclosing Circle ***************************/
/*
    pnts is points array
    cen = center, rad = radius

    // run this first
    ps = n, rs = 0;
    random_device rd;
    mt19937 g(rd());
    shuffle(pnts.begin(), pnts.end(), g);
    MEC();
//    random_shuffle(pnts.begin(), pnts.end());
*/
vector<point> pnts, r(3);
point cen;
double rad;
int ps, rs = 0;

void MEC() {
    if (ps == 0 && rs == 2) {
        cen = (r[0] + r[1]) * (double)0.5;
        rad = length(r[0] - cen);
    } else if (rs == 3) {
        pair<double, point> p = findCircle(r[0], r[1], r[2]);
        cen = p.second;
        rad = p.first;
    } else if (ps == 0) {
        cen = r[0]; // sometimes be garbage, but will not affect
        rad = 0;
    } else {
        ps--;
        MEC();
        if (length(pnts[ps] - cen) > rad) {
            r[rs++] = pnts[ps];
            MEC();
            rs--;
        }
        ps++;
    }
}
