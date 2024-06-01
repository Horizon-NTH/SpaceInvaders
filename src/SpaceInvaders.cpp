#include "../include/SpaceInvaders.h"
#include "../include/Laser.h"
#include "../include/Bomber.h"
#include "../include/Plazma.h"
#include "../include/Devastator.h"

SpaceInvaders::SpaceInvaders() :
	m_wave(1u),
	m_isSfx(true),
	m_isMusic(true)
{
	//LIbrary initialization//
	hgui::init();
	//Winodw creation//
	m_monitor = hgui::MonitorManager::get_primary_monitor();
	m_window = hgui::WindowManager::create("SpaceInvaders", m_monitor->get_size(),
		hgui::point(0), hgui::image_loader("assets/textures/logo.png"), nullptr, {hgui::WindowOption(hgui::options::MAXIMIZED, true)});
	//Tag creation//
	hgui::TagManager::create_tag("background");
	hgui::TagManager::create_tag("main_menu");
	hgui::TagManager::create_tag("game");
	//Background creation//
	hgui::TagManager::set_current_tag("background");
	m_backgrounds.first = hgui::SpriteManager::create(hgui::image_loader("assets/textures/first_background.gif"),
		hgui::size(100_em), hgui::point(0));
	m_backgrounds.second = hgui::SpriteManager::create(hgui::image_loader("assets/textures/second_background.gif"),
		hgui::size(100_em), hgui::point(0, -100_em));
	m_sfx.first = hgui::TextureManager::create(hgui::image_loader("assets/textures/icons/sfx_on.png"));
	m_sfx.second = hgui::TextureManager::create(hgui::image_loader("assets/textures/icons/sfx_off.png"));
	m_music.first = hgui::TextureManager::create(hgui::image_loader("assets/textures/icons/music_on.png"));
	m_music.second = hgui::TextureManager::create(hgui::image_loader("assets/textures/icons/music_off.png"));
	//Load Font
	m_font = hgui::FontManager::create("assets/fonts/space_age.ttf");
	//Binding
	hgui::KeyBoardManager::bind(hgui::KeyBoardAction(hgui::keys::ESCAPE, hgui::actions::PRESS), hgui::end);
}

void SpaceInvaders::start()
{
	move_background();
	set_main_menu();
	hgui::kernel::Widget::active({"main_menu"});
	hgui::Renderer::draw({"background", "main_menu"});
	hgui::Renderer::loop();
}

void SpaceInvaders::move_background()
{
	const auto speed = static_cast<float>(5. * std::log(1 + m_wave));
	const auto height = hgui::size(100_em).height;
	m_backgrounds.first->set_position((m_backgrounds.first->get_position() + hgui::point(0, speed)).undo_responsivness());
	if (m_backgrounds.first->get_position().y >= height)
	{
		m_backgrounds.first->set_position(hgui::point(0, -height));
	}
	m_backgrounds.second->set_position((m_backgrounds.second->get_position() + hgui::point(0, speed)).undo_responsivness());
	if (m_backgrounds.second->get_position().y >= height)
	{
		m_backgrounds.second->set_position(hgui::point(0, -height));
	}
	m_backgroundMovingTaskId = hgui::TaskManager::program(std::chrono::milliseconds(20), [this] { move_background(); });
}

void SpaceInvaders::set_main_menu()
{
	hgui::TagManager::set_current_tag("main_menu");
	const auto version = hgui::LabelManager::create("V 1.0", hgui::point(0), m_font);
	m_texts.push_back(version);
	const auto title = hgui::LabelManager::create("SpaceInvaders", hgui::point(0), m_font, false, {200u, HGUI_COLOR_BLUE, 1.0f});
	m_texts.push_back(title);
	const auto txt1 = hgui::LabelManager::create("Press any key to play", hgui::point(0), m_font);
	m_texts.push_back(txt1);
	const auto txt2 = hgui::LabelManager::create("or ESCAPE to close.", hgui::point(0), m_font);
	m_texts.push_back(txt2);

	const auto sfx = hgui::ButtonManager::create(nullptr, hgui::size(10_em).set_reference(hgui::reference::HEIGHT), hgui::point(0), m_sfx.first, std::make_tuple(hgui::color("6e738d"), HGUI_COLOR_BLUE, HGUI_COLOR_BLUE), 25.f, false);
	sfx->set_position(hgui::point(5_em, 95_em - sfx->get_size().em_height).set_reference(hgui::reference::HEIGHT));
	const auto sfxSwitch = [=, this]
		{
			m_isSfx = !m_isSfx;
			sfx->set_texture(m_isSfx ? m_sfx.first : m_sfx.second);
		};
	sfx->set_function(sfxSwitch);
	m_buttons.push_back(sfx);

	const auto music = hgui::ButtonManager::create(nullptr, hgui::size(10_em).set_reference(hgui::reference::HEIGHT), hgui::point(0), m_music.first, std::make_tuple(hgui::color("6e738d"), HGUI_COLOR_BLUE, HGUI_COLOR_BLUE), 25.f, false);
	music->set_position(hgui::point(20_em, 95_em - music->get_size().em_height).set_reference(hgui::reference::HEIGHT));
	const auto musicSwitch = [=, this]
		{
			m_isMusic = !m_isMusic;
			music->set_texture(m_isMusic ? m_music.first : m_music.second);
		};
	music->set_function(musicSwitch);
	m_buttons.push_back(music);

	auto callback = [=]
		{
			version->set_height(static_cast<unsigned>(hgui::size(3_em).height));
			version->set_position(hgui::point(99_em - version->get_size().width, 1_em));
			title->set_width(static_cast<unsigned>(hgui::size(99_em).width));
			title->set_position(hgui::point(50_em) - title->get_size() / 2 - hgui::point(0, 20_em));
			txt1->set_height(static_cast<unsigned>(hgui::size(3_em).height));
			txt1->set_position(hgui::point(50_em) - txt1->get_size() / 2);
			txt2->set_height(static_cast<unsigned>(hgui::size(3_em).height));
			txt2->set_position(hgui::point(50_em) - txt2->get_size() / 2 + hgui::point(0, 4_em));
			if (m_backgrounds.first->get_position().y < m_backgrounds.second->get_position().y)
			{
				m_backgrounds.second->set_position(m_backgrounds.first->get_position() + hgui::point(0, hgui::size(100_em).em_height));
			}
			else
			{
				m_backgrounds.first->set_position(m_backgrounds.second->get_position() + hgui::point(0, hgui::size(100_em).em_height));
			}
			if (hgui::TaskManager::is_program(m_backgroundMovingTaskId))
				hgui::TaskManager::deprogram(m_backgroundMovingTaskId);
			m_backgroundMovingTaskId = hgui::TaskManager::program(std::chrono::milliseconds(20), [this] { move_background(); });
		};
	callback();
	m_window->set_size_callback(callback);

	auto start = [this](const hgui::keys& key, const hgui::actions& action)
		{
			if (action == hgui::actions::PRESS && key != hgui::keys::ESCAPE)
			{
				hgui::KeyBoardManager::bind_key_callback([] {});
				start_game();
			}
		};
	hgui::KeyBoardManager::bind_key_callback(start);
}

void SpaceInvaders::start_game()
{
	hgui::TagManager::set_current_tag("game");
	hgui::CursorManager::hide();
	m_player = std::make_shared<Laser>();

	hgui::Renderer::draw({"background", "game"});
}