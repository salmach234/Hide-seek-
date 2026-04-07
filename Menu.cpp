#include "Menu.h"
#include "GameConfig.h"

static const float BTN_W = 300.f;
static const float BTN_H = 60.f;
static const float BTN_G = 18.f;

Menu::Menu(sf::Font& font)
    : m_font(font),
    m_choice(Choice::None),
    m_hovered(-1),
    m_normal(50, 50, 80),
    m_hover(100, 100, 180)
{
    build();
}

void Menu::centerText(sf::Text& text, float cx, float cy)
{
    sf::FloatRect b = text.getLocalBounds();
    text.setOrigin({ b.position.x + b.size.x / 2.f,
                     b.position.y + b.size.y / 2.f });
    text.setPosition({ cx, cy });
}

void Menu::build()
{
    std::vector<std::string> labels = { "Play", "About", "Settings", "Quit" };

    float startX = (WIN_W - BTN_W) / 2.f;
    float totalH = labels.size() * BTN_H + (labels.size() - 1) * BTN_G;
    float startY = (WIN_H - totalH) / 2.f + 40.f;

    for (int i = 0; i < (int)labels.size(); i++)
    {
        float posY = startY + i * (BTN_H + BTN_G);

        sf::RectangleShape box({ BTN_W, BTN_H });
        box.setPosition({ startX, posY });
        box.setFillColor(m_normal);
        box.setOutlineColor(sf::Color::White);
        box.setOutlineThickness(2.f);
        m_boxes.push_back(box);

        sf::Text txt(m_font, labels[i], 28);
        txt.setFillColor(sf::Color::White);
        centerText(txt,
            startX + BTN_W / 2.f,
            posY + BTN_H / 2.f);
        m_labels.push_back(txt);
    }
}

void Menu::updateHover(sf::Vector2f mouse)
{
    m_hovered = -1;
    for (int i = 0; i < (int)m_boxes.size(); i++)
    {
        bool over = m_boxes[i].getGlobalBounds().contains(mouse);
        m_boxes[i].setFillColor(over ? m_hover : m_normal);
        if (over) m_hovered = i;
    }
}

void Menu::update(const sf::RenderWindow& window)
{
    sf::Vector2i mp = sf::Mouse::getPosition(window);
    updateHover(window.mapPixelToCoords(mp));
}

void Menu::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    sf::Vector2i mp = sf::Mouse::getPosition(window);
    updateHover(window.mapPixelToCoords(mp));

    if (const auto* c = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (c->button == sf::Mouse::Button::Left && m_hovered >= 0)
        {
            if (m_hovered == 0) m_choice = Choice::Play;
            if (m_hovered == 1) m_choice = Choice::About;
            if (m_hovered == 2) m_choice = Choice::Settings;
            if (m_hovered == 3) m_choice = Choice::Quit;
        }
    }
}

void Menu::draw(sf::RenderWindow& window)
{
    for (int i = 0; i < (int)m_boxes.size(); i++)
    {
        window.draw(m_boxes[i]);
        window.draw(m_labels[i]);
    }
}

Menu::Choice Menu::getChoice() const { return m_choice; }
void         Menu::resetChoice() { m_choice = Choice::None; }