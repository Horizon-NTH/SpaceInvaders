#pragma once

#include "Include.h"
#include "Laser.h"
#include "Shot.h"

class Laser::Shot final : public SpaceShip::Shot
{
public:
	Shot(const std::shared_ptr<hgui::kernel::Image>& image, const hitbox& hitbox, damage damage, unsigned level = 1);

private:
	unsigned int m_level;

	void has_collide() override;
};
