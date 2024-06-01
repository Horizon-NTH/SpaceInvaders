#pragma once

#include "Include.h"
#include "Plazma.h"
#include "Shot.h"

class Plazma::Shot final : public SpaceShip::Shot
{
public:
	Shot(const std::shared_ptr<hgui::kernel::Image>& image, const hitbox& hitbox, const hgui::point& position, unsigned level = 1);

private:
	unsigned int m_level;
	std::vector<std::weak_ptr<Entity>> m_collidedEntities;

	void has_collide() override;
	void collide() override;
};
