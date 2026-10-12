#include "main/window.hpp"
#include <iostream>

Window::Window(SharedContext& context)
	: m_context(context)
	, m_windowSize(800, 600)
	, m_windowTitle("Maghreb")
	, m_isFullscreen(false)
	, m_isFocused(true)
{
	std::cerr << "[Stub] Window created: " << m_windowTitle
		<< " (" << m_windowSize.x << "x" << m_windowSize.y << ")\n";

	m_renderWindow.create(
		sf::VideoMode(m_windowSize.x, m_windowSize.y, 32),
		m_windowTitle,
		sf::Style::Default
	);
}

Window::~Window() {
	std::cerr << "[Stub] Window destroyed\n";
	m_renderWindow.close();
}

void Window::update() {
	// «аглушка: обрабатываем только закрытие окна
	sf::Event event;
	while (m_renderWindow.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			m_renderWindow.close();
		}
		if (event.type == sf::Event::LostFocus) {
			m_isFocused = false;
		}
		else if (event.type == sf::Event::GainedFocus) {
			m_isFocused = true;
		}
	}
}

sf::RenderWindow* Window::getRenderWindow() {
	return &m_renderWindow;
}

const sf::Vector2u Window::getWindowSize() const {
	return m_windowSize;
}

sf::FloatRect Window::getViewSpace() {
	sf::Vector2f viewCenter = m_renderWindow.getView().getCenter();
	sf::Vector2f viewSize = m_renderWindow.getView().getSize();
	return sf::FloatRect(
		viewCenter - sf::Vector2f(viewSize.x / 2, viewSize.y / 2),
		viewSize
	);
}

bool Window::isFullscreen() const { return m_isFullscreen; }
bool Window::isFocused()   const { return m_isFocused; }

void Window::close() {
	m_renderWindow.close();
}
