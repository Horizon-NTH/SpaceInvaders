#pragma once

#include "Include.h"
#include "Bot.h"
#include "Shot.h"

class Bot::Shot final : public SpaceShip::Shot
{
public:
	Shot(const std::shared_ptr<hgui::kernel::Image>& image, const hitbox& hitbox, const hgui::point& position, unsigned level = 1);

private:
	unsigned int m_level;

	bool can_damaged(const std::weak_ptr<Entity>& entity) override;
	void has_collide() override;
};
