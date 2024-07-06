#pragma once

#include "SpaceShip.h"

class Bot final : public SpaceShip
{
	class Shot;

public:
	Bot(const hgui::point& position, unsigned level, bool shielded, bool invincibility, const std::function<void(const std::shared_ptr<Bot>&)>& onDeath);
	~Bot() override;

	void move() override;
	bool shoot() override;

	const std::vector<std::unique_ptr<SpaceShip::Shot>>& get_shots();

private:
	hgui::vec2 m_velocity;
	bool m_hasEnteredWindow;
	bool m_collisonProcessed;
	std::string m_shotTempID;
	std::function<void(const std::shared_ptr<Bot>&)> m_onDeath;

	void destroy() override;
	void get_random_velocity();
	void collide();
};
