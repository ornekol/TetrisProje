#include <SFML/Graphics.hpp>
#include <cstdlib> 
#include <ctime>   
#include <string>
#include <vector> 

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

// OYUN ALANI KAYDIRMA OFSETİ (Sol panel eklendiği için Izgara merkezde)
const float GRID_X = 120.f;
const float SAG_PANEL_X = 440.f; // 120 (Sol) + 320 (Oyun) = 440

// --- MODERN TETRIS: KUYRUK VE 7-BAG ALGORİTMASI ---
std::vector<int> siradakiParcalar;

void torbaDoldur()
{
    std::vector<int> yeniTorba;
    for (int i = 0; i < 7; i++) yeniTorba.push_back(i);

    for (int i = 6; i > 0; i--) {
        int j = rand() % (i + 1);
        std::swap(yeniTorba[i], yeniTorba[j]);
    }

    for (int p : yeniTorba) {
        siradakiParcalar.push_back(p);
    }
}

int parcaCek()
{
    if (siradakiParcalar.size() <= 7) {
        torbaDoldur();
    }
    int secilenParca = siradakiParcalar.front();
    siradakiParcalar.erase(siradakiParcalar.begin());
    return secilenParca;
}

// --- RETRO PİKSEL TASARIMLARI ---
const int rakamlar[10][5][3] = {
    {{1,1,1},{1,0,1},{1,0,1},{1,0,1},{1,1,1}},
    {{0,1,0},{0,1,0},{0,1,0},{0,1,0},{0,1,0}},
    {{1,1,1},{0,0,1},{1,1,1},{1,0,0},{1,1,1}},
    {{1,1,1},{0,0,1},{1,1,1},{0,0,1},{1,1,1}},
    {{1,0,1},{1,0,1},{1,1,1},{0,0,1},{0,0,1}},
    {{1,1,1},{1,0,0},{1,1,1},{0,0,1},{1,1,1}},
    {{1,1,1},{1,0,0},{1,1,1},{1,0,1},{1,1,1}},
    {{1,1,1},{0,0,1},{0,1,0},{0,1,0},{0,1,0}},
    {{1,1,1},{1,0,1},{1,1,1},{1,0,1},{1,1,1}},
    {{1,1,1},{1,0,1},{1,1,1},{0,0,1},{1,1,1}}
};

const int skoryazisi[4][5][3] = {
    {{1,1,1},{1,0,0},{1,1,1},{0,0,1},{1,1,1}},
    {{1,0,1},{1,1,0},{1,0,0},{1,1,0},{1,0,1}},
    {{1,1,1},{1,0,1},{1,0,1},{1,0,1},{1,1,1}},
    {{1,1,0},{1,0,1},{1,1,0},{1,0,1},{1,0,1}}
};

const int levelYazisi[5][5][3] = {
    {{1,0,0},{1,0,0},{1,0,0},{1,0,0},{1,1,1}},
    {{1,1,1},{1,0,0},{1,1,0},{1,0,0},{1,1,1}},
    {{1,0,1},{1,0,1},{1,0,1},{1,0,1},{0,1,0}},
    {{1,1,1},{1,0,0},{1,1,0},{1,0,0},{1,1,1}},
    {{1,0,0},{1,0,0},{1,0,0},{1,0,0},{1,1,1}}
};

const int gameYazisi[4][5][3] = {
    {{1,1,1},{1,0,0},{1,0,1},{1,0,1},{1,1,1}},
    {{0,1,0},{1,0,1},{1,1,1},{1,0,1},{1,0,1}},
    {{1,0,1},{1,1,1},{1,0,1},{1,0,1},{1,0,1}},
    {{1,1,1},{1,0,0},{1,1,0},{1,0,0},{1,1,1}}
};

const int overYazisi[4][5][3] = {
    {{1,1,1},{1,0,1},{1,0,1},{1,0,1},{1,1,1}},
    {{1,0,1},{1,0,1},{1,0,1},{1,0,1},{0,1,0}},
    {{1,1,1},{1,0,0},{1,1,0},{1,0,0},{1,1,1}},
    {{1,1,0},{1,0,1},{1,1,0},{1,0,1},{1,0,1}}
};

// YENİ: HOLD YAZISI MATRİSİ (H, O, L, D)
const int holdYazisi[4][5][3] = {
    {{1,0,1},{1,0,1},{1,1,1},{1,0,1},{1,0,1}},
    {{1,1,1},{1,0,1},{1,0,1},{1,0,1},{1,1,1}},
    {{1,0,0},{1,0,0},{1,0,0},{1,0,0},{1,1,1}},
    {{1,1,0},{1,0,1},{1,0,1},{1,0,1},{1,1,0}}
};

// YENİ: SOL PANEL (KASA) ÇİZİM FONKSİYONU
void solPanelCiz(sf::RenderWindow& window, int holdParca, sf::Color renkler[])
{
    float pikselBoyutu = 6.0f;
    sf::RectangleShape piksel(sf::Vector2f(pikselBoyutu, pikselBoyutu));

    // Sol panel ayırıcı çizgisi (Oyun alanıyla arasına)
    sf::RectangleShape dikeyCizgi(sf::Vector2f(2.f, SATIR * 32.f));
    dikeyCizgi.setFillColor(sf::Color(70, 70, 70));
    dikeyCizgi.setPosition(GRID_X - 2.f, 0.f);
    window.draw(dikeyCizgi);

    sf::RectangleShape yatayCizgi(sf::Vector2f(GRID_X - 2.f, 2.f));
    yatayCizgi.setFillColor(sf::Color(70, 70, 70));
    yatayCizgi.setPosition(0.f, 130.f);
    window.draw(yatayCizgi);

    // "HOLD" Başlığı
    piksel.setFillColor(sf::Color(180, 180, 180));
    float yaziX = 15.f;
    float yaziY = 30.f;
    for (int h = 0; h < 4; h++) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 3; j++) {
                if (holdYazisi[h][i][j] == 1) {
                    piksel.setPosition(yaziX + (j * (pikselBoyutu + 1)), yaziY + (i * (pikselBoyutu + 1)));
                    window.draw(piksel);
                }
            }
        }
        yaziX += 26.f;
    }

    // Kasadaki parçayı çizdir (Eğer varsa)
    if (holdParca != -1) {
        float kucukKareBoyut = 14.f;
        sf::RectangleShape kucukKare(sf::Vector2f(kucukKareBoyut, kucukKareBoyut));
        kucukKare.setOutlineThickness(-1.f);
        kucukKare.setFillColor(renkler[holdParca + 1]);
        kucukKare.setOutlineColor(sf::Color(50, 50, 50));

        float holdOffsetX = 35.f;
        if (holdParca == 0 || holdParca == 6) holdOffsetX -= 7.f;

        for (int i = 0; i < 4; i++) {
            int pX = sekiller[holdParca][i] % 2;
            int pY = sekiller[holdParca][i] / 2;
            kucukKare.setPosition(holdOffsetX + (pX * kucukKareBoyut), 70.f + (pY * kucukKareBoyut));
            window.draw(kucukKare);
        }
    }
}

// SAĞ PANEL ÇİZİM FONKSİYONU
void sagPanelCiz(sf::RenderWindow& window, int skor, int level)
{
    float baslangicX = SAG_PANEL_X + 15.f;
    float pikselBoyutu = 6.0f;
    sf::RectangleShape piksel(sf::Vector2f(pikselBoyutu, pikselBoyutu));

    sf::RectangleShape dikeyCizgi(sf::Vector2f(2.f, SATIR * 32.f));
    dikeyCizgi.setFillColor(sf::Color(70, 70, 70));
    dikeyCizgi.setPosition(SAG_PANEL_X, 0.f);
    window.draw(dikeyCizgi);

    sf::RectangleShape yatayCizgi(sf::Vector2f(140.f, 2.f));
    yatayCizgi.setFillColor(sf::Color(70, 70, 70));
    yatayCizgi.setPosition(SAG_PANEL_X, 130.f);
    window.draw(yatayCizgi);

    // SKOR Yazısı
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

    // Skor Rakamları
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

    // LEVEL Yazısı
    piksel.setFillColor(sf::Color(180, 180, 180));
    float lvlYaziX = baslangicX - 5.f;
    float lvlYaziY = 470.f;
    for (int h = 0; h < 5; h++) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 3; j++) {
                if (levelYazisi[h][i][j] == 1) {
                    piksel.setPosition(lvlYaziX + (j * (pikselBoyutu + 1)), lvlYaziY + (i * (pikselBoyutu + 1)));
                    window.draw(piksel);
                }
            }
        }
        lvlYaziX += 24.f;
    }

    // Level Rakamları
    piksel.setFillColor(sf::Color::Yellow);
    std::string levelStr = std::to_string(level);
    float lvlSayiX = baslangicX + 35.f;
    float lvlSayiY = 520.f;
    for (char c : levelStr) {
        int rakam = c - '0';
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 3; j++) {
                if (rakamlar[rakam][i][j] == 1) {
                    piksel.setPosition(lvlSayiX + (j * (pikselBoyutu + 1)), lvlSayiY + (i * (pikselBoyutu + 1)));
                    window.draw(piksel);
                }
            }
        }
        lvlSayiX += 28.f;
    }
}

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

    // Genişlik: 120 (Sol) + 320 (Oyun) + 140 (Sağ) = 580 Piksel
    sf::RenderWindow window(sf::VideoMode(580, SATIR * 32), "SFML Tetris");

    sf::RectangleShape kare(sf::Vector2f(32.0f, 32.0f));
    kare.setOutlineThickness(-1.f);

    float kucukKareBoyut = 14.f;
    sf::RectangleShape kucukKare(sf::Vector2f(kucukKareBoyut, kucukKareBoyut));
    kucukKare.setOutlineThickness(-1.f);

    sf::Color renkler[8] = {
        sf::Color::Black, sf::Color::Cyan, sf::Color::Red, sf::Color::Green,
        sf::Color(128, 0, 128), sf::Color(255, 165, 0), sf::Color::Blue, sf::Color::Yellow
    };

    torbaDoldur();
    int n = parcaCek();
    int renkNumarasi = n + 1;

    for (int i = 0; i < 4; i++) {
        a[i].x = sekiller[n][i] % 2 + 4;
        a[i].y = sekiller[n][i] / 2;
    }

    sf::Clock saat;
    float timer = 0, gecikme = 0.5f;
    float kilitTimer = 0.0f;

    int skor = 0;
    int level = 1;
    bool gameOver = false;

    // YENİ: HOLD MEKANİĞİ DEĞİŞKENLERİ
    int holdParca = -1; // -1 kasanın boş olduğunu belirtir
    bool holdKullanildi = false;

    while (window.isOpen())
    {
        float zaman = saat.getElapsedTime().asSeconds();
        saat.restart();
        timer += zaman;

        level = (skor / 100) + 1;
        float bazGecikme = 0.5f - ((level - 1) * 0.04f);
        if (bazGecikme < 0.05f) bazGecikme = 0.05f;

        sf::Event event;
        int dx = 0;
        bool dondur = false;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (gameOver)
                {
                    if (event.key.code == sf::Keyboard::R)
                    {
                        for (int i = 0; i < SATIR; i++)
                            for (int j = 0; j < SUTUN; j++)
                                oyunAlani[i][j] = 0;

                        skor = 0; level = 1;
                        gameOver = false;
                        timer = 0; kilitTimer = 0.0f;

                        holdParca = -1; // Kasayı da sıfırla
                        holdKullanildi = false;

                        siradakiParcalar.clear();
                        torbaDoldur();
                        n = parcaCek();
                        renkNumarasi = n + 1;
                        for (int i = 0; i < 4; i++) {
                            a[i].x = sekiller[n][i] % 2 + 4;
                            a[i].y = sekiller[n][i] / 2;
                        }
                    }
                }
                else
                {
                    if (event.key.code == sf::Keyboard::Up) dondur = true;
                    else if (event.key.code == sf::Keyboard::Left) dx = -1;
                    else if (event.key.code == sf::Keyboard::Right) dx = 1;

                    // YENİ: 'C' Tuşu ile Kasa Kullanımı
                    else if (event.key.code == sf::Keyboard::C)
                    {
                        if (!holdKullanildi) // Sadece yere düşene kadar 1 kez basılabilir
                        {
                            if (holdParca == -1) {
                                // Kasa boşsa: Elindekini koy, yenisini çek
                                holdParca = n;
                                n = parcaCek();
                            }
                            else {
                                // Kasada varsa: Elindekiyle takas et
                                int temp = n;
                                n = holdParca;
                                holdParca = temp;
                            }

                            renkNumarasi = n + 1;
                            for (int i = 0; i < 4; i++) {
                                a[i].x = sekiller[n][i] % 2 + 4;
                                a[i].y = sekiller[n][i] / 2;
                            }

                            if (!kontrol(a)) gameOver = true;

                            holdKullanildi = true;
                            timer = 0;
                            kilitTimer = 0.0f;
                        }
                    }

                    else if (event.key.code == sf::Keyboard::Space)
                    {
                        while (kontrol(a))
                        {
                            for (int i = 0; i < 4; i++) b[i] = a[i];
                            for (int i = 0; i < 4; i++) a[i].y += 1;
                        }
                        for (int i = 0; i < 4; i++) a[i] = b[i];

                        for (int i = 0; i < 4; i++) oyunAlani[a[i].y][a[i].x] = renkNumarasi;

                        n = parcaCek();
                        renkNumarasi = n + 1;
                        for (int i = 0; i < 4; i++) {
                            a[i].x = sekiller[n][i] % 2 + 4;
                            a[i].y = sekiller[n][i] / 2;
                        }

                        if (!kontrol(a)) gameOver = true;

                        timer = 0;
                        kilitTimer = 0.0f;
                        holdKullanildi = false; // Taş yere oturduğunda kasa hakkı yenilenir
                    }
                }
            }
        }

        if (!gameOver)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) gecikme = 0.05f;
            else gecikme = bazGecikme;

            for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].x += dx; }
            if (!kontrol(a)) { for (int i = 0; i < 4; i++) a[i] = b[i]; }

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

            bool asagiInilebilir = true;
            for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].y += 1; }
            if (!kontrol(a)) asagiInilebilir = false;
            for (int i = 0; i < 4; i++) a[i] = b[i];

            if (!asagiInilebilir)
            {
                kilitTimer += zaman;

                if (kilitTimer > 0.4f)
                {
                    for (int i = 0; i < 4; i++) oyunAlani[a[i].y][a[i].x] = renkNumarasi;

                    n = parcaCek();
                    renkNumarasi = n + 1;
                    for (int i = 0; i < 4; i++) {
                        a[i].x = sekiller[n][i] % 2 + 4;
                        a[i].y = sekiller[n][i] / 2;
                    }

                    if (!kontrol(a)) gameOver = true;

                    timer = 0;
                    kilitTimer = 0.0f;
                    holdKullanildi = false; // Taş kilitlendi, kasa hakkı geri geldi
                }
            }
            else
            {
                kilitTimer = 0.0f;

                if (timer > gecikme)
                {
                    for (int i = 0; i < 4; i++) a[i].y += 1;
                    timer = 0;
                }
            }

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

            for (int i = 0; i < 4; i++) hayalet[i] = a[i];
            while (kontrol(hayalet))
                for (int i = 0; i < 4; i++) hayalet[i].y += 1;
            for (int i = 0; i < 4; i++) hayalet[i].y -= 1;
        }

        // --- ÇİZDİRME (RENDER) ---
        window.clear(sf::Color::Black);

        // OYUN ALANI (GRID_X kadar sağa kaydırılarak çiziliyor)
        for (int i = 0; i < SATIR; i++) {
            for (int j = 0; j < SUTUN; j++) {
                kare.setOutlineColor(sf::Color(50, 50, 50));
                if (oyunAlani[i][j] == 0) kare.setFillColor(sf::Color(20, 20, 20));
                else kare.setFillColor(renkler[oyunAlani[i][j]]);

                kare.setPosition(GRID_X + j * 32.f, i * 32.f);
                window.draw(kare);
            }
        }

        if (!gameOver)
        {
            for (int i = 0; i < 4; i++) {
                sf::Color hayaletRengi = renkler[renkNumarasi];
                hayaletRengi.a = 40;
                kare.setFillColor(hayaletRengi);
                kare.setOutlineColor(renkler[renkNumarasi]);
                kare.setPosition(GRID_X + hayalet[i].x * 32.f, hayalet[i].y * 32.f);
                window.draw(kare);
            }

            for (int i = 0; i < 4; i++) {
                kare.setFillColor(renkler[renkNumarasi]);
                kare.setOutlineColor(sf::Color(50, 50, 50));
                kare.setPosition(GRID_X + a[i].x * 32.f, a[i].y * 32.f);
                window.draw(kare);
            }

            // SIRADAKİ 6 ŞEKLİ MİNYATÜR ÇİZDİRME (Sağ Panel)
            float siradakiBaslangicY = 150.f;

            for (int k = 0; k < 6; k++) {
                int siradakiN = siradakiParcalar[k];
                int siradakiRenkNumarasi = siradakiN + 1;

                kucukKare.setFillColor(renkler[siradakiRenkNumarasi]);
                kucukKare.setOutlineColor(sf::Color(50, 50, 50));

                float siradakiOffsetX = SAG_PANEL_X + 55.f;
                if (siradakiN == 0 || siradakiN == 6) siradakiOffsetX -= 7.f;

                for (int i = 0; i < 4; i++) {
                    int pX = sekiller[siradakiN][i] % 2;
                    int pY = sekiller[siradakiN][i] / 2;
                    kucukKare.setPosition(siradakiOffsetX + (pX * kucukKareBoyut), siradakiBaslangicY + (k * 48.f) + (pY * kucukKareBoyut));
                    window.draw(kucukKare);
                }
            }
        }

        // PANELLERİ ÇAĞIR
        solPanelCiz(window, holdParca, renkler);
        sagPanelCiz(window, skor, level);

        if (gameOver)
        {
            // Oyun alanına kırmızı filtre (Sadece ızgaranın üzerine)
            sf::RectangleShape kirmiziEkran(sf::Vector2f(SUTUN * 32.f, SATIR * 32.f));
            kirmiziEkran.setFillColor(sf::Color(150, 0, 0, 150));
            kirmiziEkran.setPosition(GRID_X, 0.f);
            window.draw(kirmiziEkran);

            sf::RectangleShape goPiksel(sf::Vector2f(5.0f, 5.0f));
            goPiksel.setFillColor(sf::Color::Red);

            float goBaslangicX = SAG_PANEL_X + 25.f;
            float gameY = 250.f;
            float overY = 300.f;

            float yaziX = goBaslangicX;
            for (int h = 0; h < 4; h++) {
                for (int i = 0; i < 5; i++) {
                    for (int j = 0; j < 3; j++) {
                        if (gameYazisi[h][i][j] == 1) {
                            goPiksel.setPosition(yaziX + (j * 6.f), gameY + (i * 6.f));
                            window.draw(goPiksel);
                        }
                    }
                }
                yaziX += 24.f;
            }

            yaziX = goBaslangicX;
            for (int h = 0; h < 4; h++) {
                for (int i = 0; i < 5; i++) {
                    for (int j = 0; j < 3; j++) {
                        if (overYazisi[h][i][j] == 1) {
                            goPiksel.setPosition(yaziX + (j * 6.f), overY + (i * 6.f));
                            window.draw(goPiksel);
                        }
                    }
                }
                yaziX += 24.f;
            }
        }

        window.display();
    }

    return 0;
}