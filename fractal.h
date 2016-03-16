#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <complex.h>

typedef struct _RGB_COLOR {
    double r;
    double g;
    double b;
} RGB_COLOR, *P_RGB_COLOR;

typedef struct _HSV_COLOR {
    double h;
    double s;
    double v;
} HSV_COLOR, *P_HSV_COLOR;

void draw_mono_mandelbrot(unsigned int width, unsigned int height,
                          double complex point, double step,
                          unsigned int maxiter, HDC hdc);
void draw_mandelbrot(unsigned int width, unsigned int height,
                     double complex point, double step,
                     unsigned int maxiter, HDC hdc);
void draw_mandelbrot2(unsigned int width, unsigned int height,
                      double complex point, double step,
                      unsigned int maxiter, HDC hdc);
void draw_mandelbrot3(unsigned int width, unsigned int height,
                      double complex point, double step,
                      unsigned int maxiter, HDC hdc);
void draw_mandelbrot4(unsigned int width, unsigned int height,
                      double complex point, double step,
                      unsigned int maxiter, HDC hdc);
void draw_mandelbrot5(unsigned int width, unsigned int height,
                      double complex point, double step,
                      unsigned int maxiter, HDC hdc);
void draw_mandelbrot6(unsigned int width, unsigned int height,
                      double complex point, double step,
                      unsigned int maxiter, HDC hdc);
double mandelbrot(double complex point, unsigned int max_iterations);
void SaveDc2Image(HDC hdc, char filename[]);
COLORREF get_smooth_color( double complex point, double step, unsigned int maxiter);
COLORREF get_smooth_color2( double complex point, double step, unsigned int maxiter);
RGB_COLOR hsv2rgb(double h, double s, double v);
RGB_COLOR palette(double d);
RGB_COLOR interpolate(RGB_COLOR a, RGB_COLOR b, double t);
RGB_COLOR log_color(double d);
COLORREF convert_color(RGB_COLOR m);