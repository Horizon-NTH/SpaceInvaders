#include "../include/Laser.h"

Laser::Laser() :
	SpaceShip(hgui::image_loader("assets/textures/spaceships/laser.gif"), 3u, 0u, 1u, 
		std::make_tuple(1u, 5u, std::chrono::milliseconds(700)))
{
}

void Laser::shoot()
{
}
