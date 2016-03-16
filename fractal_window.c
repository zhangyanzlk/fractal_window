#include <windows.h>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include "fractal.h"
pthread_t thread_id;
void *paint_thread(void);

HDC hDC; // 设备句柄
HDC buffer_hdc; //兼容设备，用于缓冲
HBITMAP buffer_bmp; //兼容位图，用于缓冲
RECT paint_rect; //存储绘制区域数据

LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow)
{
    char szClassName[] = "MainWClass";
    WNDCLASSEX wndclass;
    // 用描述主窗口的参数填充 WNDCLASSEX 结构
    wndclass.cbSize = sizeof(wndclass); // 结构的大小
    wndclass.style = CS_HREDRAW | CS_VREDRAW; // 指定如果大小改变就重画
    wndclass.lpfnWndProc = MainWndProc; // 窗口函数指针
    wndclass.cbClsExtra = 0; // 没有额外的类内存
    wndclass.cbWndExtra = 0; // 没有额外的窗口内存
    wndclass.hInstance = hInstance; // 实例句柄
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION); // 使用预定义图标
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW); // 使用预定义的光标
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // 使用白色背景画刷
    wndclass.lpszMenuName = NULL; // 不指定菜单
    wndclass.lpszClassName = szClassName ; // 窗口类的名称
    wndclass.hIconSm = NULL; // 没有类的小图标
    // 注册这个窗口类
    RegisterClassEx(&wndclass);// 创建主窗口
    HWND hwnd = CreateWindowEx(
                    0, // dwExStyle，扩展样式
                    szClassName, // lpClassName，类名
                    "fractal_window", // lpWindowName，标题
                    WS_OVERLAPPEDWINDOW, // dwStyle，窗口风格
                    CW_USEDEFAULT, // X，初始 X 坐标
                    CW_USEDEFAULT, // Y，初始 Y 坐标
                    // CW_USEDEFAULT, // nWidth，宽度
                    // CW_USEDEFAULT, // nHeight，高度
                    800, // nWidth，宽度
                    480, // nHeight，高度
                    NULL, // hWndParent，父窗口句柄
                    NULL, // hMenu，菜单句柄
                    hInstance, // hlnstance，程序实例句柄
                    NULL) ; // lpParam，用户数据

    if (hwnd == NULL) {
        MessageBox(NULL, "创建窗口出错！ ", "error", MB_OK);
        return -1;
    }

    // 显示窗口，刷新窗口客户区
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    // 从消息队列中取出消息，交给窗口函数处理，直到 GetMessage 返回 FALSE，结束消息循环
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0)) {
        // 转化键盘消息
        TranslateMessage(&msg);
        // 将消息发送到相应的窗口函数
        DispatchMessage(&msg);
    }

    // 当 GetMessage 返回 FALSE 时程序结束
    return msg.wParam;
}
LRESULT CALLBACK MainWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    int ret;
    switch (message) {
    case WM_CREATE:
        hDC = GetDC((HWND)hwnd);
        GetClientRect (hwnd, &paint_rect);
        ret = pthread_create(&thread_id, NULL, (void*(*)(void *))paint_thread, NULL);
        if (ret != 0) {
            printf ("Create pthread error!\n");
            return 0;
        }
        return 0;

    case WM_SIZE:
        // pthread_join(thread_id, NULL);
        // ret = pthread_create(&thread_id, NULL, paint_thread, (void *)hwnd);
        // if (ret != 0) {
        //     printf ("Create pthread error!\n");
        //     return 0;
        // }
        return 0;

    case WM_PAINT: // 窗口客户区需要重画
        // GetClientRect (hwnd, &rect) ;
        // // 使无效的客户区变的有效，并取得设备环境句柄
        // hdc = BeginPaint (hwnd, &ps) ;
        // draw_mandelbrot5(rect.right - rect.left, rect.bottom - rect.top,
        //                  -0.7520481061064106 + 0.036303434876881466 * I,
        //                  3.637978807091713e-15, 10000, hdc);
        // EndPaint(hwnd, &ps);
        return 0;

    case WM_DESTROY: // 正在销毁窗口
        pthread_cancel(thread_id);
        // pthread_kill(thread_id, 0);
        // pthread_exit(0);
        pthread_join(thread_id, NULL);
        if (buffer_hdc)
            DeleteDC(buffer_hdc);
        if (buffer_bmp)
            DeleteObject(buffer_bmp);
        ReleaseDC(hwnd, hDC);
        // 向消息队列投递一个 WM_QUIT 消息，促使 GetMessage 函数返回 0，结束消息循环
        PostQuitMessage(0) ;
        return 0 ;
    }

    // 将我们不处理的消息交给系统做默认处理
    return DefWindowProc(hwnd, message, wParam, lParam);
}

void *paint_thread(void)
{
    HDC hdc = hDC;
    RECT rect = paint_rect;
    draw_mandelbrot2(rect.right - rect.left, rect.bottom - rect.top,
                     -0.7524968185841104 + 0.039981581780396 * I,
                     1.4551915228366852e-15, 60000, hdc);
    return NULL;
}