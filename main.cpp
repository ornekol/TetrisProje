#include <SFML/Graphics.hpp>

const int SATIR = 20;
const int SUTUN = 10;
int oyunAlani[SATIR][SUTUN] = { 0 };

int main()
{
    sf::RenderWindow window(sf::VideoMode(SUTUN * 32, SATIR * 32), "SFML Tetris");
    sf::RectangleShape kare(sf::Vector2f(32.0f, 32.0f));
    kare.setOutlineThickness(-1.f);
    kare.setOutlineColor(sf::Color(50, 50, 50));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed) window.close();

        window.clear(sf::Color::Black);

        for (int i = 0; i < SATIR; i++)
        {
            for (int j = 0; j < SUTUN; j++)
            {
                if (oyunAlani[i][j] == 0) kare.setFillColor(sf::Color(20, 20, 20));
                kare.setPosition(j * 32.f, i * 32.f);
                window.draw(kare);
            }
        }
        window.display();
    }
    return 0;
}