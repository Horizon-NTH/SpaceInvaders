#pragma once

#include "Entity.h"

class SpaceShip : public Entity
{
protected:
	using damage = unsigned int;
	using ammo = unsigned int;
	using cooldown = std::pair<std::chrono::milliseconds, hgui::Timer>;
	class Shot;

public:
	SpaceShip(const std::string& name, const hitbox& hitbox, const std::shared_ptr<hgui::kernel::Image>& shipImage, unsigned int health, unsigned int shield, unsigned int level, const std::tuple<damage, ammo, cooldown>& weaponStats);
	~SpaceShip() override;

	virtual bool shoot() = 0;
	void take_damage() override;
	[[nodiscard]] bool is_alive() const override;

protected:
	std::shared_ptr<hgui::kernel::Sprite> m_shipTexture;
	std::vector<std::unique_ptr<Shot>> m_shots;

	unsigned int m_health;
	unsigned int m_shield;
	unsigned int m_level;
	bool m_invincible;
	std::string m_name;
	std::tuple<damage, ammo, cooldown> m_weaponStats;

	void set_texture() const;
	virtual void destroy() = 0;
};
