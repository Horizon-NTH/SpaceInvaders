#pragma once

#include "PowerUp.h"

class FullHealthRegeneration final : public PowerUp
{
public:
	explicit FullHealthRegeneration(const hgui::point& position);

	static constexpr double probability{0.6};

private:
	void apply_effect(const std::shared_ptr<Player>& player) const override;
};
