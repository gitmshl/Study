#ifndef COMMON_FUNCTIONS_H
#define COMMON_FUNCTIONS_H

/*
    Этот файл содержит функции, которые являются общими и могут использоваться
    разными модулями (например, функция макс, или подсчета площади пересечения
    двух прямоугольников)
*/

double max(double a, double b)
{
    if (a > b) return a;
    return b;
}

double min(double a, double b)
{
    if (a < b) return a;
    return b;
}

/// Определяет координату 'y' у прямой по координате x_point
/// (x1, y1) and (x2, y2) определяют прямую. Задавать можно в любом порядке
double line_border_by_x(double x1, double y1, double x2, double y2, double x_point)
{
    /// Отсортируем так, чтобы x1 <= x2
    if (x1 > x2)
    {
        double a = x1, b = y1;
        x1 = x2;
        y1 = y2;
        x2 = a;
        y2 = b;
    }

 if (x_point <= x1) return y1;
 if (x_point >= x2) return y2;

/// Определим коэффициенты прямой k и m (y = kx + m)
double k = (y2-y1)/(x2-x1);
double m = y1 - k*x1;

return (k*x_point + m);

}

/// Определяет координату 'x' у прямой по координате y_point
/// (x1, y1) and (x2, y2) определяют прямую. Задавать можно в любом порядке
double line_border_by_y(double x1, double y1, double x2, double y2, double y_point)
{
    /// Отсортируем так, чтобы y1 <= y2
    if (y2 < y1)
    {
        double a = x1, b = y1;
        x1 = x2;
        y1 = y2;
        x2 = a;
        y2 = b;
    }

 if (y_point <= y1) return x1;
 if (y_point >= y2) return x2;

/// Определим коэффициенты прямой k и m (y = kx + m)
double k = (y2-y1)/(x2-x1);
double m = y1 - k*x1;

return ((y_point-m)/k);

}

/// Производит присвоение координат процесса по прямоугольнику
void process_rect_coord(RECT r, double &x_p_1, double &y_p_1, double &x_p_2,
    double &y_p_2, double &x_p_3, double &y_p_3, double &x_p_4, double &y_p_4)
{
    x_p_1 = r.left;
    y_p_1 = r.top;

    x_p_2 = r.right;
    y_p_2 = y_p_1;

    x_p_3 = x_p_2;
    y_p_3 = r.bottom;

    x_p_4 = x_p_1;
    y_p_4 = y_p_3;
}

void sector_1_coord(double &x1, double &y1, double &x2, double &y2, double &x3,
              double &y3, double &x4, double &y4)
{
    double nx =  window.getPosition().x + ex;
    double ny = window.getPosition().y + ey;

    x1 = nx;
    y1 = ny;

    x2 = Wall.getPosition().x + nx;
    y2 = Wall.getPosition().y + ny;

    x3 = x2;
    y3 = y2 + Wall.getSize().y;

    x4 = x1;
    y4 = y1 + window.getSize().y;
}

/// Возвращает координаты y верхней и средней линий в 1 секторе при координате
 /// x = x_point
void sector_1(double x_point, double &y_up, double &y_down)
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

    double k = (y2-y1)/(x2-x1);
    double s = (y4-y3)/(x3-x4);
    double delta = x_point-x1;

    y_up = y1 + k*delta;
    y_down = y4 - s*delta;
}


/// Вычисляет площадь пересечения прямоугольника процесса r с прямоугольником,
///имеющим координаты (xi, yi)
double area_rectangle(RECT r, double x1, double y1, double x2, double y2,
    double x3, double y3, double x4, double y4)
{
    double x_p_1, y_p_1, x_p_2, y_p_2, x_p_3, y_p_3, x_p_4, y_p_4;
    process_rect_coord(r, x_p_1, y_p_1, x_p_2, y_p_2, x_p_3, y_p_3, x_p_4, y_p_4);

    if ((x_p_2 < x1) || (x_p_1 > x2) || (y_p_1 > y4) || (y_p_4 < y1)) /// типа вообще
        /// нету пересечения
        return 0;

    double x_i_1 = max(x_p_1, x1);
    double y_i_1 = max(y_p_1, y1);
    double x_i_2 = min(x_p_2, x2);
    double y_i_4 = min(y_p_4, y4);

    return (x_i_2-x_i_1) * (y_i_4-y_i_1);
}

/*
    Обход обсчета по часовой стрелке

    |\
    |_\ 1 тип треугольника
     _
    | /
    |/  2 тип треугольника
    _
   \ |
    \| 3 тип треугольника

    /|
   /_| 4 тип треугольника

*/

/// Функции, вычисляющие площади пересечения окна процесса с треугольниками
/// не совсем хорошо аппроксимируют реальную площадь(учет делается на вероятность)
double area_triangle_1(RECT r, double x1, double y1, double x2, double y2,
    double x3, double y3)
{
    return (area_rectangle(r, x1, y1, x2, y1, x2, y2, x3, y3)/2);
}

double area_triangle_2(RECT r, double x1, double y1, double x2, double y2,
    double x3, double y3)
{
    return (area_rectangle(r, x1, y1, x2, y2, x2, y3, x3, y3)/2);
}

double area_triangle_3(RECT r, double x1, double y1, double x2, double y2,
    double x3, double y3)
{
    return (area_rectangle(r, x1, y1, x2, y2, x3, y3, x1, y3)/2);
}

double area_triangle_4(RECT r, double x1, double y1, double x2, double y2,
    double x3, double y3)
{
    return (area_rectangle(r, x1, y2, x2, y2, x3, y3, x1, y1)/2);
}

#endif // COMMON_FUNCTIONS_H
