#pragma once

#include "Include.h"
#include "SpaceShip.h"

class Laser : public SpaceShip
{
public:
	Laser();

	void shoot() override;
};

