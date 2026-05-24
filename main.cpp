#include <SFML/Graphics.hpp>
#include <time.h>

const int SATIR = 20;
const int SUTUN = 10;
int oyunAlani[SATIR][SUTUN] = { 0 };

// 7 farklı Tetris şeklinin matris koordinatları
int sekiller[7][4] = {
    {1,3,5,7}, // I Şekli
    {2,4,5,7}, // Z Şekli
    {3,5,4,6}, // S Şekli
    {3,5,4,7}, // T Şekli
    {2,3,5,7}, // L Şekli
    {3,5,7,6}, // J Şekli
    {2,3,4,5}  // O Şekli
};

struct Nokta { int x, y; } a[4];

int main()
{
    srand(time(0)); // Rastgele şekil üretimi için saati başlat
    sf::RenderWindow window(sf::VideoMode(SUTUN * 32, SATIR * 32), "SFML Tetris");

    sf::RectangleShape kare(sf::Vector2f(32.0f, 32.0f));
    kare.setOutlineThickness(-1.f);
    kare.setOutlineColor(sf::Color(50, 50, 50));

    // Rastgele bir şekil seçip başlangıç koordinatlarını ayarla
    int n = rand() % 7;
    for (int i = 0; i < 4; i++) {
        a[i].x = sekiller[n][i] % 2 + 4; // Ekranın ortasından başlasın
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
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // AŞAĞI DÜŞME MANTIĞI (Çarpışma kontrolü yok)
        if (timer > gecikme)
        {
            for (int i = 0; i < 4; i++) {
                a[i].y += 1; // Sadece Y ekseninde 1 birim aşağı kaydır
            }
            timer = 0;
        }

        // --- ÇİZDİRME (RENDER) KISMI ---
        window.clear(sf::Color::Black);

        // 1. Arka plan ızgarasını çiz
        for (int i = 0; i < SATIR; i++) {
            for (int j = 0; j < SUTUN; j++) {
                kare.setFillColor(sf::Color(20, 20, 20)); // Boşluk rengi
                kare.setPosition(j * 32.f, i * 32.f);
                window.draw(kare);
            }
        }

        // 2. Düşen şekli çiz
        for (int i = 0; i < 4; i++) {
            kare.setFillColor(sf::Color::Cyan); // Şimdilik tüm şekiller turkuaz
            kare.setPosition(a[i].x * 32.f, a[i].y * 32.f);
            window.draw(kare);
        }

        window.display();
    }

    return 0;
}