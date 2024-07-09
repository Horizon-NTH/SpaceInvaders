#include "../include/Plazma.h"
#include "../include/PlazmaShot.h"

Plazma::Plazma() :
	Player("plazma", 3u, 0u, 1u, std::make_tuple(1u, 2u, std::make_pair(std::chrono::milliseconds(1000), hgui::Timer())))
{
}

Plazma::Plazma(const Player& player) :
	Player(player, "plazma",
		player.get_level() == 1u ? std::make_tuple(1u, 2u, std::make_pair(std::chrono::milliseconds(1000), hgui::Timer())) : player.get_level() == 2u ? std::make_tuple(1u, 3u, std::make_pair(std::chrono::milliseconds(800), hgui::Timer())) : std::make_tuple(2u, 3u, std::make_pair(std::chrono::milliseconds(600), hgui::Timer())))
{
}

bool Plazma::shoot()
{
	m_shots.erase(std::begin(std::ranges::remove_if(m_shots, [](const std::unique_ptr<SpaceShip::Shot>& shot) { return shot->is_destroyed(); })), m_shots.end());
	if (m_shots.size() < std::get<1>(m_weaponStats))
	{
		auto image = hgui::image_loader("assets/textures/lasers/plazma.png");
		const float ratio = image->get_size().width / image->get_size().height;
		const hgui::size size = hgui::size(1_em * ratio, 1_em).set_reference(hgui::reference::HEIGHT);
		hgui::point position = m_hitbox.first + hgui::point(m_hitbox.second.width / 2.f, -size.height / 2);
		hitbox hitbox = std::make_pair(position - size / 2., size);
		m_shots.emplace_back(std::make_unique<Shot>(image, hitbox, std::get<0>(m_weaponStats), m_level));
		return true;
	}
	return false;
}

void Plazma::level_up()
{
	Player::level_up();
	if (m_level == 2u)
	{
		m_weaponStats = std::make_tuple(1u, 3u, std::make_pair(std::chrono::milliseconds(800), hgui::Timer()));
	}
	else if (m_level == 3u)
	{
		m_weaponStats = std::make_tuple(2u, 3u, std::make_pair(std::chrono::milliseconds(600), hgui::Timer()));
	}
}
