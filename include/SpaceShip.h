#pragma once

#include "Include.h"

class SpaceShip
{
public:
	SpaceShip(const std::shared_ptr<hgui::kernel::Image>& shipImage, 
		unsigned int health, unsigned int shield, unsigned int level, 
		const std::tuple<unsigned int, unsigned int, std::chrono::milliseconds>& weaponStats);

	void move();
	virtual void shoot() = 0;
	void set_health();

protected:
	std::shared_ptr<hgui::kernel::Sprite> m_shipTexture;
	std::shared_ptr<hgui::kernel::Sprite> m_healthTexture;

	unsigned int m_health;
	unsigned int m_shield;
	unsigned int m_level;
	bool m_invincible;
	// (damage, numbers, cooldown)
	std::tuple<unsigned int, unsigned int, std::chrono::milliseconds> m_weaponStats;
};

