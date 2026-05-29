#include <SFML/Graphics.hpp>
#include <cstdlib> // rand ve srand fonksiyonları için kesin çözüm
#include <ctime>   // time fonksiyonu için kesin çözüm

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
    {2,3,4,5}  // O Şekli
};

struct Nokta { int x, y; };
Nokta a[4], b[4], hayalet[4];

// ÇARPIŞMA VE ZEMİN KONTROLÜ
// Derleyici hatalarını önlemek için işaretçi (pointer) altyapısına geçildi
bool kontrol(Nokta* parca)
{
    for (int i = 0; i < 4; i++)
    {
        // Y ekseninde eksiye düşme (y < 0) kontrolü de eklenerek güvenlik artırıldı
        if (parca[i].x < 0 || parca[i].x >= SUTUN || parca[i].y < 0 || parca[i].y >= SATIR)
            return false;

        if (oyunAlani[parca[i].y][parca[i].x] != 0)
            return false;
    }
    return true;
}

int main()
{
    srand(static_cast<unsigned int>(time(0)));
    sf::RenderWindow window(sf::VideoMode(SUTUN * 32, SATIR * 32), "SFML Tetris");

    sf::RectangleShape kare(sf::Vector2f(32.0f, 32.0f));
    kare.setOutlineThickness(-1.f);

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

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Up) dondur = true;
                else if (event.key.code == sf::Keyboard::Left) dx = -1;
                else if (event.key.code == sf::Keyboard::Right) dx = 1;

                // SPACE: ANINDA İNDİRME (HARD DROP)
                else if (event.key.code == sf::Keyboard::Space)
                {
                    while (kontrol(a))
                    {
                        for (int i = 0; i < 4; i++) b[i] = a[i];
                        for (int i = 0; i < 4; i++) a[i].y += 1;
                    }
                    for (int i = 0; i < 4; i++) a[i] = b[i];

                    for (int i = 0; i < 4; i++) oyunAlani[a[i].y][a[i].x] = renkNumarasi;

                    // Yeni şekil üretimi
                    n = rand() % 7;
                    renkNumarasi = n + 1;
                    for (int i = 0; i < 4; i++) {
                        a[i].x = sekiller[n][i] % 2 + 4;
                        a[i].y = sekiller[n][i] / 2;
                    }
                    timer = 0;
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) gecikme = 0.05f;
        else gecikme = 0.5f;

        // 1. Sağa ve Sola Hareket
        for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].x += dx; }
        if (!kontrol(a)) { for (int i = 0; i < 4; i++) a[i] = b[i]; }

        // 2. Döndürme
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
            if (!kontrol(a)) { for (int i = 0; i < 4; i++) a[i] = b[i]; }
        }

        // 3. Zamanlayıcı ile Aşağı Düşme
        if (timer > gecikme)
        {
            for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].y += 1; }

            if (!kontrol(a))
            {
                for (int i = 0; i < 4; i++) oyunAlani[b[i].y][b[i].x] = renkNumarasi;

                n = rand() % 7;
                renkNumarasi = n + 1;
                for (int i = 0; i < 4; i++) {
                    a[i].x = sekiller[n][i] % 2 + 4;
                    a[i].y = sekiller[n][i] / 2;
                }
            }
            timer = 0;
        }

        // HAYALET BLOK KOORDİNAT HESAPLAMA
        for (int i = 0; i < 4; i++) hayalet[i] = a[i];
        while (kontrol(hayalet))
        {
            for (int i = 0; i < 4; i++) hayalet[i].y += 1;
        }
        for (int i = 0; i < 4; i++) hayalet[i].y -= 1;


        // --- ÇİZDİRME (RENDER) ---
        window.clear(sf::Color::Black);

        // 1. Izgara ve Sabit Bloklar
        for (int i = 0; i < SATIR; i++) {
            for (int j = 0; j < SUTUN; j++) {
                kare.setOutlineColor(sf::Color(50, 50, 50));
                if (oyunAlani[i][j] == 0) kare.setFillColor(sf::Color(20, 20, 20));
                else kare.setFillColor(renkler[oyunAlani[i][j]]);

                kare.setPosition(j * 32.f, i * 32.f);
                window.draw(kare);
            }
        }

        // 2. Hayalet Blok (Yarı Saydam Gölgelendirme)
        for (int i = 0; i < 4; i++) {
            sf::Color hayaletRengi = renkler[renkNumarasi];
            hayaletRengi.a = 40; // Opaklık ayarı

            kare.setFillColor(hayaletRengi);
            kare.setOutlineColor(renkler[renkNumarasi]);
            kare.setPosition(hayalet[i].x * 32.f, hayalet[i].y * 32.f);
            window.draw(kare);
        }

        // 3. Aktif Gerçek Blok
        for (int i = 0; i < 4; i++) {
            kare.setFillColor(renkler[renkNumarasi]);
            kare.setOutlineColor(sf::Color(50, 50, 50));
            kare.setPosition(a[i].x * 32.f, a[i].y * 32.f);
            window.draw(kare);
        }

        window.display();
    }

    return 0;
}