#pragma once

#include "Include.h"
#include "Devastator.h"
#include "Shot.h"

class Devastator::Shot final : public SpaceShip::Shot
{
public:
	Shot(const std::shared_ptr<hgui::kernel::Image>& image, const hitbox& hitbox, damage damage, unsigned level = 1);

private:
	unsigned int m_level;
	std::vector<std::weak_ptr<Entity>> m_collidedEntities;

	void has_collide() override;
	void collide() override;
};
