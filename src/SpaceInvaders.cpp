#include "../include/SpaceInvaders.h"

SpaceInvaders::SpaceInvaders() :
	m_wave(1u), m_monitor(nullptr), m_window(nullptr), m_backgrounds(std::make_pair(nullptr, nullptr)),
	m_font(nullptr), m_texts(), m_player(nullptr)
{
	//LIbrary initialization//
	hgui::init();
	//Winodw creation//
	m_monitor = hgui::MonitorManager::get_primary_monitor();
	m_window = hgui::WindowManager::create("SpaceInvaders", m_monitor->get_size(),
		hgui::point(0), hgui::image_loader("assets/textures/logo.png"), m_monitor);
	//Tag creation//
	hgui::TagManager::create_tag("background");
	hgui::TagManager::create_tag("main_menu");
	hgui::TagManager::create_tag("game");
	//Background creation//
	hgui::TagManager::set_current_tag("background");
	m_backgrounds.first = hgui::SpriteManager::create(hgui::image_loader("assets/textures/first_background.gif"),
		m_window->get_size(), hgui::point(0));
	m_backgrounds.second = hgui::SpriteManager::create(hgui::image_loader("assets/textures/second_background.gif"),
		m_window->get_size(), hgui::point(0, -m_monitor->get_size().height));
	//Load Font
	m_font = hgui::FontManager::create("assets/fonts/space_age.ttf");
	//Binding
	hgui::KeyBoardManager::bind(hgui::KeyBoardAction(hgui::keys::ESCAPE, hgui::actions::PRESS), hgui::end);
}

void SpaceInvaders::start()
{
	move_background();
	set_main_menu();

	hgui::Widget::active({ "background", "main_menu" });
	hgui::Renderer::draw({ "background", "main_menu" });
	hgui::Renderer::loop();
}

void SpaceInvaders::move_background()
{
	hgui::TagManager::set_current_tag("main_menu");
	float speed =  5 * std::log(1 + m_wave);
	m_backgrounds.first->set_position(m_backgrounds.first->get_position() + hgui::point(0, speed));
	if (m_backgrounds.first->get_position().y >= m_backgrounds.first->get_size().height)
	{
		m_backgrounds.first->set_position(hgui::point(0, -m_backgrounds.first->get_size().height));
	}
	m_backgrounds.second->set_position(m_backgrounds.second->get_position() + hgui::point(0, speed));
	if (m_backgrounds.second->get_position().y >= m_backgrounds.second->get_size().height)
	{
		m_backgrounds.second->set_position(hgui::point(0, -m_backgrounds.second->get_size().height));
	}
	hgui::after(std::chrono::milliseconds(20), std::bind(&SpaceInvaders::move_background, this));
}

void SpaceInvaders::set_main_menu()
{
	m_texts.push_back(hgui::LabelManager::create("V 1.0", hgui::point(0), m_font, 
		{ 150u,  HGUI_COLOR_WHITE, 1.0f }));
	m_texts[0]->set_position(hgui::point(100_em - m_texts[0]->get_size().width, 0));
	m_texts.push_back(hgui::LabelManager::create("SpaceInvaders", hgui::point(0), m_font, 
		{ 160u, HGUI_COLOR_BLUE, 1.0f }));
	m_texts[1]->set_position(hgui::point(50_em) - m_texts[1]->get_size() / 2 - hgui::point(0, 10_em));
	m_texts.push_back(hgui::LabelManager::create("Press any key to play", hgui::point(0), m_font, 
		{ 50u, HGUI_COLOR_WHITE, 1.0f }));
	m_texts[2]->set_position(hgui::point(50_em) - m_texts[2]->get_size() / 2 + hgui::point(0, 15_em));
	m_texts.push_back(hgui::LabelManager::create("or escape to close.", hgui::point(0), m_font, 
		{ 50u, HGUI_COLOR_WHITE, 1.0f }));
	m_texts[3]->set_position(hgui::point(50_em) - m_texts[3]->get_size() / 2 + 
		hgui::point(0, 2 * m_texts[1]->get_size().height + 15_em));
	auto start = [this](const hgui::keys& key, const hgui::actions& action)
	{
		if (action == hgui::actions::PRESS && key != hgui::keys::ESCAPE)
		{
			m_texts.clear();
			start_game();
		}
	};
	hgui::KeyBoardManager::bind_keycallback(start);
}

void SpaceInvaders::start_game()
{
	hgui::TagManager::set_current_tag("game");
	hgui::CursorManager::hide();
	m_player = std::make_shared<Laser>();

	hgui::Renderer::draw({ "background", "game" });
}
