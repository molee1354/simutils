/**
 * @file gaussj.c
 * @brief Finding the solution of a system of linear equations using
 * Gauss-Jordan elimination.
 *
 * The example code is based off of code in the book "Numerical Recipes in C"
 * 2nd Edition, Chapter 2 Section 2.1.
 *
 * This code is mainly to show an example use-case, and is not meant to be 
 * of practical computational use!
 */

#include <math.h>
#include <simutil/matrix.h>
#include <simutil/vector.h>

#define SWAP(a, b)                                                             \
    {                                                                          \
        temp = (a);                                                            \
        (a) = (b);                                                             \
        (b) = temp;                                                            \
    }
static int gaussj(matrix(double) a, int n, matrix(double) b, int m) {
    int i, icol, irow, j, k, l, ll;
    double big, dumb, pivinv, temp;

    vector(int) indxc = new_vector(int, n);
    vector(int) indxr = new_vector(int, n);
    vector(int) ipiv = new_vector(int, n);

    for (i = 1; i <= n; i++) {
        big = 0.0;
        for (j = 1; j <= n; j++)
            if (ipiv[j] != 1)
                for (k = 1; k <= n; k++) {
                    if (ipiv[k] == 0) {
                        if (fabs(a[j][k]) >= big) {
                            big = fabs(a[j][k]);
                            irow = j;
                            icol = k;
                        }
                    } else if (ipiv[k] > 1)
                        perror("SINGULAR MATRIX! (1)\n");
                }
        ++(ipiv[icol]);
        if (irow != icol) {
            for (l = 1; l <= n; l++)
                SWAP(a[irow][l], a[icol][l]);
            for (l = 1; l <= m; l++)
                SWAP(a[irow][l], a[icol][l]);
        }
        indxr[i] = irow;
        indxc[i] = icol;
        if (a[icol][icol] == 0.0)
            perror("SINGULAR MATRIX! (2)\n");
        pivinv = 1.0 / a[icol][icol];
        a[icol][icol] = 1.0;
        for (l = 1; l <= n; l++)
            a[icol][l] *= pivinv;
        for (l = 1; l <= m; l++)
            b[icol][l] *= pivinv;
        for (ll = 1; ll <= n; ll++)
            if (ll != icol) {
                dumb = a[ll][icol];
                a[ll][icol] = 0.0;
                for (l = 1; l <= n; l++)
                    a[ll][l] -= a[icol][l] * dumb;
                for (l = 1; l <= m; l++)
                    b[ll][l] -= b[icol][l] * dumb;
            }
    }
    for (l = n; l >= 1; l--) {
        if (indxr[l] != indxc[l])
            for (k = 1; k <= n; k++)
                SWAP(a[k][indxr[l]], a[k][indxc[l]]);
    }
    free_vector(ipiv);
    free_vector(indxr);
    free_vector(indxc);
    return 0;
}

int test_gaussj(FILE* fp) {
    const int n = 2;
    const int m = 1;

    double base_a[2][2] = {
        {2, 3},
        {1, 1},
    };
    double base_b[2][1] = {
        {5},
        {2},
    };

    matrix(double) a = new_matrix(double, n, n);
    matrix(double) b = new_matrix(double, m, n);
    FROM_MATRIX(base_a, a, n, n);
    FROM_MATRIX(base_b, b, m, n);
    fprint_matrix(fp, a);
    fprint_matrix(fp, b);
    int result = gaussj(a, n, b, m);
    puts("\nanswer:");
    fprint_matrix(fp, a);
    fprint_matrix(fp, b);
    free_matrix(a);
    free_matrix(b);
    return result;
}
