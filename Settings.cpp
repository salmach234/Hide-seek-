#include "Settings.h"
#include "GameConfig.h"

static const float BTN_SIZE = 40.f;
static const float BAR_W = 300.f;
static const float BAR_H = 18.f;
static const float ROW1_Y = 200.f;
static const float ROW2_Y = 330.f;
static const float LEFT_X = 180.f;

Settings::Settings(sf::Font& font)
    : m_font(font),
    m_musicVol(70),
    m_effectsVol(70),
    m_wantsBack(false),
    m_title(font, "SETTINGS", 44),
    m_lblMusic(font, "Music Volume", 24),
    m_valMusic(font, "70%", 22),
    m_btnMusicMinus(sf::Vector2f(BTN_SIZE, BTN_SIZE)),
    m_btnMusicPlus(sf::Vector2f(BTN_SIZE, BTN_SIZE)),
    m_txtMusicMinus(font, "-", 28),
    m_txtMusicPlus(font, "+", 28),
    m_barMusicBg(sf::Vector2f(BAR_W, BAR_H)),
    m_barMusicFill(sf::Vector2f(BAR_W * 0.7f, BAR_H)),
    m_lblEffects(font, "Effects Volume", 24),
    m_valEffects(font, "70%", 22),
    m_btnEffMinus(sf::Vector2f(BTN_SIZE, BTN_SIZE)),
    m_btnEffPlus(sf::Vector2f(BTN_SIZE, BTN_SIZE)),
    m_txtEffMinus(font, "-", 28),
    m_txtEffPlus(font, "+", 28),
    m_barEffBg(sf::Vector2f(BAR_W, BAR_H)),
    m_barEffFill(sf::Vector2f(BAR_W * 0.7f, BAR_H)),
    m_btnBack(sf::Vector2f(120.f, 44.f)),
    m_txtBack(font, "<  Back", 22)
{
    // Titre centré
    m_title.setFillColor(sf::Color::Yellow);
    sf::FloatRect tb = m_title.getLocalBounds();
    m_title.setOrigin({ tb.position.x + tb.size.x / 2.f,
                        tb.position.y + tb.size.y / 2.f });
    m_title.setPosition({ WIN_W / 2.f, 90.f });

    // ---- Rangée Musique ----
    m_lblMusic.setFillColor(sf::Color::White);
    m_lblMusic.setPosition({ LEFT_X, ROW1_Y });

    float minX = LEFT_X;
    float barX = minX + BTN_SIZE + 14.f;
    float plusX = barX + BAR_W + 14.f;

    // Bouton -
    m_btnMusicMinus.setFillColor(sf::Color(60, 60, 100));
    m_btnMusicMinus.setOutlineColor(sf::Color::White);
    m_btnMusicMinus.setOutlineThickness(2.f);
    m_btnMusicMinus.setPosition({ minX, ROW1_Y + 40.f });

    m_txtMusicMinus.setFillColor(sf::Color::White);
    centerText(m_txtMusicMinus,
        minX + BTN_SIZE / 2.f,
        ROW1_Y + 40.f + BTN_SIZE / 2.f);

    // Barre
    m_barMusicBg.setFillColor(sf::Color(60, 60, 70));
    m_barMusicBg.setOutlineColor(sf::Color(120, 120, 140));
    m_barMusicBg.setOutlineThickness(2.f);
    m_barMusicBg.setPosition({ barX, ROW1_Y + 40.f + (BTN_SIZE - BAR_H) / 2.f });

    m_barMusicFill.setFillColor(sf::Color(0, 180, 255));
    m_barMusicFill.setPosition({ barX, ROW1_Y + 40.f + (BTN_SIZE - BAR_H) / 2.f });

    // Bouton +
    m_btnMusicPlus.setFillColor(sf::Color(60, 60, 100));
    m_btnMusicPlus.setOutlineColor(sf::Color::White);
    m_btnMusicPlus.setOutlineThickness(2.f);
    m_btnMusicPlus.setPosition({ plusX, ROW1_Y + 40.f });

    m_txtMusicPlus.setFillColor(sf::Color::White);
    centerText(m_txtMusicPlus,
        plusX + BTN_SIZE / 2.f,
        ROW1_Y + 40.f + BTN_SIZE / 2.f);

    // Valeur
    m_valMusic.setFillColor(sf::Color(200, 200, 100));
    m_valMusic.setPosition({ plusX + BTN_SIZE + 14.f, ROW1_Y + 44.f });

    // ---- Rangée Effets ----
    m_lblEffects.setFillColor(sf::Color::White);
    m_lblEffects.setPosition({ LEFT_X, ROW2_Y });

    m_btnEffMinus.setFillColor(sf::Color(60, 60, 100));
    m_btnEffMinus.setOutlineColor(sf::Color::White);
    m_btnEffMinus.setOutlineThickness(2.f);
    m_btnEffMinus.setPosition({ minX, ROW2_Y + 40.f });

    m_txtEffMinus.setFillColor(sf::Color::White);
    centerText(m_txtEffMinus,
        minX + BTN_SIZE / 2.f,
        ROW2_Y + 40.f + BTN_SIZE / 2.f);

    m_barEffBg.setFillColor(sf::Color(60, 60, 70));
    m_barEffBg.setOutlineColor(sf::Color(120, 120, 140));
    m_barEffBg.setOutlineThickness(2.f);
    m_barEffBg.setPosition({ barX, ROW2_Y + 40.f + (BTN_SIZE - BAR_H) / 2.f });

    m_barEffFill.setFillColor(sf::Color(255, 160, 0));
    m_barEffFill.setPosition({ barX, ROW2_Y + 40.f + (BTN_SIZE - BAR_H) / 2.f });

    m_btnEffPlus.setFillColor(sf::Color(60, 60, 100));
    m_btnEffPlus.setOutlineColor(sf::Color::White);
    m_btnEffPlus.setOutlineThickness(2.f);
    m_btnEffPlus.setPosition({ plusX, ROW2_Y + 40.f });

    m_txtEffPlus.setFillColor(sf::Color::White);
    centerText(m_txtEffPlus,
        plusX + BTN_SIZE / 2.f,
        ROW2_Y + 40.f + BTN_SIZE / 2.f);

    m_valEffects.setFillColor(sf::Color(200, 200, 100));
    m_valEffects.setPosition({ plusX + BTN_SIZE + 14.f, ROW2_Y + 44.f });

    // ---- Bouton Back ----
    m_btnBack.setFillColor(sf::Color(50, 50, 80));
    m_btnBack.setOutlineColor(sf::Color::White);
    m_btnBack.setOutlineThickness(2.f);
    m_btnBack.setPosition({ 30.f, 20.f });

    m_txtBack.setFillColor(sf::Color::White);
    sf::FloatRect bb = m_txtBack.getLocalBounds();
    m_txtBack.setOrigin({ bb.position.x, bb.position.y + bb.size.y / 2.f });
    m_txtBack.setPosition({ 38.f, 20.f + 22.f });

    refreshBars();
}

void Settings::centerText(sf::Text& text, float cx, float cy)
{
    sf::FloatRect b = text.getLocalBounds();
    text.setOrigin({ b.position.x + b.size.x / 2.f,
                     b.position.y + b.size.y / 2.f });
    text.setPosition({ cx, cy });
}

void Settings::clampVolumes()
{
    if (m_musicVol < 0)   m_musicVol = 0;
    if (m_musicVol > 100) m_musicVol = 100;
    if (m_effectsVol < 0)   m_effectsVol = 0;
    if (m_effectsVol > 100) m_effectsVol = 100;
}

void Settings::refreshBars()
{
    float mRatio = m_musicVol / 100.f;
    float eRatio = m_effectsVol / 100.f;

    m_barMusicFill.setSize({ BAR_W * mRatio, BAR_H });
    m_barEffFill.setSize({ BAR_W * eRatio, BAR_H });

    m_valMusic.setString(std::to_string(m_musicVol) + "%");
    m_valEffects.setString(std::to_string(m_effectsVol) + "%");
}

void Settings::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    if (const auto* key = event.getIf<sf::Event::KeyPressed>())
    {
        if (key->code == sf::Keyboard::Key::Escape)
            m_wantsBack = true;
    }

    if (const auto* c = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (c->button != sf::Mouse::Button::Left) return;

        sf::Vector2i mp = sf::Mouse::getPosition(window);
        sf::Vector2f pos = window.mapPixelToCoords(mp);

        if (m_btnBack.getGlobalBounds().contains(pos))
        {
            m_wantsBack = true;
            return;
        }

        if (m_btnMusicMinus.getGlobalBounds().contains(pos))
        {
            m_musicVol -= 10; clampVolumes(); refreshBars();
        }

        if (m_btnMusicPlus.getGlobalBounds().contains(pos))
        {
            m_musicVol += 10; clampVolumes(); refreshBars();
        }

        if (m_btnEffMinus.getGlobalBounds().contains(pos))
        {
            m_effectsVol -= 10; clampVolumes(); refreshBars();
        }

        if (m_btnEffPlus.getGlobalBounds().contains(pos))
        {
            m_effectsVol += 10; clampVolumes(); refreshBars();
        }
    }
}

void Settings::update(const sf::RenderWindow& /*window*/) {}

void Settings::draw(sf::RenderWindow& window)
{
    window.draw(m_title);

    // Musique
    window.draw(m_lblMusic);
    window.draw(m_btnMusicMinus);
    window.draw(m_txtMusicMinus);
    window.draw(m_barMusicBg);
    window.draw(m_barMusicFill);
    window.draw(m_btnMusicPlus);
    window.draw(m_txtMusicPlus);
    window.draw(m_valMusic);

    // Effets
    window.draw(m_lblEffects);
    window.draw(m_btnEffMinus);
    window.draw(m_txtEffMinus);
    window.draw(m_barEffBg);
    window.draw(m_barEffFill);
    window.draw(m_btnEffPlus);
    window.draw(m_txtEffPlus);
    window.draw(m_valEffects);

    // Back
    window.draw(m_btnBack);
    window.draw(m_txtBack);
}

bool Settings::wantsBack()     const { return m_wantsBack; }
void Settings::resetBack() { m_wantsBack = false; }
int  Settings::getMusicVolume()  const { return m_musicVol; }
int  Settings::getEffectsVolume() const { return m_effectsVol; }