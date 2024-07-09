#pragma once

#include "Include.h"
#include "SpaceShip.h"

class Player : public SpaceShip
{
public:
	Player(const std::string& shipName, unsigned int health, unsigned int shield, unsigned int level, const std::tuple<damage, ammo, cooldown>& weaponStats);
	Player(const Player& player, const std::string& shipName, const std::tuple<damage, ammo, cooldown>& weaponStats);

	void take_damage() override;
	virtual void level_up();

	void heal(bool full = false);
	void shield_up(bool full = false);

	[[nodiscard]] unsigned int get_health() const;
	[[nodiscard]] unsigned int get_shield() const;

	void stop();

private:
	std::shared_ptr<hgui::kernel::Sprite> m_healthTexture;
	std::shared_ptr<hgui::kernel::Sprite> m_levelTexture;
	std::shared_ptr<hgui::kernel::AnimatedSprite> m_reloading;

	void move() override;
	void set_health();
	void destroy() override;
};
