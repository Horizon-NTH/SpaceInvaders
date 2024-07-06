#include <utility>

#include "../include/SpaceShip.h"
#include "../include/Shot.h"

SpaceShip::SpaceShip(std::string name, const hitbox& hitbox, const unsigned int health, const unsigned int shield, const unsigned int level, const std::tuple<damage, ammo, cooldown>& weaponStats) :
	Entity(hitbox),
	m_health(health),
	m_shield(shield),
	m_level(level),
	m_invincible(false),
	m_name(std::move(name)),
	m_weaponStats(weaponStats)
{
	set_texture();
}

SpaceShip::~SpaceShip()
{
	if (hgui::TaskManager::is_program(m_invincibilityTempID))
		hgui::TaskManager::deprogram(m_invincibilityTempID);
}

void SpaceShip::take_damage()
{
	if (m_invincible)
		return;
	if (m_shield)
		--m_shield;
	else if (m_health)
		--m_health;
	set_texture();
	if (!is_alive())
		destroy();
}

bool SpaceShip::is_alive() const
{
	return m_health;
}

void SpaceShip::make_invincible(const std::chrono::seconds& duration)
{
	m_invincible = true;
	set_texture();
	m_invincibilityTempID = hgui::TaskManager::program(duration, [this]
		{
			m_invincible = false;
			set_texture();
		});
}

unsigned SpaceShip::get_level() const
{
	return m_level;
}

void SpaceShip::set_texture()
{
	std::string bonus(".gif");
	if (m_invincible)
		bonus = "_invincible.gif";
	else if (m_shield)
		bonus = "_shield.gif";
	if (!m_shipTexture)
		m_shipTexture = hgui::SpriteManager::create(hgui::TextureManager::create(hgui::image_loader("assets/textures/spaceships/" + m_name + bonus)), m_hitbox.second, m_hitbox.first);
	else
		m_shipTexture->set_texture(hgui::TextureManager::create(hgui::image_loader("assets/textures/spaceships/" + m_name + bonus)));
}
