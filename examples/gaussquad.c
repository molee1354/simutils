/**
 * @file gaussquad.c
 * @brief Computing single integrals using the Gauss quadratures method, with
 * the weights and Gauss points calculated using the Gauss-Legendre method.
 *
 * The example code is based off of code in the book "Numerical Recipes in C"
 * 2nd Edition, Chapter 4 Section 4.5.
 *
 * This code is mainly to show an example use-case, and is not meant to be 
 * of practical computational use!
 */

#include <math.h>
#include <simutil/vector.h>
#define EPS (3.0e-11)

static void gausslegendre(double x1, double x2, vector(double) x,
                          vector(double) w, int n) {
    const int m = (n + 1) / 2;
    const double xm = .5 * (x2 + x1);
    const double xl = .5 * (x2 - x1);

    double z1, z, pp, p3, p2, p1; // loop variables 
    for (int i = 1; i <= m; i++) {
        z = cos(M_PI * (i - .25) / (n + .5));
        do {
            p1 = 1.0;
            p2 = 0.0;
            for (int j = 1; j <= n; j++) {
                p3 = p2;
                p2 = p1;
                p1 = ((2.0 * j - 1.0) * z * p2 - (j - 1.0) * p3) / j;
            }
            pp = n * (z * p1 - p2) / (z * z - 1.0);
            z1 = z;
            z = z1 - p1 / pp;
            // printf("|z - z1| : %g\n", fabs(z-z1));
        } while (fabs(z - z1) > EPS);
        x[i] = xm - xl * z;
        x[n + 1 - i] = xm + xl * z;
        w[i] = 2.0 * xl / ((1.0 - z * z) * pp * pp);
        w[n + 1 - i] = w[i];
    }
}

static double gaussquad(double (*func)(double), double lower, double upper) {
    // The upper and lower bounds to convert to
    const double x1 = -1.;
    const double x2 = 1.;

    // number of gauss points
    const int n = 2;

    vector(double) x = new_vector(double, n);
    vector(double) w = new_vector(double, n);

    // computing w and x using the gauss-legendre method
    gausslegendre(x1, x2, x, w, n);

    printf("x : ");
    print_vector(x);
    printf("w : ");
    print_vector(w);

    const double xm = 0.5 * (upper + lower);
    const double xr = 0.5 * (upper - lower);
    double s = 0.0;
    for (int j = 1; j <= (int)(n / 2); j++) {
        double dx = xr * x[j];
        s += w[j] * ((*func)(xm + dx) + (*func)(xm - dx));
    }
    free_vector(x);
    free_vector(w);
    return s *= xr;
}

static double myfun(double x) { return x*x*x*x + 2*x*x*x + 3*x*x; }

int test_gaussquad(FILE* fp) {
    double lower = -1.;
    double upper = 1.;
    double result = gaussquad(myfun, lower, upper);
    fprintf(fp,
            "result from gaussquad integration of f(x) from x "
            "in[%.1f,%.1f] :\n",
            lower, upper);
    fprintf(fp, " %.5f\n", result);

    return 0;
}
