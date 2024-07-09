#pragma once

#include "Entity.h"

class Meteor final : public Entity
{
public:
	Meteor(const hgui::point& position, unsigned type, const hgui::vec2& velocity);
	~Meteor() override;

	void take_damage() override;
	bool is_alive() const override;

private:
	std::shared_ptr<hgui::kernel::Sprite> m_texture;
	hgui::vec2 m_velocity;
	unsigned m_type;
	bool m_hasEnteredInWindow;

	void destroy();
	void collide();
	void move();
};
