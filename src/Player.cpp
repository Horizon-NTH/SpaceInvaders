#include "../include/Player.h"
#include "../include/Shot.h"

Player::Player(const std::string& shipName, const unsigned health, const unsigned shield, const unsigned level, const std::tuple<damage, ammo, cooldown>& weaponStats) :
	SpaceShip(shipName, hitbox(hgui::MouseManager::get_position() - hgui::point(2.5_em).set_reference(hgui::reference::HEIGHT), hgui::size(5_em).set_reference(hgui::reference::HEIGHT)), health, shield, level, weaponStats)
{
	set_health();
	hgui::MouseManager::bind(hgui::MouseAction(hgui::buttons::LEFT, hgui::actions::PRESS), [this]
		{
			if (!m_reloading->is_playing())
			{
				if (shoot())
					m_reloading->play();
			}
		});
	const auto gif = hgui::gif_loader("assets/textures/reloading.gif");
	const auto framesCount = gif->get_frames_count();
	gif->set_delay(std::vector(framesCount, std::get<2>(weaponStats).first / framesCount));
	const float ratio = gif->get_size().width / gif->get_size().height;
	m_reloading = hgui::SpriteManager::create(gif, hgui::size(5_em * ratio, 5_em), hgui::point(100_em) - hgui::point(3.5_em * ratio, 5_em));
	m_reloading->play();
	m_level = 0u;
	while (m_level < level)
		Player::level_up();
	m_tempID = hgui::TaskManager::program(std::chrono::milliseconds(20), [this] { move(); });
}

Player::Player(const Player& player, const std::string& shipName, const std::tuple<damage, ammo, cooldown>& weaponStats) :
	Player(shipName, player.m_health, player.m_shield, player.m_level, weaponStats)
{
}

void Player::take_damage()
{
	SpaceShip::take_damage();
	if (is_alive())
		set_health();
}

void Player::level_up()
{
	m_level = std::min(++m_level, 3u);
	const auto image = hgui::image_loader("assets/textures/level/" + std::to_string(m_level) + ".png");
	const float aspectRatio = image->get_size().width / image->get_size().height;
	const hgui::size size = hgui::size(10_em).set_reference(hgui::reference::HEIGHT) * aspectRatio;
	m_levelTexture = hgui::SpriteManager::create(image, size, hgui::point());
	m_levelTexture->set_position(hgui::point(0, 100_em - size.em_height));
}

void Player::heal(const bool full)
{
	if (full)
		m_health = 3u;
	else
		m_health = std::min(++m_health, 3u);
	set_health();
}

void Player::shield_up(const bool full)
{
	if (full)
		m_shield = 3u;
	else
		m_shield = std::min(++m_shield, 3u);
	set_health();
}

unsigned int Player::get_health() const
{
	return m_health;
}

unsigned int Player::get_shield() const
{
	return m_shield;
}

void Player::stop()
{
	if (hgui::TaskManager::is_program(m_tempID))
		hgui::TaskManager::deprogram(m_tempID);
	if (hgui::MouseManager::is_bind(hgui::MouseAction(hgui::buttons::LEFT, hgui::actions::PRESS)))
		hgui::MouseManager::unbind(hgui::MouseAction(hgui::buttons::LEFT, hgui::actions::PRESS));
}

void Player::move()
{
	if (m_shipTexture)
	{
		const hgui::point newPosition = hgui::MouseManager::get_position();
		const auto windowSize = hgui::size(100_em);
		m_hitbox.first = newPosition - m_shipTexture->get_size() / 2;
		m_hitbox.first.em_x = hgui::kernel::EM<float>{} + std::clamp(m_hitbox.first.x, 0.f, windowSize.width - m_hitbox.second.width);
		m_hitbox.first.em_y = hgui::kernel::EM<float>{} + std::clamp(m_hitbox.first.y, 0.f, windowSize.height - m_hitbox.second.height);
		m_hitbox.first.update();
		m_shipTexture->set_position(m_hitbox.first);
		m_tempID = hgui::TaskManager::program(std::chrono::milliseconds(20), [this] { move(); });
	}
}

void Player::set_health()
{
	std::stringstream type;
	type << "assets/textures/hp/health-" << m_health << '-' << m_shield << ".gif";
	m_healthTexture = hgui::SpriteManager::create(hgui::image_loader(type.str()), hgui::size(25_em, 6_em), hgui::point(37.5_em, 94_em));
}

void Player::destroy()
{
	m_shipTexture = nullptr;
	m_healthTexture = nullptr;
	m_reloading = nullptr;
	m_levelTexture = nullptr;
	stop();
	const auto size = hgui::size(10_em).set_reference(hgui::reference::HEIGHT);
	const auto position = m_hitbox.first + m_hitbox.second / 2 - size / 2;
	const auto gif = hgui::gif_loader("assets/textures/explosions/explosion_player.gif");
	const auto explosion = hgui::SpriteManager::create(gif, size, position);
	explosion->play();
	hgui::kernel::GIFData::delay totalDelay(100);
	for (const auto& delay : gif->get_data().pixels | std::views::values)
		totalDelay += delay;
	hgui::TaskManager::program(totalDelay, [explosion] {});
	if (*m_isSfx)
	{
		const auto deathSound = hgui::SoundPlayerManager::create(hgui::audio_loader("assets/sfx/player_explode.wav"));
		deathSound->play();
		hgui::TaskManager::program(std::chrono::milliseconds(2000), [deathSound] {});
	}
}
