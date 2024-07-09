#pragma once

#include "Player.h"

class Laser final : public Player
{
	class Shot;

public:
	Laser();
	explicit Laser(const Player& player);

	bool shoot() override;

private:
	void level_up() override;
};
