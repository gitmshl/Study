#ifndef INCLUDE_H
#define INCLUDE_H

#include "Consts.hpp"

/*
    Этот файл содержит функции, которые проверяют, содержится ли наше окно в каком
    либо из секторов, и если да, то возвращается номер этого сектора. Иначе, -1.
     _______
    |\  2  /|
    |1 |0| 3|
    |/  4  \|

      - Карта секторов
*/

/*
    Функция, которая проверяет, содержится ли точка в 0 секторе. Если да, то возвращает-
    ся true, иначе false
*/
bool Include_Point_In_0(double x_point, double y_point)
{
    double Left = window.getPosition().x + Wall.getPosition().x + ex;
    double Right = Left + Wall.getSize().x; // если что, подправить чуть чуть;)
    double Top = window.getPosition().y + Wall.getPosition().y + ey;
    double Bottom = Top + Wall.getSize().y;

    if (x_point >= Left && x_point <= Right && y_point >= Top && y_point <= Bottom)
        return true;
    return false;
}

bool Include_Point_In_1(double x_point, double y_point)
{
    double nx =  window.getPosition().x + ex;
    double ny = window.getPosition().y + ey;

    double x1 = nx;
    double y1 = ny;

    double x2 = Wall.getPosition().x + nx;
    double y2 = Wall.getPosition().y + ny;

    if (x_point >= x1 && x_point <= x2)
    {
        double x3 = x2;
        double y3 = y2 + Wall.getSize().y;

        double x4 = x1;
        double y4 = y1 + window.getSize().y;

        double k = (y2-y1)/(x2-x1);
        double s = (y4-y3)/(x3-x4);
        double delta = x_point-x1;

        double y_up = y1 + k*delta;
        double y_down = y4 - s*delta;
///std::cout << y_up << " " << y_point << " " << y_down << " Yes!\n";
        if (y_point >= y_up && y_point <= y_down) return true;
        return false;
    }
    return false;
}

bool Include_Point_In_2(double x_point, double y_point)
{
    double nx =  window.getPosition().x + ex;
    double ny = window.getPosition().y + ey;

    double x1 = nx;
    double y1 = ny;

    double x2 = Wall.getPosition().x + nx;
    double y2 = Wall.getPosition().y + ny;

    if (y1 <= y_point && y_point <= y2)
    {
        double x3 = x2 + Wall.getSize().x;
        double y3 = y2;

        double x4 = x1 + window.getSize().x;
        double y4 = y1;

        double k = (y2-y1)/(x2-x1);
        double s = (y1-y2)/(x3-x4);
        double delta = y_point - y1;

        double x_left = delta/k + x1;
        double x_right = x4 - delta/s;

        if (x_left <= x_point && x_point <= x_right) return true;
        return false;
    }
    return false;
}

bool Include_Point_In_3(double x_point, double y_point)
{
    double nx =  window.getPosition().x + ex;
    double ny = window.getPosition().y + ey;

    double x1 = nx + Wall.getPosition().x + Wall.getSize().x;
    double y1 = ny + Wall.getPosition().y;

    double x2 = nx + window.getSize().x;
    double y2 = ny;

    if (x1 <= x_point && x_point <= x2)
    {
        double x3 = x2;
        double y3 = ny + window.getSize().y;

        double x4 = x1;
        double y4 = y1 + Wall.getSize().y;

        double s = (y1-y2)/(x2-x1);
        double k = (y3-y4)/(x3-x4);
        double delta = x2-x_point;

        double y_up = y2 + s*delta;
        double y_down = y3 - k*delta;

        if (y_up <= y_point && y_point <= y_down) return true;
        return false;
    }
    return false;
}

bool Include_Point_In_4(double x_point, double y_point)
{
    double nx =  window.getPosition().x + ex;
    double ny = window.getPosition().y + ey;

    double x1 = nx;
    double y1 = ny + window.getSize().y;

    double x2 = x1 + Wall.getPosition().x;
    double y2 = ny + Wall.getPosition().y + Wall.getSize().y;

    if (y2 <= y_point && y_point <= y1)
    {
        double x3 = x2 + Wall.getSize().x;
        double y3 = y2;

        double x4 = x1 + window.getSize().x;
        double y4 = y1;

        double s = (y1-y2)/(x2-x1);
        double k = (y4-y3)/(x4-x3);
        double delta = y1-y_point;

        double x_left = x1 + delta/s;
        double x_right = x4 - delta/k;

        if (x_left <= x_point && x_point <= x_right) return true;
        return false;
    }
    return false;
}

/*
    Возвращает true, если прямоугольник r содержится в 0 секторе
*/
bool Include_Window_In_0(RECT r)
{
    if (Include_Point_In_0(r.left, r.top))
      if (Include_Point_In_0(r.right, r.top))
        if (Include_Point_In_0(r.left, r.bottom))
            if (Include_Point_In_0(r.right, r.bottom))
                 return true;
    return false;
}

bool Include_Window_In_1(RECT r)
{
    if (Include_Point_In_1(r.left, r.top))
      if (Include_Point_In_1(r.right, r.top))
        if (Include_Point_In_1(r.left, r.bottom))
            if (Include_Point_In_1(r.right, r.bottom))
                 return true;
    return false;
}

bool Include_Window_In_2(RECT r)
{
    if (Include_Point_In_2(r.left, r.top))
      if (Include_Point_In_2(r.right, r.top))
        if (Include_Point_In_2(r.left, r.bottom))
            if (Include_Point_In_2(r.right, r.bottom))
                 return true;
    return false;
}

bool Include_Window_In_3(RECT r)
{
    if (Include_Point_In_3(r.left, r.top))
      if (Include_Point_In_3(r.right, r.top))
        if (Include_Point_In_3(r.left, r.bottom))
            if (Include_Point_In_3(r.right, r.bottom))
                 return true;
    return false;
}

bool Include_Window_In_4(RECT r)
{
    if (Include_Point_In_4(r.left, r.top))
      if (Include_Point_In_4(r.right, r.top))
        if (Include_Point_In_4(r.left, r.bottom))
            if (Include_Point_In_4(r.right, r.bottom))
                 return true;
    return false;
}

/*
    Функция возвращает номер сектора, в котором находится целиком окно. Если же
    такого сектора нет(т.е. окно не находится целиком в каком либо из секторов), то
    возвращается -1
*/
int Include_Window_In_Sector(HWND h)
{
    RECT rect1 = {0};
    GetWindowRect(h, &rect1);

    if (Include_Window_In_0(rect1)) return 0;
    if (Include_Window_In_1(rect1)) return 1;
    if (Include_Window_In_2(rect1)) return 2;
    if (Include_Window_In_3(rect1)) return 3;
    if (Include_Window_In_4(rect1)) return 4;
    return -1;
}

#endif // INCLUDE_H
