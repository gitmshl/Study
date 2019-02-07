#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <SFML/Graphics.hpp>

using namespace sf;
/*
    Этот файл содержит функции для работы с графическими элементами оконного менед-
    жера
*/

/// "Украшает" графику, путем добавления цветов или текстурок...
void decorate(RectangleShape &Wall)
{
   ///Закрашиваем стену(дальнюю) в синий цвет
   Wall.setFillColor(Color::Blue);
}

/*
    Рисует линии в окне
*/
void Line_Draw(RenderWindow &window, RectangleShape &Wall, VertexArray &line1, VertexArray &line2, VertexArray &line3, VertexArray &line4)
{
    /// Устаналиваем линии
    line1[0].position = Vector2f(0, 0);
    line1[1].position = Vector2f(Wall.getPosition().x, Wall.getPosition().y);
    line1[0].color = Color::Black;
    line1[1].color = Color::Black;

    line2[0].position = Vector2f(0, window.getSize().y);
    line2[1].position = Vector2f(Wall.getPosition().x, Wall.getPosition().y + Wall.getSize().y);
    line2[0].color = Color::Black;
    line2[1].color = Color::Black;

    line3[0].position = Vector2f(window.getSize().x, window.getSize().y);
    line3[1].position = Vector2f(Wall.getPosition().x + Wall.getSize().x, Wall.getPosition().y + Wall.getSize().y);
    line3[0].color = Color::Black;
    line3[1].color = Color::Black;

    line4[0].position = Vector2f(window.getSize().x, 0);
    line4[1].position = Vector2f(Wall.getPosition().x + Wall.getSize().x, Wall.getPosition().y);
    line4[0].color = Color::Black;
    line4[1].color = Color::Black;

    window.draw(line1);
    window.draw(line2);
    window.draw(line3);
    window.draw(line4);
}
///Устанавливает "внутренность" окна, т.е. расположе-
///ние стен, прорисовка линий...
void Draw(RenderWindow &window, RectangleShape &Wall, VertexArray &line1, VertexArray &line2, VertexArray &line3, VertexArray &line4)
{
    /// Устанавливаем стену
    Wall.setPosition((window.getSize().x-Wall.getSize().x)/2, (window.getSize().y-Wall.getSize().y)/2);

    ///Рисуем линии
    Line_Draw(window, Wall, line1, line2, line3, line4);

    decorate(Wall);

    window.draw(Wall);
}

#endif // GRAPHIC_H
