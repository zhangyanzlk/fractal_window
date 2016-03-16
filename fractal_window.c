#include <windows.h>
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include "fractal.h"
pthread_t thread_id;
void *paint_thread(void);

HDC hDC; // �豸���
HDC buffer_hdc; //�����豸�����ڻ���
HBITMAP buffer_bmp; //����λͼ�����ڻ���
RECT paint_rect; //�洢������������

LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow)
{
    char szClassName[] = "MainWClass";
    WNDCLASSEX wndclass;
    // �����������ڵĲ������ WNDCLASSEX �ṹ
    wndclass.cbSize = sizeof(wndclass); // �ṹ�Ĵ�С
    wndclass.style = CS_HREDRAW | CS_VREDRAW; // ָ�������С�ı���ػ�
    wndclass.lpfnWndProc = MainWndProc; // ���ں���ָ��
    wndclass.cbClsExtra = 0; // û�ж�������ڴ�
    wndclass.cbWndExtra = 0; // û�ж���Ĵ����ڴ�
    wndclass.hInstance = hInstance; // ʵ�����
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION); // ʹ��Ԥ����ͼ��
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW); // ʹ��Ԥ����Ĺ��
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // ʹ�ð�ɫ������ˢ
    wndclass.lpszMenuName = NULL; // ��ָ���˵�
    wndclass.lpszClassName = szClassName ; // �����������
    wndclass.hIconSm = NULL; // û�����Сͼ��
    // ע�����������
    RegisterClassEx(&wndclass);// ����������
    HWND hwnd = CreateWindowEx(
                    0, // dwExStyle����չ��ʽ
                    szClassName, // lpClassName������
                    "fractal_window", // lpWindowName������
                    WS_OVERLAPPEDWINDOW, // dwStyle�����ڷ��
                    CW_USEDEFAULT, // X����ʼ X ����
                    CW_USEDEFAULT, // Y����ʼ Y ����
                    // CW_USEDEFAULT, // nWidth�����
                    // CW_USEDEFAULT, // nHeight���߶�
                    800, // nWidth�����
                    480, // nHeight���߶�
                    NULL, // hWndParent�������ھ��
                    NULL, // hMenu���˵����
                    hInstance, // hlnstance������ʵ�����
                    NULL) ; // lpParam���û�����

    if (hwnd == NULL) {
        MessageBox(NULL, "�������ڳ��� ", "error", MB_OK);
        return -1;
    }

    // ��ʾ���ڣ�ˢ�´��ڿͻ���
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    // ����Ϣ������ȡ����Ϣ���������ں�������ֱ�� GetMessage ���� FALSE��������Ϣѭ��
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0)) {
        // ת��������Ϣ
        TranslateMessage(&msg);
        // ����Ϣ���͵���Ӧ�Ĵ��ں���
        DispatchMessage(&msg);
    }

    // �� GetMessage ���� FALSE ʱ�������
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

    case WM_PAINT: // ���ڿͻ�����Ҫ�ػ�
        // GetClientRect (hwnd, &rect) ;
        // // ʹ��Ч�Ŀͻ��������Ч����ȡ���豸�������
        // hdc = BeginPaint (hwnd, &ps) ;
        // draw_mandelbrot5(rect.right - rect.left, rect.bottom - rect.top,
        //                  -0.7520481061064106 + 0.036303434876881466 * I,
        //                  3.637978807091713e-15, 10000, hdc);
        // EndPaint(hwnd, &ps);
        return 0;

    case WM_DESTROY: // �������ٴ���
        pthread_cancel(thread_id);
        // pthread_kill(thread_id, 0);
        // pthread_exit(0);
        pthread_join(thread_id, NULL);
        if (buffer_hdc)
            DeleteDC(buffer_hdc);
        if (buffer_bmp)
            DeleteObject(buffer_bmp);
        ReleaseDC(hwnd, hDC);
        // ����Ϣ����Ͷ��һ�� WM_QUIT ��Ϣ����ʹ GetMessage �������� 0��������Ϣѭ��
        PostQuitMessage(0) ;
        return 0 ;
    }

    // �����ǲ��������Ϣ����ϵͳ��Ĭ�ϴ���
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