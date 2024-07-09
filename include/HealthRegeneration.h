#pragma once

#include "PowerUp.h"

class HealthRegeneration final : public PowerUp
{
public:
	explicit HealthRegeneration(const hgui::point& position);

	static constexpr double probability{0.25};

private:
	void apply_effect(const std::shared_ptr<Player>& player) const override;
};
