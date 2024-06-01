#pragma once

#include "Player.h"

class Bomber final : public Player
{
	class Shot;

public:
	Bomber();

	bool shoot() override;
};
