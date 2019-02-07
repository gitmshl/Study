#ifndef ARBITR_OF_INTERSECTIONS_H
#define ARBITR_OF_INTERSECTIONS_H

#include "Common_Functions.hpp"

/*
    Этот модуль называется АРБИТР ПЕРЕСЕЧЕНИЙ. Его цель - определить сектор, которому
    нужно будет передать окно процесса. Этот процесс вызывается, если какое то
    окно пересекается с окном менеджера окон и не входит ни в один из секторов.
    Приоритет передачи определяется площадью пересечения с сектором.
*/

/// Вычисляет площадь пересечения окна процесса с сектором 0
double area_sector_0(RECT r)
{
    double nx =  window.getPosition().x + ex;
    double ny = window.getPosition().y + ey;

    double x1 =  Wall.getPosition().x + nx;
    double y1 = Wall.getPosition().y + ny;

    double x2 = x1 + Wall.getSize().x;
    double y2 = y1;

    double x3 = x2;
    double y3 = y2 + Wall.getSize().y;

    double x4 = x1;
    double y4 = y3;

   return area_rectangle(r, x1, y1, x2, y2, x3, y3, x4, y4);
}

double area_sector_1(RECT r)
{
    double nx =  window.getPosition().x + ex;
    double ny = window.getPosition().y + ey;

    double x1 = nx;
    double y1 = ny;

    double x2 = Wall.getPosition().x + nx;
    double y2 = Wall.getPosition().y + ny;

    double x3 = x2;
    double y3 = y2 + Wall.getSize().y;

    double x4 = x1;
    double y4 = y1 + window.getSize().y;

    return (area_triangle_1(r, x1, y1, x2, y2, x1, y2) +  area_rectangle(r, x1, y2, x2, y2, x3, y3, x1, y3)
            + area_triangle_2(r, x1, y3, x3, y3, x4, y4));
}

double area_sector_2(RECT r)
{
    double nx =  window.getPosition().x + ex;
    double ny = window.getPosition().y + ey;

    double x1 = nx;
    double y1 = ny;

    double x2 = x1 + window.getSize().x;
    double y2 = y1;

    double x3 = x1 + Wall.getPosition().x + Wall.getSize().x;
    double y3 = y1 + Wall.getPosition().y;

    double x4 = x1 + Wall.getPosition().x;
    double y4 = y3;

    return (area_triangle_3(r, x1, y1, x4, y1, x4, y4) +  area_rectangle(r, x4, y1, x3, y1, x3, y3, x4, y4)
            + area_triangle_2(r, x3, y1, x2, y2, x3, y3));
}


double area_sector_3(RECT r)
{
    double nx =  window.getPosition().x + ex;
    double ny = window.getPosition().y + ey;

    double x1 = nx + Wall.getPosition().x + Wall.getSize().x;
    double y1 = ny + Wall.getPosition().y;

    double x2 = nx + window.getSize().x;
    double y2 = ny;

    double x3 = x2;
    double y3 = ny + window.getSize().y;

    double x4 = x1;
    double y4 = y1 + Wall.getSize().y;

    return (area_triangle_4(r, x1, y1, x2, y2, x2, y1) +  area_rectangle(r, x1, y1, x2, y1, x2, y4, x4, y4)
            + area_triangle_3(r, x4, y4, x2, y4, x3, y3));
}

double area_sector_4(RECT r)
{
    double nx =  window.getPosition().x + ex;
    double ny = window.getPosition().y + ey;

    double x1 = nx;
    double y1 = ny + window.getSize().y;

    double x2 = x1 + Wall.getPosition().x;
    double y2 = ny + Wall.getPosition().y + Wall.getSize().y;

    double x3 = x2 + Wall.getSize().x;
    double y3 = y2;

    double x4 = x1 + window.getSize().x;
    double y4 = y1;

    return (area_triangle_4(r, x1, y1, x2, y2, x2, y1) +  area_rectangle(r, x2, y2, x3, y3, x3, y1, x2, y1)
            + area_triangle_1(r, x3, y3, x4, y4, x3, y1));
}

/// Главная функция модуля. Именно она и возвращает нужный сектор
int Arbitr_of_intersections(HWND h)
{
    RECT rect1 = {0};
    GetWindowRect(h, &rect1);

    double Area[4];

    Area[0] = area_sector_0(rect1);
    Area[1] = area_sector_1(rect1);
    Area[2] = area_sector_2(rect1);
    Area[3] = area_sector_3(rect1);
    Area[4] = area_sector_4(rect1);

    int q = 0, Max = Area[0];

    for (int i = 1; i < 5; i++)
        if (Area[i] > Max) {q = i; Max = Area[i];}

    return q;

}

#endif
