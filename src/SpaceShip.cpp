#include "../include/SpaceShip.h"

SpaceShip::SpaceShip(const std::shared_ptr<hgui::kernel::Image>& shipImage,
	unsigned int health, unsigned int shield, unsigned int level,
	const std::tuple<unsigned int, unsigned int, std::chrono::milliseconds>& weaponStats) :
	m_shipTexture(nullptr), m_level(level), m_health(health), m_shield(shield),
	m_weaponStats(weaponStats), m_healthTexture(nullptr)
{
	set_health();
	m_shipTexture = hgui::SpriteManager::create(shipImage, 2 * shipImage->get_size(), hgui::point(0));
	move();
}

void SpaceShip::move()
{
	hgui::point newPosition = hgui::MouseManager::get_position();
	m_shipTexture->set_position(newPosition - m_shipTexture->get_size() / 2);
	hgui::after(std::chrono::milliseconds(20), std::bind(&SpaceShip::move, this));
}

void SpaceShip::set_health()
{
	std::stringstream type;
	type << "assets/textures/hp/health-" << m_health << '-' << m_shield << ".gif";
	m_healthTexture = hgui::SpriteManager::create(hgui::image_loader(type.str()), hgui::size(25_em, 6_em), hgui::point(37.5_em, 94_em));
}
