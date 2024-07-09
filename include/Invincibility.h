#pragma once

#include "PowerUp.h"

class Invincibility final : public PowerUp
{
public:
	explicit Invincibility(const hgui::point& position);

	static constexpr double probability{0.8};

private:
	void apply_effect(const std::shared_ptr<Player>& player) const override;
};
