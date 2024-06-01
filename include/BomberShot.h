#pragma once

#include "Include.h"
#include "Bomber.h"
#include "Shot.h"

class Bomber::Shot final : public SpaceShip::Shot
{
public:
	Shot(const std::shared_ptr<hgui::kernel::Image>& image, const hitbox& hitbox, const hgui::point& position, unsigned level = 1);

private:
	unsigned int m_level;

	void has_collide() override;
};
