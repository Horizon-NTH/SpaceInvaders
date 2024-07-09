#include "../include/PowerUp.h"
#include "../include/Entity.h"
#include "../include/Player.h"

PowerUp::PowerUp(const hgui::point& position, const std::shared_ptr<hgui::kernel::Texture>& texture) :
	GameObject(hitbox(position - hgui::size(1.125_em, 0).set_reference(hgui::reference::HEIGHT), hgui::size(2.5_em).set_reference(hgui::reference::HEIGHT))),
	m_point(0u),
	m_texture(hgui::SpriteManager::create(texture, m_hitbox.second, m_hitbox.first))
{
	m_tempID = hgui::TaskManager::program(std::chrono::milliseconds(20), [this] { move(); });
}

PowerUp::~PowerUp()
{
	if (hgui::TaskManager::is_program(m_tempID))
		hgui::TaskManager::deprogram(m_tempID);
}

bool PowerUp::is_consumed() const
{
	return !static_cast<bool>(m_texture);
}

unsigned PowerUp::get_point() const
{
	return m_point;
}

void PowerUp::move()
{
	if (m_texture)
	{
		if (!is_in_window())
		{
			destroy();
			return;
		}
		m_hitbox.first += hgui::vec2(0, 3);
		m_texture->set_position(m_hitbox.first);
		m_tempID = hgui::TaskManager::program(std::chrono::milliseconds(20), [this] { move(); });
		check_collision();
	}
}

void PowerUp::check_collision()
{
	std::vector<std::weak_ptr<Entity>> collidedEntities;
	std::ranges::copy_if(Entity::m_gameEntities, std::back_inserter(collidedEntities), [&](const std::weak_ptr<Entity>& ptr)
		{
			if (const auto entity = ptr.lock(); entity && entity->is_alive())
				return entity->is_overlapping(m_hitbox);
			return false;
		});
	if (!collidedEntities.empty())
	{
		for (const auto& ptr : collidedEntities)
		{
			if (const auto player = std::dynamic_pointer_cast<Player>(ptr.lock()))
			{
				apply_effect(player);
				destroy();
			}
			else
				destroy("_bot");
		}
	}
}

void PowerUp::destroy(const std::string& warpType)
{
	m_texture = nullptr;
	if (hgui::TaskManager::is_program(m_tempID))
		hgui::TaskManager::deprogram(m_tempID);
	const auto size = hgui::size(5_em).set_reference(hgui::reference::HEIGHT);
	const auto position = m_hitbox.first + m_hitbox.second / 2 - size / 2;
	const auto gif = hgui::gif_loader("assets/textures/power_up/warp" + warpType + ".gif");
	const auto warp = hgui::SpriteManager::create(gif, size, position);
	warp->play();
	hgui::kernel::GIFData::delay totalDelay{};
	for (const auto& delay : gif->get_data().pixels | std::views::values)
		totalDelay += delay;
	hgui::TaskManager::program(totalDelay, [warp] {});
}
