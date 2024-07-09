#pragma once

#include "PowerUp.h"

class LevelUp final : public PowerUp
{
public:
	explicit LevelUp(const hgui::point& position);

	static constexpr double probability{0.85};

private:
	void apply_effect(const std::shared_ptr<Player>& player) const override;
};
