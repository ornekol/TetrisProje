#include <SFML/Graphics.hpp>
#include <time.h>

const int SATIR = 20;
const int SUTUN = 10;
int oyunAlani[SATIR][SUTUN] = { 0 };

int sekiller[7][4] = {
    {1,3,5,7}, // I Şekli
    {2,4,5,7}, // Z Şekli
    {3,5,4,6}, // S Şekli
    {3,5,4,7}, // T Şekli
    {2,3,5,7}, // L Şekli
    {3,5,7,6}, // J Şekli
    {2,3,4,5}  // O Şekli (Kare)
};

struct Nokta { int x, y; } a[4], b[4];

// ÇARPIŞMA VE ZEMİN KONTROLÜ
bool kontrol()
{
    for (int i = 0; i < 4; i++)
    {
        // Ekran sınırlarından çıktı mı?
        if (a[i].x < 0 || a[i].x >= SUTUN || a[i].y >= SATIR)
            return false;
        // Gideceği yerde başka bir blok (taşa dönmüş parça) var mı?
        else if (oyunAlani[a[i].y][a[i].x] != 0)
            return false;
    }
    return true;
}

int main()
{
    srand(time(0));
    sf::RenderWindow window(sf::VideoMode(SUTUN * 32, SATIR * 32), "SFML Tetris");

    sf::RectangleShape kare(sf::Vector2f(32.0f, 32.0f));
    kare.setOutlineThickness(-1.f);
    kare.setOutlineColor(sf::Color(50, 50, 50));

    sf::Color renkler[8] = {
        sf::Color::Black, sf::Color::Cyan, sf::Color::Red, sf::Color::Green,
        sf::Color(128, 0, 128), sf::Color(255, 165, 0), sf::Color::Blue, sf::Color::Yellow
    };

    int n = rand() % 7;
    int renkNumarasi = n + 1;

    for (int i = 0; i < 4; i++) {
        a[i].x = sekiller[n][i] % 2 + 4;
        a[i].y = sekiller[n][i] / 2;
    }

    sf::Clock saat;
    float timer = 0, gecikme = 0.5f;

    while (window.isOpen())
    {
        float zaman = saat.getElapsedTime().asSeconds();
        saat.restart();
        timer += zaman;

        sf::Event event;
        int dx = 0;
        bool dondur = false;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // KLAVYE KONTROLLERİ
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up) dondur = true;
                else if (event.key.code == sf::Keyboard::Left) dx = -1;
                else if (event.key.code == sf::Keyboard::Right) dx = 1;
            }
        }

        // Aşağı tuşuna basılı tutunca hızlı düşme
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) gecikme = 0.05f;
        else gecikme = 0.5f;

        // 1. SAĞA VE SOLA HAREKET
        for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].x += dx; }
        if (!kontrol()) { for (int i = 0; i < 4; i++) a[i] = b[i]; }

        // 2. DÖNDÜRME
        if (dondur)
        {
            Nokta merkez = a[1];
            for (int i = 0; i < 4; i++)
            {
                int x = a[i].y - merkez.y;
                int y = a[i].x - merkez.x;
                a[i].x = merkez.x - x;
                a[i].y = merkez.y + y;
            }
            if (!kontrol()) { for (int i = 0; i < 4; i++) a[i] = b[i]; }
        }

        // 3. AŞAĞI DÜŞME VE TAŞA DÖNME
        if (timer > gecikme)
        {
            for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].y += 1; }

            // Eğer zemine veya başka bir bloğa çarptıysa
            if (!kontrol())
            {
                // Parçayı matrise kalıcı olarak yazdır (Taşa çevir)
                for (int i = 0; i < 4; i++) oyunAlani[b[i].y][b[i].x] = renkNumarasi;

                // Yeni şekil fırlat
                n = rand() % 7;
                renkNumarasi = n + 1;
                for (int i = 0; i < 4; i++) {
                    a[i].x = sekiller[n][i] % 2 + 4;
                    a[i].y = sekiller[n][i] / 2;
                }
            }
            timer = 0;
        }

        // --- ÇİZDİRME (RENDER) ---
        window.clear(sf::Color::Black);

        // Arka planı ve sabitlenmiş blokları çizdir
        for (int i = 0; i < SATIR; i++) {
            for (int j = 0; j < SUTUN; j++) {
                if (oyunAlani[i][j] == 0) kare.setFillColor(sf::Color(20, 20, 20));
                else kare.setFillColor(renkler[oyunAlani[i][j]]);

                kare.setPosition(j * 32.f, i * 32.f);
                window.draw(kare);
            }
        }

        // Düşen aktif şekli çizdir
        for (int i = 0; i < 4; i++) {
            kare.setFillColor(renkler[renkNumarasi]);
            kare.setPosition(a[i].x * 32.f, a[i].y * 32.f);
            window.draw(kare);
        }

        window.display();
    }

    return 0;
}