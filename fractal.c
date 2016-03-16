#include "fractal.h"
#include <pthread.h>
#include <signal.h>

/**
 * @brief Draw graphics on the hdc
 * @details Draw a mono graphics on the hdc
 *
 * @param width   [description]
 * @param height  [description]
 * @param point   [description]
 * @param step    [description]
 * @param maxiter [description]
 * @param hdc     [description]
 */
void draw_mono_mandelbrot(unsigned int width, unsigned int height,
                          double complex point, double step,
                          unsigned int maxiter, HDC hdc)
{
    long double complex calculate = 0;
    unsigned int iterations_time = 0;
    double complex temp = 0;
    double complex point_temp;

    point = creal(point) - (width / 2) * step +
            (cimag(point) + (height / 2) * step) * I;
    for (int y = 0; y < height; y ++) {
        for (int x = 0; x < width; x ++) {
            complex calculate = 0;
            iterations_time = 0;
            point_temp = point + temp;
            do {
                calculate = calculate * calculate + point_temp;
                if (creall(calculate) * creall(calculate) + cimagl(calculate) * cimagl(calculate) >= 4.)
                    break;
                iterations_time ++;
            } while (iterations_time <= maxiter);

            if (iterations_time < maxiter) {
                SetPixel(hdc, x, y, RGB(255, 255, 255));
            } else {
                SetPixel(hdc, x, y, RGB(0, 0, 0));
            }
            temp += step;
        }
        temp = (cimag(temp) - step) * I;
        // printf("%d\n", y);
    }
}

/**
 * @brief Draw graphics on the hdc
 * @details Draw mandelbrot without smooth on the hdc
 *
 *  ----------->
 *  ----------->
 *  ----------->
 *  ----------->
 *  
 * @param width   [description]
 * @param height  [description]
 * @param point   [description]
 * @param step    [description]
 * @param maxiter [description]
 * @param hdc     [description]
 */
void draw_mandelbrot(unsigned int width, unsigned int height,
                     double complex point, double step,
                     unsigned int maxiter, HDC hdc)
{
    double complex temp = 0;
    point = creal(point) - (width / 2) * step +
            (cimag(point) + (height / 2) * step) * I;
    for (int y = 0; y < height; y ++) {
        for (int x = 0; x < width; x ++) {
            SetPixel(hdc, x, y, convert_color(log_color(mandelbrot(point + temp, maxiter))));
            temp += step;
        }
        temp = (cimag(temp) - step) * I;
        // printf("%d\n", y);
    }
}

/**
 * @brief Draw graphics on the hdc
 * @details Draw mandelbrot without smooth on the hdc
 *
 *    |------------->|
 *    |              |
 *    |              |
 *    |              |
 *    |              |
 *    v------------->v
 *
 * @param width   [description]
 * @param height  [description]
 * @param point   [description]
 * @param step    [description]
 * @param maxiter [description]
 * @param hdc     [description]
 */
void draw_mandelbrot2(unsigned int width, unsigned int height,
                      double complex point, double step,
                      unsigned int maxiter, HDC hdc)
{
    double dx = 0;
    double dy = 0;
    for (dx = 0, dy = 0; dx <= width / 2; dx ++) {
        pthread_testcancel();
        // printf("%d%%\n", (int)(dx * 200.0/width));
        for (int x = -dx, y = -dy; y <= dy + 1; y ++) {
            SetPixel(hdc, width / 2 + x, height / 2 + y,
                     convert_color(log_color(mandelbrot(point + x * step - y * step * I, maxiter))));
        }
        for (int x = dx + 1, y = -dy; y <= dy + 1; y ++) {
            SetPixel(hdc, width / 2 + x, height / 2 + y,
                     convert_color(log_color(mandelbrot(point + x * step - y * step * I, maxiter))));
        }
        if (dy <= height / 2) {
            for (int x = -dx, y = -dy; x <= dx + 1; x ++) {
                SetPixel(hdc, width / 2 + x, height / 2 + y,
                         convert_color(log_color(mandelbrot(point + x * step - y * step * I, maxiter))));
            }
            for (int x = -dx, y = dy + 1; x <= dx + 1; x ++) {
                SetPixel(hdc, width / 2 + x, height / 2 + y,
                         convert_color(log_color(mandelbrot(point + x * step - y * step * I, maxiter))));
            }
            dy ++;
        }
    }
}

/**
 * @brief Draw graphics on the hdc
 * @details Draw mandelbrot with smooth x2 on the hdc
 *
 *    |------------->|
 *    |              |
 *    |              |
 *    |              |
 *    |              |
 *    v------------->v
 *
 * @param width   [description]
 * @param height  [description]
 * @param point   [description]
 * @param step    [description]
 * @param maxiter [description]
 * @param hdc     [description]
 */
void draw_mandelbrot3(unsigned int width, unsigned int height,
                      double complex point, double step,
                      unsigned int maxiter, HDC hdc)
{
    double dx = 0;
    double dy = 0;
    for (dx = 0, dy = 0; dx <= width / 2; dx ++) {
        for (int x = -dx, y = -dy; y <= dy + 1; y ++) {
            SetPixel(hdc, width / 2 + x, height / 2 + y,
                     get_smooth_color2(point + x * step - y * step * I, step, maxiter));
        }
        for (int x = dx + 1, y = -dy; y <= dy + 1; y ++) {
            SetPixel(hdc, width / 2 + x, height / 2 + y,
                     get_smooth_color2(point + x * step - y * step * I, step, maxiter));
        }
        if (dy <= height / 2) {
            for (int x = -dx, y = -dy; x <= dx + 1; x ++) {
                SetPixel(hdc, width / 2 + x, height / 2 + y,
                         get_smooth_color2(point + x * step - y * step * I, step, maxiter));
            }
            for (int x = -dx, y = dy + 1; x <= dx + 1; x ++) {
                SetPixel(hdc, width / 2 + x, height / 2 + y,
                         get_smooth_color2(point + x * step - y * step * I, step, maxiter));
            }
            dy ++;
        }
    }
}

/**
 * @brief Draw graphics on the hdc
 * @details Draw mandelbrot with smooth x4 on the hdc
 *
 *    |------------->|
 *    |              |
 *    |              |
 *    |              |
 *    |              |
 *    v------------->v
 *
 * @param width   [description]
 * @param height  [description]
 * @param point   [description]
 * @param step    [description]
 * @param maxiter [description]
 * @param hdc     [description]
 */
void draw_mandelbrot4(unsigned int width, unsigned int height,
                      double complex point, double step,
                      unsigned int maxiter, HDC hdc)
{
    double dx = 0;
    double dy = 0;
    for (dx = 0, dy = 0; dx <= width / 2; dx ++) {
        for (int x = -dx, y = -dy; y <= dy + 1; y ++) {
            SetPixel(hdc, width / 2 + x, height / 2 + y,
                     get_smooth_color(point + x * step - y * step * I, step, maxiter));
        }
        for (int x = dx + 1, y = -dy; y <= dy + 1; y ++) {
            SetPixel(hdc, width / 2 + x, height / 2 + y,
                     get_smooth_color(point + x * step - y * step * I, step, maxiter));
        }
        if (dy <= height / 2) {
            for (int x = -dx, y = -dy; x <= dx + 1; x ++) {
                SetPixel(hdc, width / 2 + x, height / 2 + y,
                         get_smooth_color(point + x * step - y * step * I, step, maxiter));
            }
            for (int x = -dx, y = dy + 1; x <= dx + 1; x ++) {
                SetPixel(hdc, width / 2 + x, height / 2 + y,
                         get_smooth_color(point + x * step - y * step * I, step, maxiter));
            }
            dy ++;
        }
    }
}

/**
 * @brief Draw graphics on the hdc
 * @details Draw mandelbrot with smooth x2 on the hdc
 *
 *    ^------------->|
 *    |              |
 *    |              |
 *    |              |
 *    |              |
 *    |<-------------v
 *
 * @param width   [description]
 * @param height  [description]
 * @param point   [description]
 * @param step    [description]
 * @param maxiter [description]
 * @param hdc     [description]
 */
void draw_mandelbrot5(unsigned int width, unsigned int height,
                      double complex point, double step,
                      unsigned int maxiter, HDC hdc)
{
    int half_width = width / 2;
    int half_height = height / 2;
    for (int i = 0; i <= half_width; i ++) {
        if (i <= half_height) {
            for (int j = -i; j <= i + 1; j ++) {
                /* 垂直扫描 */
                SetPixel(hdc, half_width - i, half_height - (j - 1),
                         get_smooth_color2(point - i * step + (j - 1) * step * I, step, maxiter));
                SetPixel(hdc, half_width + (i + 1), half_height + j,
                         get_smooth_color2(point + (i + 1) * step - j * step * I, step, maxiter));
                /* 水平扫描 */
                SetPixel(hdc, half_width + j, half_height - i,
                         get_smooth_color2(point + j * step + i * step * I, step, maxiter));
                SetPixel(hdc, half_width - (j - 1), half_height + (i + 1),
                         get_smooth_color2(point - (j - 1) * step - (i + 1) * step * I, step, maxiter));
            }
        } else {
            for (int j = -half_height; j <= half_height + 1; j ++) {
                /* 垂直扫描 */
                SetPixel(hdc, half_width - i, half_height - (j - 1),
                         get_smooth_color2(point - i * step + (j - 1) * step * I, step, maxiter));
                SetPixel(hdc, half_width + (i + 1), half_height + j,
                         get_smooth_color2(point + (i + 1) * step - j * step * I, step, maxiter));
            }
        }
    }
}

/**
 * @brief Draw graphics on the hdc
 * @details Draw mandelbrot with smooth x4 on the hdc
 *
 *    ^------------->|
 *    |              |
 *    |              |
 *    |              |
 *    |              |
 *    |<-------------v
 *
 * @param width   [description]
 * @param height  [description]
 * @param point   [description]
 * @param step    [description]
 * @param maxiter [description]
 * @param hdc     [description]
 */
void draw_mandelbrot6(unsigned int width, unsigned int height,
                      double complex point, double step,
                      unsigned int maxiter, HDC hdc)
{
    int half_width = width / 2;
    int half_height = height / 2;
    for (int i = 0; i <= half_width; i ++) {
        if (i <= half_height) {
            for (int j = -i; j <= i + 1; j ++) {
                /* 垂直扫描 */
                SetPixel(hdc, half_width - i, half_height - (j - 1),
                         get_smooth_color(point - i * step + (j - 1) * step * I, step, maxiter));
                SetPixel(hdc, half_width + (i + 1), half_height + j,
                         get_smooth_color(point + (i + 1) * step - j * step * I, step, maxiter));
                /* 水平扫描 */
                SetPixel(hdc, half_width + j, half_height - i,
                         get_smooth_color(point + j * step + i * step * I, step, maxiter));
                SetPixel(hdc, half_width - (j - 1), half_height + (i + 1),
                         get_smooth_color(point - (j - 1) * step - (i + 1) * step * I, step, maxiter));
            }
        } else {
            for (int j = -half_height; j <= half_height + 1; j ++) {
                /* 垂直扫描 */
                SetPixel(hdc, half_width - i, half_height - (j - 1),
                         get_smooth_color(point - i * step + (j - 1) * step * I, step, maxiter));
                SetPixel(hdc, half_width + (i + 1), half_height + j,
                         get_smooth_color(point + (i + 1) * step - j * step * I, step, maxiter));
            }
        }
    }
}

/**
 * @brief Calculate the data of mandbrot
 * @details [long description]
 * 
 * @param point [description]
 * @param max_iterations [description]
 * 
 * @return [description]
 */
double mandelbrot(double complex point, unsigned int max_iterations)
{
    double complex calculate = 0;
    unsigned int iterations_time = 0;
    while (iterations_time <= max_iterations) {
        iterations_time ++;
        calculate = calculate * calculate + point;
        if (creal(calculate) * creal(calculate) + cimag(calculate) * cimag(calculate) >= 4.) {
            calculate = calculate * calculate + point;
            calculate = calculate * calculate + point;
            return iterations_time + 10.0 - log(log(creal(calculate) * creal(calculate) + cimag(calculate) * cimag(calculate)) * 0.5) * 1.44269504;
        }
    }
    return -1;
}

/**
 * @brief Save device to image
 * @details [long description]
 * 
 * @param hdc [description]
 * @param filename [description]
 */
void SaveDc2Image(HDC hdc, char filename[])
{
    // 获取屏幕分辨率
    int Width = GetSystemMetrics(SM_CXSCREEN);
    int Height = GetSystemMetrics(SM_CYSCREEN);

    HBITMAP memBitmap, oldmemBitmap;
    oldmemBitmap = CreateCompatibleBitmap(hdc, Width, Height);//建立和屏幕兼容的bitmap

    memBitmap = SelectObject(hdc, oldmemBitmap); // 置换出当前bitmap

    //以下代码保存memDC中的位图到文件
    BITMAP bmp;
    GetObject(memBitmap, sizeof(bmp), &bmp);
    oldmemBitmap = SelectObject(hdc, memBitmap); // 恢复之前的bitmap

    FILE *fp = fopen(filename, "w+b");

    BITMAPINFOHEADER bih = {0};//位图信息头
    bih.biBitCount = bmp.bmBitsPixel;//每个像素字节大小
    bih.biCompression = BI_RGB;
    bih.biHeight = bmp.bmHeight;//高度
    bih.biPlanes = 1;
    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;//图像数据大小
    bih.biWidth = bmp.bmWidth;//宽度

    BITMAPFILEHEADER bfh = {0};//位图文件头
    bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//到位图数据的偏移量
    bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;//文件总的大小
    bfh.bfType = (WORD)0x4d42;

    fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), fp);//写入位图文件头

    fwrite(&bih, 1, sizeof(BITMAPINFOHEADER), fp);//写入位图信息头

    byte *p = malloc(bmp.bmWidthBytes * bmp.bmHeight);//申请内存保存位图数据

    GetDIBits(hdc, memBitmap, 0, Height, p, (LPBITMAPINFO)(&bih), DIB_RGB_COLORS);//获取位图数据

    fwrite(p, 1, bmp.bmWidthBytes * bmp.bmHeight, fp);//写入位图数据
    free(p);
    fclose(fp);

    DeleteObject(oldmemBitmap); // 清理
}

/**
 * @brief Get smooth color
 * @details [long description]
 * 
 *   O * O
 *   * x *
 *   O * O
 * 
 * @param point   [description]
 * @param step    [description]
 * @param maxiter [description]
 * @return        [description]
 */
COLORREF get_smooth_color( double complex point, double step, unsigned int maxiter)
{
    return convert_color(
               interpolate(
                   interpolate(
                       interpolate(
                           log_color(mandelbrot(point - 0.25 * step  - 0.25 * step * I, maxiter)),
                           log_color(mandelbrot(point - 0.25 * step  + 0.25 * step * I, maxiter)),
                           0.5
                       ),
                       interpolate(
                           log_color(mandelbrot(point + 0.25 * step  - 0.25 * step * I, maxiter)),
                           log_color(mandelbrot(point + 0.25 * step  + 0.25 * step * I, maxiter)),
                           0.5
                       ),
                       0.5
                   ),
                   interpolate(
                       interpolate(
                           log_color(mandelbrot(point - 0.25 * step, maxiter)),
                           log_color(mandelbrot(point + 0.25 * step, maxiter)),
                           0.5
                       ),
                       interpolate(
                           log_color(mandelbrot(point - 0.25 * step * I, maxiter)),
                           log_color(mandelbrot(point + 0.25 * step * I, maxiter)),
                           0.5
                       ),
                       0.5
                   ),
                   0.5
               )
           );
}

/**
 * @brief Get smooth color
 * @details [long description]
 * 
 *   O   O
 *     x
 *   O   O
 * 
 * @param point   [description]
 * @param step    [description]
 * @param maxiter [description]
 * @return        [description]
 */
COLORREF get_smooth_color2( double complex point, double step, unsigned int maxiter)
{
    return convert_color(
               interpolate(
                   interpolate(
                       log_color(mandelbrot(point - 0.25 * step  - 0.25 * step * I, maxiter)),
                       log_color(mandelbrot(point - 0.25 * step  + 0.25 * step * I, maxiter)),
                       0.5
                   ),
                   interpolate(
                       log_color(mandelbrot(point + 0.25 * step  - 0.25 * step * I, maxiter)),
                       log_color(mandelbrot(point + 0.25 * step  + 0.25 * step * I, maxiter)),
                       0.5
                   ),
                   0.5
               )
           );
}

RGB_COLOR hsv2rgb(double h, double s, double v)
{
    double f;
    RGB_COLOR i;
    h -= floor(h);
    h *= 6.0;
    if (s < 0) {
        s = 0;
    }
    if (s > 1) {
        s = 1;
    }
    if (v < 0) {
        v = 0;
    }
    if (v > 1) {
        v = 1;
    }
    f = h - floor(h);
    switch ((long)floor(h)) {
    case 0:
        i.r = 1;
        i.g = 1 - (1 - f) * s;
        i.b = 1 - s;
        break;
    case 1:
        i.r = 1 - s * f;
        i.g = 1;
        i.b = 1 - s;
        break;
    case 2:
        i.r = 1 - s;
        i.g = 1;
        i.b = 1 - (1 - f) * s;
        break;
    case 3:
        i.r = 1 - s;
        i.g = 1 - s * f;
        i.b = 1;
        break;
    case 4:
        i.r = 1 - (1 - f) * s;
        i.g = 1 - s;
        i.b = 1;
        break;
    default:
        i.r = 1;
        i.g = 1 - s;
        i.b = 1 - s * f;
    }
    i.r *= v;
    i.g *= v;
    i.b *= v;
    return i;
}

RGB_COLOR palette(double d)
{
    // time += 100;
    double v = sin(d * 0.1847969) * 0.5 + 0.5;
    return hsv2rgb(d * 0.00521336,
                   (sin(d * 0.162012467) * 0.5 + 0.5) * (1 - v),
                   v);
}

RGB_COLOR interpolate(RGB_COLOR a, RGB_COLOR b, double t)
{
    RGB_COLOR i;
    i.r = a.r + t * (b.r - a.r);
    i.g = a.g + t * (b.g - a.g);
    i.b = a.b + t * (b.b - a.b);
    return i;
}

RGB_COLOR log_color(double d)
{
    double p;
    if (d < 0)
        return (RGB_COLOR) {0, 0, 0};
    d += 50;
    d = log(d) * 100;
    p = floor(d);
    return interpolate(palette(p), palette(p + 1), d - p);
}

COLORREF convert_color(RGB_COLOR m)
{
    return RGB(
               m.r <= 0 ? 0 : m.r >= 0.999 ? 255 : floor(m.r * 256),
               m.g <= 0 ? 0 : m.g >= 0.999 ? 255 : floor(m.g * 256),
               m.b <= 0 ? 0 : m.b >= 0.999 ? 255 : floor(m.b * 256)
           );
}