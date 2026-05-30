#include <SFML/Graphics.hpp>
#include <cstdlib> 
#include <ctime>   
#include <string>

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

// --- RETRO PİKSEL TASARIMLARI ---
const int rakamlar[10][5][3] = {
    {{1,1,1},{1,0,1},{1,0,1},{1,0,1},{1,1,1}}, // 0
    {{0,1,0},{0,1,0},{0,1,0},{0,1,0},{0,1,0}}, // 1
    {{1,1,1},{0,0,1},{1,1,1},{1,0,0},{1,1,1}}, // 2
    {{1,1,1},{0,0,1},{1,1,1},{0,0,1},{1,1,1}}, // 3
    {{1,0,1},{1,0,1},{1,1,1},{0,0,1},{0,0,1}}, // 4
    {{1,1,1},{1,0,0},{1,1,1},{0,0,1},{1,1,1}}, // 5
    {{1,1,1},{1,0,0},{1,1,1},{1,0,1},{1,1,1}}, // 6
    {{1,1,1},{0,0,1},{0,1,0},{0,1,0},{0,1,0}}, // 7
    {{1,1,1},{1,0,1},{1,1,1},{1,0,1},{1,1,1}}, // 8
    {{1,1,1},{1,0,1},{1,1,1},{0,0,1},{1,1,1}}  // 9
};

const int skoryazisi[4][5][3] = {
    {{1,1,1},{1,0,0},{1,1,1},{0,0,1},{1,1,1}}, // S
    {{1,0,1},{1,1,0},{1,0,0},{1,1,0},{1,0,1}}, // K
    {{1,1,1},{1,0,1},{1,0,1},{1,0,1},{1,1,1}}, // O
    {{1,1,0},{1,0,1},{1,1,0},{1,0,1},{1,0,1}}  // R
};

// BİLGİ PANELİNİ ÇİZEN FONKSİYON
void retroPanelCiz(sf::RenderWindow& window, int skor)
{
    float baslangicX = SUTUN * 32.f + 15.f;
    float pikselBoyutu = 6.0f;
    sf::RectangleShape piksel(sf::Vector2f(pikselBoyutu, pikselBoyutu));

    // 1. Ayırıcı Dikey Çizgi 
    sf::RectangleShape dikeyCizgi(sf::Vector2f(2.f, SATIR * 32.f));
    dikeyCizgi.setFillColor(sf::Color(70, 70, 70));
    dikeyCizgi.setPosition(SUTUN * 32.f, 0.f);
    window.draw(dikeyCizgi);

    // 2. Skor ile Sıradaki Şekil Alanını Ayıran Yatay Çizgi
    sf::RectangleShape yatayCizgi(sf::Vector2f(140.f, 2.f));
    yatayCizgi.setFillColor(sf::Color(70, 70, 70));
    yatayCizgi.setPosition(SUTUN * 32.f, 130.f);
    window.draw(yatayCizgi);

    // 3. "SKOR" Başlığını Çiz 
    piksel.setFillColor(sf::Color(180, 180, 180));
    float yaziX = baslangicX;
    float yaziY = 30.f;
    for (int h = 0; h < 4; h++) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 3; j++) {
                if (skoryazisi[h][i][j] == 1) {
                    piksel.setPosition(yaziX + (j * (pikselBoyutu + 1)), yaziY + (i * (pikselBoyutu + 1)));
                    window.draw(piksel);
                }
            }
        }
        yaziX += 30.f;
    }

    // 4. Gerçek Skor Sayılarını Çiz 
    piksel.setFillColor(sf::Color::White);
    std::string sayiStr = std::to_string(skor);

    float sayiX = baslangicX + 27.f;
    float sayiY = 80.f;

    for (char c : sayiStr) {
        int rakam = c - '0';
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 3; j++) {
                if (rakamlar[rakam][i][j] == 1) {
                    piksel.setPosition(sayiX + (j * (pikselBoyutu + 1)), sayiY + (i * (pikselBoyutu + 1)));
                    window.draw(piksel);
                }
            }
        }
        sayiX += 28.f;
    }
}

// ÇARPIŞMA VE ZEMİN KONTROLÜ
bool kontrol(Nokta* parca)
{
    for (int i = 0; i < 4; i++)
    {
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

    sf::RenderWindow window(sf::VideoMode(SUTUN * 32 + 140, SATIR * 32), "SFML Tetris");

    sf::RectangleShape kare(sf::Vector2f(32.0f, 32.0f));
    kare.setOutlineThickness(-1.f);

    sf::Color renkler[8] = {
        sf::Color::Black, sf::Color::Cyan, sf::Color::Red, sf::Color::Green,
        sf::Color(128, 0, 128), sf::Color(255, 165, 0), sf::Color::Blue, sf::Color::Yellow
    };

    int n = rand() % 7;
    int renkNumarasi = n + 1;

    int siradakiN = rand() % 7;
    int siradakiRenkNumarasi = siradakiN + 1;

    for (int i = 0; i < 4; i++) {
        a[i].x = sekiller[n][i] % 2 + 4;
        a[i].y = sekiller[n][i] / 2;
    }

    sf::Clock saat;
    float timer = 0, gecikme = 0.5f;
    float kilitTimer = 0.0f; // YENİ: Lock Delay Sayacı
    int skor = 0;

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

                    n = siradakiN;
                    renkNumarasi = siradakiRenkNumarasi;
                    for (int i = 0; i < 4; i++) {
                        a[i].x = sekiller[n][i] % 2 + 4;
                        a[i].y = sekiller[n][i] / 2;
                    }

                    siradakiN = rand() % 7;
                    siradakiRenkNumarasi = siradakiN + 1;

                    timer = 0;
                    kilitTimer = 0.0f; // Hard drop sonrası kilit sayacını sıfırla
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

        // --- 3. LOCK DELAY (KİLİTLENME GECİKMESİ) VE DÜŞÜŞ ---
        bool asagiInilebilir = true;
        for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].y += 1; }
        if (!kontrol(a)) asagiInilebilir = false; // Bir alt satır doluysa inilemez
        for (int i = 0; i < 4; i++) a[i] = b[i]; // Testi geri al

        if (!asagiInilebilir)
        {
            // Parça zemine veya başka bir bloğa değiyorsa süreyi başlat
            kilitTimer += zaman;

            // 0.4 saniye (400ms) boyunca oyuncunun kaydırmasına izin ver
            if (kilitTimer > 0.4f)
            {
                for (int i = 0; i < 4; i++) oyunAlani[a[i].y][a[i].x] = renkNumarasi;

                n = siradakiN;
                renkNumarasi = siradakiRenkNumarasi;
                for (int i = 0; i < 4; i++) {
                    a[i].x = sekiller[n][i] % 2 + 4;
                    a[i].y = sekiller[n][i] / 2;
                }

                siradakiN = rand() % 7;
                siradakiRenkNumarasi = siradakiN + 1;

                timer = 0;
                kilitTimer = 0.0f;
            }
        }
        else
        {
            // Eğer blok havadaysa (veya oyuncu sağ/sola çekip boşluğa getirdiyse)
            // Kilitlenme süresini iptal edip sayacı sıfırla
            kilitTimer = 0.0f;

            // Normal zamanlayıcı ile aşağı düşme
            if (timer > gecikme)
            {
                for (int i = 0; i < 4; i++) a[i].y += 1;
                timer = 0;
            }
        }

        // 4. SATIR SİLME VE SKOR
        for (int i = SATIR - 1; i >= 0; i--)
        {
            bool satirDolu = true;
            for (int j = 0; j < SUTUN; j++)
            {
                if (oyunAlani[i][j] == 0) {
                    satirDolu = false;
                    break;
                }
            }

            if (satirDolu)
            {
                skor += 10;
                for (int k = i; k > 0; k--)
                    for (int j = 0; j < SUTUN; j++)
                        oyunAlani[k][j] = oyunAlani[k - 1][j];
                for (int j = 0; j < SUTUN; j++) oyunAlani[0][j] = 0;
                i++;
            }
        }

        // Hayalet Blok Koordinat Hesaplama
        for (int i = 0; i < 4; i++) hayalet[i] = a[i];
        while (kontrol(hayalet))
            for (int i = 0; i < 4; i++) hayalet[i].y += 1;
        for (int i = 0; i < 4; i++) hayalet[i].y -= 1;


        // --- ÇİZDİRME (RENDER) ---
        window.clear(sf::Color::Black);

        for (int i = 0; i < SATIR; i++) {
            for (int j = 0; j < SUTUN; j++) {
                kare.setOutlineColor(sf::Color(50, 50, 50));
                if (oyunAlani[i][j] == 0) kare.setFillColor(sf::Color(20, 20, 20));
                else kare.setFillColor(renkler[oyunAlani[i][j]]);

                kare.setPosition(j * 32.f, i * 32.f);
                window.draw(kare);
            }
        }

        for (int i = 0; i < 4; i++) {
            sf::Color hayaletRengi = renkler[renkNumarasi];
            hayaletRengi.a = 40;
            kare.setFillColor(hayaletRengi);
            kare.setOutlineColor(renkler[renkNumarasi]);
            kare.setPosition(hayalet[i].x * 32.f, hayalet[i].y * 32.f);
            window.draw(kare);
        }

        for (int i = 0; i < 4; i++) {
            kare.setFillColor(renkler[renkNumarasi]);
            kare.setOutlineColor(sf::Color(50, 50, 50));
            kare.setPosition(a[i].x * 32.f, a[i].y * 32.f);
            window.draw(kare);
        }

        // Panel ve Skor Metni
        retroPanelCiz(window, skor);

        // Sıradaki Şekil Göstergesi
        float siradakiOffsetX = SUTUN * 32.f + 40.f;
        float siradakiOffsetY = 180.f;

        for (int i = 0; i < 4; i++) {
            int siradakiX = sekiller[siradakiN][i] % 2;
            int siradakiY = sekiller[siradakiN][i] / 2;

            kare.setFillColor(renkler[siradakiRenkNumarasi]);
            kare.setOutlineColor(sf::Color(50, 50, 50));
            kare.setPosition(siradakiOffsetX + (siradakiX * 32.f), siradakiOffsetY + (siradakiY * 32.f));
            window.draw(kare);
        }

        window.display();
    }

    return 0;
}