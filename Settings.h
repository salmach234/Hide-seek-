#pragma once
#include <SFML/Graphics.hpp>

class Settings
{
public:
    explicit Settings(sf::Font& font);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    void update(const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

    bool wantsBack() const;
    void resetBack();

    int getMusicVolume()  const;
    int getEffectsVolume() const;

private:
    sf::Font& m_font;

    int  m_musicVol;
    int  m_effectsVol;
    bool m_wantsBack;

    // Titre
    sf::Text m_title;

    // Rangée musique
    sf::Text           m_lblMusic;
    sf::Text           m_valMusic;
    sf::RectangleShape m_btnMusicMinus;
    sf::RectangleShape m_btnMusicPlus;
    sf::Text           m_txtMusicMinus;
    sf::Text           m_txtMusicPlus;
    sf::RectangleShape m_barMusicBg;
    sf::RectangleShape m_barMusicFill;

    // Rangée effets
    sf::Text           m_lblEffects;
    sf::Text           m_valEffects;
    sf::RectangleShape m_btnEffMinus;
    sf::RectangleShape m_btnEffPlus;
    sf::Text           m_txtEffMinus;
    sf::Text           m_txtEffPlus;
    sf::RectangleShape m_barEffBg;
    sf::RectangleShape m_barEffFill;

    // Bouton Back
    sf::RectangleShape m_btnBack;
    sf::Text           m_txtBack;

    void centerText(sf::Text& text, float cx, float cy);
    void refreshBars();
    void clampVolumes();
};