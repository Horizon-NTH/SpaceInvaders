#pragma once

#include "Player.h"

class Laser final : public Player
{
	class Shot;

public:
	Laser();

	bool shoot() override;
};
