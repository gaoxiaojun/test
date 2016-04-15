#include <stdio.h>
#include <math.h>

const double EARTH_RADIUS = 6378.137;

static double rad(double d)
{
    return d * M_PI / 180.0;
}

static double get_distance(double lat1, double lng1, double lat2,
        double lng2)
{
    double rad_lat1 = rad(lat1);
    double rad_lat2 = rad(lat2);
    double a = rad_lat1 - rad_lat2;
    double b = rad(lng1) - rad(lng2);
    double s = 2 * asin(sqrt(pow(sin(a/2),2)
                + cos(rad_lat1)*cos(rad_lat2)*pow(
                    sin(b/2),2)));
    s *= EARTH_RADIUS;
//    s = round(s*10000)/10000;
    return s;
}

int main()
{
    printf("EARTH_RADIUS=%lf\n", EARTH_RADIUS);
    printf("(0, 0, 90, 180)=%lf\n", get_distance(0, 0, 90, 180));
    printf("(0, 0, 0, 60)=%lf\n", get_distance(0, 0, 0, 60));
    printf("(0, 180, 0, -180)=%lf\n", get_distance(0, 180, 0, -180));
    printf("(60, 180, 0, -180)=%lf\n", get_distance(60, 180, 0, -180));
    printf("(60, 180, -60, -180)=%lf\n", get_distance(60, 180, -60, -180));
    printf("(90, 45, -90, 37)=%lf\n", get_distance(90, 45, -90, 37));
    return 0;
}
