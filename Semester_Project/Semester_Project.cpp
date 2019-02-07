#include <SFML/Graphics.hpp>
#include "Moduls_For_Project/Graphic.hpp"
#include "Moduls_For_Project/Intersect.hpp"
#include "Moduls_For_Project/Consts.hpp"
#include "Moduls_For_Project/Include.hpp"
#include "Moduls_For_Project/Arbitr_of_intersections.hpp"
#include "Moduls_For_Project/Position_Engine.hpp"

using namespace sf;


BOOL CALLBACK fnEnumWindowProc(HWND hwnd, LPARAM lParam){

   if (isIntersect(window, hwnd))
   {
       double a = Include_Window_In_Sector(hwnd);
      if (a == -1) /// не принадлежит ни одному сектору, то
        /// вызываем Арбитр пересечений
      {
          /// Вычисляем сектор, которому решает передать окно
          /// арбитр пересечений
          int area = Arbitr_of_intersections(hwnd);
          /// Передаем управление Position_Engine
          Position_Engine(hwnd, area);
      }

   }

  return TRUE;
}



int main()
{

Sleep(5000);
window.setPosition(Vector2i(100, 100));
Initialize_NotAcces_Array();

while (window.isOpen())
    {
        Event event;
    while (window.pollEvent(event))
    {
        if (event.type == Event::Closed)
            window.close();
    }



    window.clear(Color::White);

    Draw(window, Wall, line1, line2, line3, line4);

    EnumWindows(fnEnumWindowProc, 0);

    window.display();

    }
    return 0;

}
