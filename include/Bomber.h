#pragma once

#include "Player.h"

class Bomber final : public Player
{
	class Shot;

public:
	Bomber();
	explicit Bomber(const Player& player);

	bool shoot() override;

private:
	void level_up() override;
};
