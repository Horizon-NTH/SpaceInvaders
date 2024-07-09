#include "../include/Bomber.h"
#include "../include/BomberShot.h"

Bomber::Bomber() :
	Player("bomber", 3u, 0u, 1u, std::make_tuple(1u, 1u, std::make_pair(std::chrono::milliseconds(2000), hgui::Timer())))
{
}

Bomber::Bomber(const Player& player) :
	Player(player, "bomber",
		player.get_level() == 1u ? std::make_tuple(1u, 1u, std::make_pair(std::chrono::milliseconds(2000), hgui::Timer())) : player.get_level() == 2u ? std::make_tuple(1u, 2u, std::make_pair(std::chrono::milliseconds(1750), hgui::Timer())) : std::make_tuple(1u, 3u, std::make_pair(std::chrono::milliseconds(1500), hgui::Timer())))
{
}

bool Bomber::shoot()
{
	m_shots.erase(std::begin(std::ranges::remove_if(m_shots, [](const std::unique_ptr<SpaceShip::Shot>& shot) { return shot->is_destroyed(); })), m_shots.end());
	if (m_shots.size() < std::get<1>(m_weaponStats))
	{
		auto image = hgui::image_loader("assets/textures/lasers/bomber.png");
		const hgui::size size = hgui::size(2.5_em).set_reference(hgui::reference::HEIGHT);
		hgui::point position = m_hitbox.first + hgui::point(m_hitbox.second.width / 2.f, -size.height / 2);
		hitbox hitbox = std::make_pair(position - size / 2., size);
		m_shots.emplace_back(std::make_unique<Shot>(image, hitbox, std::get<0>(m_weaponStats), m_level));
		return true;
	}
	return false;
}

void Bomber::level_up()
{
	Player::level_up();
	if (m_level == 2u)
	{
		m_weaponStats = std::make_tuple(1u, 2u, std::make_pair(std::chrono::milliseconds(1750), hgui::Timer()));
	}
	else if (m_level == 3u)
	{
		m_weaponStats = std::make_tuple(1u, 3u, std::make_pair(std::chrono::milliseconds(1500), hgui::Timer()));
	}
}
