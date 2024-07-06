#pragma once

#include "Include.h"
#include "SpaceShip.h"

class Player : public SpaceShip
{
public:
	Player(const std::string& shipName, unsigned int health, unsigned int shield, unsigned int level, const std::tuple<damage, ammo, cooldown>& weaponStats);

	void take_damage() override;

private:
	std::shared_ptr<hgui::kernel::Sprite> m_healthTexture;
	std::shared_ptr<hgui::kernel::AnimatedSprite> m_reloading;

	void move() override;
	void set_health();
	void destroy() override;
};
