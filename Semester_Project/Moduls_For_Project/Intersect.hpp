#ifndef INTERSECT_H
#define INTERSECT_H

#include <SFML/Graphics.hpp>
#include  "Filter.hpp"
#include "Consts.hpp"
#include <iostream>

using namespace sf;

/*
    Этот файл содержит функции, которые определяют существование пересечений и
    что-то, что связано с пересечениями. ЭТО НЕ АРБИТР ПЕРЕСЕЧЕНИЙ, КОТОРЫЙ ЗАНИМАЕТ-
    СЯ ОПРЕДЕЛЕНИЕМ СЕКТОРА, КОТОРОМУ НУЖНО ОТДАТЬ ОБРАБОТКУ ОКНА ПРИЛОЖЕНИЯ
    ПРИ ПЕРЕСЕЧЕНИИ ОКНА С НЕСКОЛЬКИМИ СЕКТОРАМИ!!!
*/

bool isIntersect(RenderWindow &window, HWND h)
{
    Acces_Current_Window = false;
if (acces(h))
{
    Acces_Current_Window = true;

    Event event;

    if (h != window.getSystemHandle())
    {
        if (Mouse::isButtonPressed(Mouse::Left))
        {

        }
        else
        {
            FloatRect rect;
            RECT rect1 = {0};
            GetWindowRect(h, &rect1);
            rect.left = rect1.left;
            rect.top = rect1.top;
            rect.width = rect1.right-rect1.left;
            rect.height = rect1.bottom-rect1.top;
            FloatRect Window_Rect;
            Window_Rect.left = window.getPosition().x;
            Window_Rect.top = window.getPosition().y;
            Window_Rect.height = window.getSize().y;
            Window_Rect.width = window.getSize().x;

            if (rect != Window_Rect && Window_Rect.intersects(rect))
            {
                return true;
            }
            else return false;
        }

    }
}
return false;
}

#endif // INTERSECT_H
