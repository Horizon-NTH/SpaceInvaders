#include "../include/Plazma.h"
#include "../include/PlazmaShot.h"

Plazma::Plazma() :
	Player("plazma", hgui::image_loader("assets/textures/spaceships/plazma.gif"), 3u, 0u, 1u,
		std::make_tuple(1u, 2u, std::make_pair(std::chrono::milliseconds(1000), hgui::Timer())))
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
		m_shots.emplace_back(std::make_unique<Shot>(image, hitbox, position, m_level));
		return true;
	}
	return false;
}
