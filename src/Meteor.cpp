#include "../include/Meteor.h"
#include "../include/Player.h"

Meteor::Meteor(const hgui::point& position, const unsigned type, const hgui::vec2& velocity) :
	Entity({}, type + 1),
	m_velocity(velocity),
	m_type(type),
	m_hasEnteredInWindow(false)
{
	const auto image = hgui::image_loader("assets/textures/meteors/meteor_" + std::to_string(type) + ".png");
	const float ratio = static_cast<float>(image->get_size().width) / image->get_size().height;
	const auto em = 2.5_em * (type + 1);
	m_hitbox = {position, hgui::size(em * ratio, em).set_reference(hgui::reference::HEIGHT)};
	m_texture = hgui::SpriteManager::create(image, m_hitbox.second, m_hitbox.first);
	m_tempID = hgui::TaskManager::program(std::chrono::milliseconds(20), [this] { move(); });
}

Meteor::~Meteor()
{
	if (hgui::TaskManager::is_program(m_tempID))
		hgui::TaskManager::deprogram(m_tempID);
}

void Meteor::take_damage()
{
	if (m_health)
		--m_health;
	if (!is_alive())
		destroy();
}

bool Meteor::is_alive() const
{
	return m_health;
}

void Meteor::destroy()
{
	m_texture = nullptr;
	m_health = 0u;
	if (hgui::TaskManager::is_program(m_tempID))
		hgui::TaskManager::deprogram(m_tempID);
	const auto gif = hgui::gif_loader("assets/textures/explosions/explosion_meteor.gif");
	const auto explosion = hgui::SpriteManager::create(gif, m_hitbox.second, m_hitbox.first);
	explosion->play();
	hgui::kernel::GIFData::delay totalDelay(0);
	for (const auto& delay : gif->get_data().pixels | std::views::values)
		totalDelay += delay;
	hgui::TaskManager::program(totalDelay, [explosion] {});
}

void Meteor::collide()
{
	if (const auto playerIterator = std::ranges::find_if(m_gameEntities,
		[this](const std::weak_ptr<Entity>& ptr)
			{
				if (const auto entity = std::dynamic_pointer_cast<Player>(ptr.lock()); entity && entity->is_alive())
					return entity->is_overlapping(m_hitbox);
				return false;
			}); playerIterator != m_gameEntities.end())
	{
		if (const auto player = std::dynamic_pointer_cast<Player>(playerIterator->lock()))
		{
			player->take_damage();
			take_damage();
		}
	}
}

void Meteor::move()
{
	if (m_hasEnteredInWindow && !is_in_window())
		destroy();
	else if (m_texture)
	{
		m_hitbox.first += m_velocity;
		m_texture->set_position(m_hitbox.first);
		if (is_in_window())
			m_hasEnteredInWindow = true;
		m_tempID = hgui::TaskManager::program(std::chrono::milliseconds(20), [this] { move(); });
		collide();
	}
}
