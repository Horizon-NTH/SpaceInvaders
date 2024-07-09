#pragma once

#include "PowerUp.h"

class MegaRegeneration final : public PowerUp
{
public:
	explicit MegaRegeneration(const hgui::point& position);

	static constexpr double probability{0.75};

private:
	void apply_effect(const std::shared_ptr<Player>& player) const override;
};
