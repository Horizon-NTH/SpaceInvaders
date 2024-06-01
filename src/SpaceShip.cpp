#include "../include/SpaceShip.h"
#include "../include/Shot.h"

SpaceShip::SpaceShip(const std::string& name, const hitbox& hitbox, const std::shared_ptr<hgui::kernel::Image>& shipImage, const unsigned int health, const unsigned int shield, const unsigned int level, const std::tuple<damage, ammo, cooldown>& weaponStats) :
	Entity(hitbox),
	m_health(health),
	m_shield(shield),
	m_level(level),
	m_invincible(false),
	m_weaponStats(weaponStats),
	m_name(name)
{
	m_shipTexture = hgui::SpriteManager::create(shipImage, m_hitbox.second, m_hitbox.first);
}

SpaceShip::~SpaceShip() = default;

void SpaceShip::take_damage()
{
	if (m_shield)
		--m_shield;
	else if (m_health)
		--m_health;
	set_texture();
}

bool SpaceShip::is_alive() const
{
	return m_health;
}

void SpaceShip::set_texture() const
{
	std::string bonus(".gif");
	if (m_invincible)
		bonus = "_invincible.gif";
	else if (m_shield)
		bonus = "_shield.gif";
	m_shipTexture->set_texture(hgui::TextureManager::create(hgui::image_loader("assets/textures/spaceships/" + m_name + bonus)));
}
