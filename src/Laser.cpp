#include "../include/Laser.h"
#include "../include/LaserShot.h"

Laser::Laser() :
	Player("laser", 3u, 0u, 1u, std::make_tuple(1u, 5u, std::make_pair(std::chrono::milliseconds(700), hgui::Timer())))
{
}

Laser::Laser(const Player& player) :
	Player(player, "laser",
		player.get_level() == 1u ? std::make_tuple(1u, 5u, std::make_pair(std::chrono::milliseconds(700), hgui::Timer())) : player.get_level() == 2u ? std::make_tuple(1u, 6u, std::make_pair(std::chrono::milliseconds(600), hgui::Timer())) : std::make_tuple(1u, 7u, std::make_pair(std::chrono::milliseconds(500), hgui::Timer())))
{
}

bool Laser::shoot()
{
	m_shots.erase(std::begin(std::ranges::remove_if(m_shots, [](const std::unique_ptr<SpaceShip::Shot>& shot) { return shot->is_destroyed(); })), m_shots.end());
	if (m_shots.size() < std::get<1>(m_weaponStats))
	{
		auto image = hgui::image_loader("assets/textures/lasers/laser_blue.png");
		const float ratio = image->get_size().width / image->get_size().height;
		const hgui::size size = hgui::size(2_em * ratio, 2_em).set_reference(hgui::reference::HEIGHT);
		hgui::point position = m_hitbox.first + hgui::point(m_hitbox.second.width / 2.f, -size.height / 2);
		hitbox hitbox = std::make_pair(position - size / 2., size);
		m_shots.emplace_back(std::make_unique<Shot>(image, hitbox, std::get<0>(m_weaponStats), m_level));
		return true;
	}
	return false;
}

void Laser::level_up()
{
	Player::level_up();
	if (m_level == 2u)
	{
		m_weaponStats = std::make_tuple(1u, 6u, std::make_pair(std::chrono::milliseconds(600), hgui::Timer()));
	}
	else if (m_level == 3u)
	{
		m_weaponStats = std::make_tuple(1u, 7u, std::make_pair(std::chrono::milliseconds(500), hgui::Timer()));
	}
}
