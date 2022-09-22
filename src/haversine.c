#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Straal van de aarde in km
#define R 6371
#define TO_RAD (3.1415926536 / 180)
double d_geo(double lat1, double lon1, double lat2, double lon2) {
    double dx, dy, dz;
    lon1 = lon1 - lon2;
    lon1 *= TO_RAD, lat1 *= TO_RAD, lat2 *= TO_RAD;

    dz = sin(lat1) - sin(lat2);
    dx = cos(lon1) * cos(lat1) - cos(lat2);
    dy = sin(lon1) * cos(lat1);
    return asin(sqrt(dx * dx + dy * dy + dz * dz) / 2) * 2 * R; // km
}

double extent(int rank) {
    double mapping[30] = { 100000, 50000, 10000, 1000, 750, 600, 500, 350, 200, 100, 75, 50, 10, 9, 7.5, 6, 4.5, 4, 2, 1, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1, 0.05, 0.01};
    return mapping[rank-1];
}
