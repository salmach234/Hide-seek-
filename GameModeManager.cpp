#include "GameModeManager.h"
#include "GameConfig.h"

GameModeManager::GameModeManager(sf::Font& font)
    : m_font(font),
    m_mode(Mode::None),
    m_hovered(-1),
    m_normal(40, 40, 65),
    m_hover(80, 80, 140),
    m_title(font, "SELECT MODE", 42),
    m_lblStory(font, "STORY MODE", 30),
    m_lblEndless(font, "ENDLESS MODE", 30),
    m_descStory(font, "Reach the bunker in 180 seconds.\nSurvive and escape.", 18),
    m_descEndless(font, "Run as far as possible.\nNo time limit. Infinite difficulty.", 18)
{
    m_title.setFillColor(sf::Color::Yellow);
    sf::FloatRect tb = m_title.getLocalBounds();
    m_title.setOrigin({ tb.size.x / 2.f, 0.f });
    m_title.setPosition({ WIN_W / 2.f, 80.f });

    m_boxStory.setSize({ 380.f, 160.f });
    m_boxStory.setFillColor(m_normal);
    m_boxStory.setOutlineColor(sf::Color(80, 80, 140));
    m_boxStory.setOutlineThickness(2.f);
    m_boxStory.setPosition({ 100.f, 200.f });

    m_boxEndless.setSize({ 380.f, 160.f });
    m_boxEndless.setFillColor(m_normal);
    m_boxEndless.setOutlineColor(sf::Color(80, 80, 140));
    m_boxEndless.setOutlineThickness(2.f);
    m_boxEndless.setPosition({ 544.f, 200.f });

    m_lblStory.setFillColor(sf::Color::White);
    m_lblStory.setPosition({ 140.f, 220.f });

    m_lblEndless.setFillColor(sf::Color::White);
    m_lblEndless.setPosition({ 560.f, 220.f });

    m_descStory.setFillColor(sf::Color(180, 180, 180));
    m_descStory.setPosition({ 115.f, 272.f });

    m_descEndless.setFillColor(sf::Color(180, 180, 180));
    m_descEndless.setPosition({ 556.f, 272.f });
}

void GameModeManager::updateHover(sf::Vector2f mouse)
{
    m_hovered = -1;
    if (m_boxStory.getGlobalBounds().contains(mouse))
    {
        m_hovered = 0;
        m_boxStory.setFillColor(m_hover);
        m_boxEndless.setFillColor(m_normal);
    }
    else if (m_boxEndless.getGlobalBounds().contains(mouse))
    {
        m_hovered = 1;
        m_boxEndless.setFillColor(m_hover);
        m_boxStory.setFillColor(m_normal);
    }
    else
    {
        m_boxStory.setFillColor(m_normal);
        m_boxEndless.setFillColor(m_normal);
    }
}

void GameModeManager::update(const sf::RenderWindow& window)
{
    sf::Vector2i mp = sf::Mouse::getPosition(window);
    updateHover(window.mapPixelToCoords(mp));
}

void GameModeManager::handleEvent(const sf::Event& event,
    const sf::RenderWindow& window)
{
    sf::Vector2i mp = sf::Mouse::getPosition(window);
    updateHover(window.mapPixelToCoords(mp));

    if (const auto* c = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (c->button == sf::Mouse::Button::Left)
        {
            if (m_hovered == 0) m_mode = Mode::Story;
            if (m_hovered == 1) m_mode = Mode::Endless;
        }
    }
}

void GameModeManager::draw(sf::RenderWindow& window)
{
    window.draw(m_title);
    window.draw(m_boxStory);
    window.draw(m_boxEndless);
    window.draw(m_lblStory);
    window.draw(m_lblEndless);
    window.draw(m_descStory);
    window.draw(m_descEndless);
}

GameModeManager::Mode GameModeManager::getMode() const { return m_mode; }
void                  GameModeManager::reset() { m_mode = Mode::None; }