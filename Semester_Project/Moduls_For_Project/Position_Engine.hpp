#ifndef POSITION_ENGINE_H
#define POSITION_ENGINE_H

/*
    ���� ������ ���������� Position_Engine. ��� ���� - ��������� ���� ��������
    � ������ ��� ������� area ����� �������� � ������������ ��������.
*/

void engine_sector_0(HWND hwnd)
{
    RECT r = {0};
    GetWindowRect(hwnd, &r);

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

    double x_p_1, y_p_1, x_p_2, y_p_2, x_p_3, y_p_3, x_p_4, y_p_4;
    process_rect_coord(r, x_p_1, y_p_1, x_p_2, y_p_2, x_p_3, y_p_3, x_p_4, y_p_4);

/// ��������� ���������� �� x
   if (!(x_p_1 > x1 && x_p_2 < x2)) /// ���� ���-�� �� ����� ���������� � ����������
   {
       if (x_p_1 > x1) /// ������ ����� ������� �� ������� �������
       {
           double delta = x_p_2 - x2;
           if (x_p_1 - delta > x1)
           {
               x_p_1 -= delta;
               x_p_2 = x2 - 4;
             if (hwnd == hwnd0)  std::cout << "x right 1\n";
           }
           else
           {
               x_p_1 = x1;
               x_p_2 = x2 - 4;
              if (hwnd == hwnd0)  std::cout << "x right 2\n";
           }
       }
       else if (x_p_2 < x2) /// ����� ����� ������� �� �������
       {
           double delta = x1 - x_p_1;
           if (x_p_2 + delta < x2)
           {
               x_p_2 += delta;
               x_p_1 = x1;
               if (hwnd == hwnd0)  std::cout << "x left 1\n";
           }
           else
           {
               x_p_2 = x2 - 4;
               x_p_1 = x1;
              if (hwnd == hwnd0)  std::cout << "x left 2\n";
           }
       }
       else /// ������� �� ������� ������� ��� �������
       {
            x_p_1 = x1;
            x_p_2 = x2 - 4;
       }
   }
  /// ������ ��������� �� y

  if (!(y_p_1 > y1 && y_p_3 < y3)) /// ���� �� ��� � �������
  {
      if (y_p_1 > y1) /// ������ ����� ������� �� ������� �������
      {
          double delta = y_p_3 - y3;
          if (y_p_1 - delta > y1)
          {
              y_p_1 -= delta;
              y_p_3 = y3;
             if (hwnd == hwnd0)  std::cout << "delta: " << delta << " y down 1\n";
          }
          else
          {
              y_p_1 = y1;
              y_p_3 = y3;
             if (hwnd == hwnd0)  std::cout << "y down 2\n";
          }
      }
      else if (y_p_3 < y3) /// ������� ����� ���� ������� �� ������� �������
      {
          double delta = y1 - y_p_1;
          if (y_p_3 + delta < y3)
          {
              y_p_3 += delta;
              y_p_1 = y1;
            if (hwnd == hwnd0)   std::cout << "y up 1\n";
          }
          else
          {
              y_p_3 = y3;
              y_p_1 = y1;
             if (hwnd == hwnd0)  std::cout << "y up 2\n";
          }
      }
      else
      {
          y_p_1 = y1;
          y_p_3 = y3;
      }
  }


    MoveWindow(hwnd, (int) x_p_1, (int) y_p_1, (int) (x_p_2-x_p_1), (int) (y_p_3-y_p_1), true);
}

void engine_sector_1(HWND hwnd)
{
    RECT r = {0};
    GetWindowRect(hwnd, &r);
    /// ���������� ���������� ������� 1
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
/// ���������� � ���������� ���������� ���� ��������
    double x_p_1, y_p_1, x_p_2, y_p_2, x_p_3, y_p_3, x_p_4, y_p_4;
    process_rect_coord(r, x_p_1, y_p_1, x_p_2, y_p_2, x_p_3, y_p_3, x_p_4, y_p_4);

/// ����������� �� x

/// ��������� ������������ ����� � ������ ������� �� x ��� ������������
double x_max_left = x1, x_max_right = x2;

    if (!(x_p_1 > x_max_left && x_p_2 < x_max_right)) /// ���� ����� ���-�� �����������
    {
        if (x_p_1 > x_max_left) /// ������ ����� ������� �� ������� �������
       {
           double delta = x_p_2 - x_max_right;
           if (x_p_1 - delta > x_max_left)
           {
               x_p_1 -= delta;
               x_p_2 = x_max_right - 4;
           }
           else
           {
               x_p_1 = x_max_left;
               x_p_2 = x_max_right - 4;
           }
       }
       else if (x_p_2 < x_max_right) /// ����� ����� ������� �� �������
       {
           double delta = x_max_left - x_p_1;
           if (x_p_2 + delta < x_max_right)
           {
               x_p_2 += delta;
               x_p_1 = x_max_left;
           }
           else
           {
               x_p_2 = x_max_right - 4;
               x_p_1 = x_max_left;
           }
       }
       else /// ������� �� ������� ������� ��� �������
       {
            x_p_1 = x_max_left;
            x_p_2 = x_max_right - 4;
       }
    }

/// ������ ����������� �� y
double y_max_up = line_border_by_x(x1, y1, x2, y2, x_p_2),  y_max_down = line_border_by_x(x3, y3, x4, y4, x_p_2);

if (!(y_p_1 > y_max_up && y_p_3 < y_max_down)) /// ���� �� ��� � �������
  {
      if (y_p_1 > y_max_up) /// ������ ����� ������� �� ������� �������
      {
          double delta = y_p_3 - y_max_down;
          if (y_p_1 - delta > y_max_up)
          {
              y_p_1 -= delta;
              y_p_3 = y_max_down;
          }
          else
          {
              y_p_1 = y_max_up;
              y_p_3 = y_max_down;
          }
      }
      else if (y_p_3 < y_max_down) /// ������� ����� ���� ������� �� ������� �������
      {
          double delta = y_max_up - y_p_1;
          if (y_p_3 + delta < y_max_down)
          {
              y_p_3 += delta;
              y_p_1 = y_max_up;
          }
          else
          {
              y_p_3 = y_max_down;
              y_p_1 = y_max_up;
          }
      }
      else
      {
          y_p_1 = y_max_up;
          y_p_3 = y_max_down;
      }
}
     MoveWindow(hwnd, (int) x_p_1, (int) y_p_1, (int) (x_p_2-x_p_1), (int) (y_p_3-y_p_1), true);
}

void engine_sector_2(HWND hwnd)
{
    RECT r = {0};
    GetWindowRect(hwnd, &r);

    /// ���������� ���������� ������� 2
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
/// ���������� � ���������� ���������� ���� ��������
    double x_p_1, y_p_1, x_p_2, y_p_2, x_p_3, y_p_3, x_p_4, y_p_4;
    process_rect_coord(r, x_p_1, y_p_1, x_p_2, y_p_2, x_p_3, y_p_3, x_p_4, y_p_4);

/// ����������� �� y

double y_max_up = y1, y_max_down = y3;

if (!(y_p_1 > y_max_up && y_p_3 < y_max_down)) /// ���� �� ��� � �������
  {
      if (y_p_1 > y_max_up) /// ������ ����� ������� �� ������� �������
      {
          double delta = y_p_3 - y_max_down;
          if (y_p_1 - delta > y_max_up)
          {
              y_p_1 -= delta;
              y_p_3 = y_max_down;
          }
          else
          {
              y_p_1 = y_max_up;
              y_p_3 = y_max_down;
          }
      }
      else if (y_p_3 < y_max_down) /// ������� ����� ���� ������� �� ������� �������
      {
          double delta = y_max_up - y_p_1;
          if (y_p_3 + delta < y_max_down)
          {
              y_p_3 += delta;
              y_p_1 = y_max_up;
          }
          else
          {
              y_p_3 = y_max_down;
              y_p_1 = y_max_up;
          }
      }
      else
      {
          y_p_1 = y_max_up;
          y_p_3 = y_max_down;
      }
}
/// ������ ����������� �� x

/// ��������� ������������ ����� � ������ ������� �� x ��� ������������
double x_max_left = line_border_by_y(x1, y1, x4, y4, y_p_3), x_max_right = line_border_by_y(x2, y2, x3, y3, y_p_3);

    if (!(x_p_1 > x_max_left && x_p_2 < x_max_right)) /// ���� ����� ���-�� �����������
    {
        if (x_p_1 > x_max_left) /// ������ ����� ������� �� ������� �������
       {
           double delta = x_p_2 - x_max_right;
           if (x_p_1 - delta > x_max_left)
           {
               x_p_1 -= delta;
               x_p_2 = x_max_right - 4;
           }
           else
           {
               x_p_1 = x_max_left;
               x_p_2 = x_max_right - 4;
           }
       }
       else if (x_p_2 < x_max_right) /// ����� ����� ������� �� �������
       {
           double delta = x_max_left - x_p_1;
           if (x_p_2 + delta < x_max_right)
           {
               x_p_2 += delta;
               x_p_1 = x_max_left;
           }
           else
           {
               x_p_2 = x_max_right - 4;
               x_p_1 = x_max_left;
           }
       }
       else /// ������� �� ������� ������� ��� �������
       {
            x_p_1 = x_max_left;
            x_p_2 = x_max_right - 4;
       }
    }


     MoveWindow(hwnd, (int) x_p_1, (int) y_p_1, (int) (x_p_2-x_p_1), (int) (y_p_3-y_p_1), true);
}

void engine_sector_3(HWND hwnd)
{
    RECT r = {0};
    GetWindowRect(hwnd, &r);

/// ���������� ���������� ������� 3

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

/// ���������� � ���������� ���������� ���� ��������
    double x_p_1, y_p_1, x_p_2, y_p_2, x_p_3, y_p_3, x_p_4, y_p_4;
    process_rect_coord(r, x_p_1, y_p_1, x_p_2, y_p_2, x_p_3, y_p_3, x_p_4, y_p_4);
  /// ����������� �� x

/// ��������� ������������ ����� � ������ ������� �� x ��� ������������
double x_max_left = x1, x_max_right = x2;

    if (!(x_p_1 > x_max_left && x_p_2 < x_max_right)) /// ���� ����� ���-�� �����������
    {
        if (x_p_1 > x_max_left) /// ������ ����� ������� �� ������� �������
       {
           double delta = x_p_2 - x_max_right;
           if (x_p_1 - delta > x_max_left)
           {
               x_p_1 -= delta;
               x_p_2 = x_max_right - 4;
           }
           else
           {
               x_p_1 = x_max_left;
               x_p_2 = x_max_right - 4;
           }
       }
       else if (x_p_2 < x_max_right) /// ����� ����� ������� �� �������
       {
           double delta = x_max_left - x_p_1;
           if (x_p_2 + delta < x_max_right)
           {
               x_p_2 += delta;
               x_p_1 = x_max_left;
           }
           else
           {
               x_p_2 = x_max_right - 4;
               x_p_1 = x_max_left;
           }
       }
       else /// ������� �� ������� ������� ��� �������
       {
            x_p_1 = x_max_left;
            x_p_2 = x_max_right - 4;
       }
    }

/// ������ ����������� �� y
double y_max_up = line_border_by_x(x1, y1, x2, y2, x_p_1),  y_max_down = line_border_by_x(x3, y3, x4, y4, x_p_1);

if (!(y_p_1 > y_max_up && y_p_3 < y_max_down)) /// ���� �� ��� � �������
  {
      if (y_p_1 > y_max_up) /// ������ ����� ������� �� ������� �������
      {
          double delta = y_p_3 - y_max_down;
          if (y_p_1 - delta > y_max_up)
          {
              y_p_1 -= delta;
              y_p_3 = y_max_down;
          }
          else
          {
              y_p_1 = y_max_up;
              y_p_3 = y_max_down;
          }
      }
      else if (y_p_3 < y_max_down) /// ������� ����� ���� ������� �� ������� �������
      {
          double delta = y_max_up - y_p_1;
          if (y_p_3 + delta < y_max_down)
          {
              y_p_3 += delta;
              y_p_1 = y_max_up;
          }
          else
          {
              y_p_3 = y_max_down;
              y_p_1 = y_max_up;
          }
      }
      else
      {
          y_p_1 = y_max_up;
          y_p_3 = y_max_down;
      }
}
     MoveWindow(hwnd, (int) x_p_1, (int) y_p_1, (int) (x_p_2-x_p_1), (int) (y_p_3-y_p_1), true);

}

void engine_sector_4(HWND hwnd)
{
    RECT r = {0};
    GetWindowRect(hwnd, &r);

/// ���������� ���������� ������� 4
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

    /// ���������� � ���������� ���������� ���� ��������
    double x_p_1, y_p_1, x_p_2, y_p_2, x_p_3, y_p_3, x_p_4, y_p_4;
    process_rect_coord(r, x_p_1, y_p_1, x_p_2, y_p_2, x_p_3, y_p_3, x_p_4, y_p_4);

 /// ����������� �� y

double y_max_up = y2, y_max_down = y1;

if (!(y_p_1 > y_max_up && y_p_3 < y_max_down)) /// ���� �� ��� � �������
  {
      if (y_p_1 > y_max_up) /// ������ ����� ������� �� ������� �������
      {
          double delta = y_p_3 - y_max_down;
          if (y_p_1 - delta > y_max_up)
          {
              y_p_1 -= delta;
              y_p_3 = y_max_down;
          }
          else
          {
              y_p_1 = y_max_up;
              y_p_3 = y_max_down;
          }
      }
      else if (y_p_3 < y_max_down) /// ������� ����� ���� ������� �� ������� �������
      {
          double delta = y_max_up - y_p_1;
          if (y_p_3 + delta < y_max_down)
          {
              y_p_3 += delta;
              y_p_1 = y_max_up;
          }
          else
          {
              y_p_3 = y_max_down;
              y_p_1 = y_max_up;
          }
      }
      else
      {
          y_p_1 = y_max_up;
          y_p_3 = y_max_down;
      }
}
/// ������ ����������� �� x

/// ��������� ������������ ����� � ������ ������� �� x ��� ������������
double x_max_left = line_border_by_y(x1, y1, x2, y2, y_p_1), x_max_right = line_border_by_y(x3, y3, x4, y4, y_p_1);

    if (!(x_p_1 > x_max_left && x_p_2 < x_max_right)) /// ���� ����� ���-�� �����������
    {
        if (x_p_1 > x_max_left) /// ������ ����� ������� �� ������� �������
       {
           double delta = x_p_2 - x_max_right;
           if (x_p_1 - delta > x_max_left)
           {
               x_p_1 -= delta;
               x_p_2 = x_max_right - 4;
           }
           else
           {
               x_p_1 = x_max_left;
               x_p_2 = x_max_right - 4;
           }
       }
       else if (x_p_2 < x_max_right) /// ����� ����� ������� �� �������
       {
           double delta = x_max_left - x_p_1;
           if (x_p_2 + delta < x_max_right)
           {
               x_p_2 += delta;
               x_p_1 = x_max_left;
           }
           else
           {
               x_p_2 = x_max_right - 4;
               x_p_1 = x_max_left;
           }
       }
       else /// ������� �� ������� ������� ��� �������
       {
            x_p_1 = x_max_left;
            x_p_2 = x_max_right - 4;
       }
    }


     MoveWindow(hwnd, (int) x_p_1, (int) y_p_1, (int) (x_p_2-x_p_1), (int) (y_p_3-y_p_1), true);


}

void Position_Engine(HWND hwnd, int area)
{
    switch (area)
    {
        case 0: {engine_sector_0(hwnd); break;}
        case 1: {engine_sector_1(hwnd); break;}
        case 2: {engine_sector_2(hwnd); break;}
        case 3: {engine_sector_3(hwnd); break;}
        case 4: {engine_sector_4(hwnd); break;}
    }
}

#endif // POSITION_ENGINE_H
