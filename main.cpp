#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <cmath>
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Code::Blocks Template Windows App"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}
void Draw8Points(HDC hdc , int xc ,int yc , int a , int b , COLORREF color ){
    SetPixel(hdc , xc + a ,yc + b , color);
    SetPixel(hdc , xc - a, yc + b, color);
    SetPixel(hdc , xc - a, yc - b, color);
    SetPixel(hdc , xc + a, yc - b, color);
    SetPixel(hdc , xc + b, yc + a, color);
    SetPixel(hdc , xc - b, yc + a, color);
    SetPixel(hdc , xc - b, yc - a, color);
    SetPixel(hdc , xc + b, yc - a, color);

}
void DrawCircle(HDC hdc ,int xc , int yc , int R , COLORREF color)
{
    //computed point (0,R)
    int x = 0 ;
    int y = R ;
    //initial change
    int d = 1-R ;
    Draw8Points(hdc , xc , yc , x , y , color);
    while(x<y){
        if (d<0)
        {
            d+= 2*x+3 ;
        }
        else{
            d+= 2*(x-y) + 5 ;
            y--;
        }
        x++;
        Draw8Points(hdc,xc , yc , x , y, color) ;
    }
}
/*----------------Curve Part--------------------*/
struct point{
    int x,y;
};
void HermitCurve(HDC hdc,int x1,int y1,int u1, int v1,int x2,int y2,int u2,int v2,COLORREF c){

double alpha_0=x1;
double alpha_1=u1;
double alpha_2 =-3*x1-2*u1+3*x2-u2;
double alpha_3 =2*x1 + u1 -2*x2 + u2;

double beta_0=y1;
double beta_1=v1;
double beta_2=-3*y1-2*v1+3*y2-v2;
double beta_3=2*y1+v1-2*y2+v2;

for(double t=0;t<=1;t+=0.001){
    double t3=pow(t,3);
    double t2=pow(t,2);
    double x=alpha_0+(alpha_1*t)+(alpha_2*t2)+(alpha_3*t3);
    double y=beta_0+(beta_1*t)+(beta_2*t2)+(beta_3*t3);
    SetPixel(hdc,x,y,c);
}
}


void parametric_line (HDC hdc , int x1 , int y1 , int x2 , int y2,COLORREF c){
    for(double t =0 ; t<1 ; t+=0.0001){
        double x = x1 + t*(x2-x1);
        double y = y1 +t*(y2-y1) ;
        SetPixel(hdc , x , y , c) ;
    }

}

/*  This function is called by the Windows function DispatchMessage()  */
int counter = 0 ;
int x_c_large , y_c_large ;
int x_1_large , y_1_large;
int R;

int x_c_right , y_c_right;
int x_right , y_right;
int R_right;

int x_c_left ,y_c_left;
int x_left , y_left;
int R_left ;

double x_1,y_1,u_1, v_1, x_2, y_2, u_2, v_2;
point P_start,T_start,P_end,T_end;


int h1_x1,h1_y1,h1_x2,h1_y2;
int h2_x1,h2_y1,h2_x2,h2_y2;
int h3_x1,h3_y1,h3_x2,h3_y2;

COLORREF c = RGB(0,0,255);
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     HDC hdc = GetDC(hwnd);
    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        case WM_LBUTTONDBLCLK:
            if(counter==0)
            {
                x_c_large = LOWORD(lParam);
                y_c_large = HIWORD(lParam);
                counter++;
            }
            else if(counter ==1) {
                x_1_large = LOWORD(lParam);
                y_1_large = HIWORD(lParam);
                R = sqrt(pow((x_1_large-x_c_large),2) + pow((y_1_large-y_c_large),2));
                DrawCircle(hdc,x_c_large, y_c_large , R , RGB(100,100,100));
                counter++;
            }
            else if(counter == 2)
            {
                x_c_right = LOWORD(lParam);
                y_c_right = HIWORD(lParam);
                counter ++ ;
            }
            else if(counter == 3)
            {
                x_right= LOWORD(lParam);
                y_right= HIWORD(lParam);
                R_right = sqrt(pow((x_right-x_c_right),2) + pow((y_right-y_c_right),2));
                DrawCircle(hdc,x_c_right, y_c_right , R_right , c);
                counter++;
            }
            else if(counter == 4)
            {
                x_c_left = LOWORD(lParam);
                y_c_left = HIWORD(lParam);
                counter ++ ;
            }
            else if(counter ==5)
            {
                x_left = LOWORD(lParam);
                y_left = HIWORD(lParam);
                R_left = sqrt(pow((x_left-x_c_left),2) + pow((y_left-y_c_left),2));
                DrawCircle(hdc,x_c_left, y_c_left, R_left, c);
                counter++;
            }
            else if(counter == 6)
            {
                x_1 = LOWORD(lParam);
                y_1 = HIWORD(lParam);
                P_start.x = x_1;
                P_start.y = y_1 ;
                counter++;
            }
            else if(counter == 7)
            {
                u_1 = LOWORD(lParam);
                v_1 = HIWORD(lParam);
                T_start.x = u_1 ;
                T_start.y = v_1 ;
                counter ++ ;
            }
            else if(counter ==8)
            {
                u_2 = LOWORD(lParam);
                v_2 = HIWORD(lParam);
                T_end.x = u_2;
                T_end.y = v_2 ;
                counter ++ ;
            }
            else if (counter==9)
            {
                x_2 = LOWORD(lParam);
                y_2 = HIWORD(lParam);
                P_end.x = x_2 ;
                P_end.y = y_2 ;
                HermitCurve(hdc,P_start.x,P_start.y,T_start.x,T_start.y,T_end.x,T_end.y,P_end.x,P_end.y,c);
                counter++;
            }
            else if(counter == 10)
            {
                h1_x1 = LOWORD(lParam);
                h1_y1 = HIWORD(lParam);
                counter++;
            }
            else if (counter == 11)
            {
                h1_x2 = LOWORD(lParam);
                h1_y2 = HIWORD(lParam);
                parametric_line(hdc,h1_x1,h1_y1,h1_x2,h1_y2,c);
                counter ++;
            }
            else if (counter == 12)
            {
                h2_x1 = LOWORD(lParam);
                h2_y1 = HIWORD(lParam);
                counter++;
            }
            else if(counter == 13)
            {
                h2_x2 = LOWORD(lParam);
                h2_y2 = HIWORD(lParam);
                parametric_line(hdc,h2_x1,h2_y1,h2_x2,h2_y2,c);
                counter ++;
            }
            else if (counter == 14)
            {
                h3_x1 = LOWORD(lParam);
                h3_y1 = HIWORD(lParam);
                counter++;
            }
            else if(counter == 15)
            {
                h3_x2 = LOWORD(lParam);
                h3_y2 = HIWORD(lParam);
                parametric_line(hdc,h3_x1,h3_y1,h3_x2,h3_y2,c);
                counter ++;
            }
             break;

        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
