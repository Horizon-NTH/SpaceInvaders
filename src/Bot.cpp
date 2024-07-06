#include "../include/Bot.h"
#include "../include/BotShot.h"
#include "../include/Player.h"

Bot::Bot(const hgui::point& position, const unsigned level, const bool shielded, const bool invincibility, const std::function<void(const std::shared_ptr<Bot>&)>& onDeath) :
	SpaceShip("bot_" + std::to_string(level), hitbox(position, hgui::size(5_em).set_reference(hgui::reference::HEIGHT)), level, shielded, level, std::make_tuple(level == 3u ? 2u : 1u, level == 1u ? 1u : 2u, std::make_pair(std::chrono::milliseconds(1000 - 150 * level), hgui::Timer()))),
	m_hasEnteredWindow(false),
	m_collisonProcessed(false),
	m_onDeath(onDeath)
{
	if (invincibility)
	{
		make_invincible(std::chrono::seconds(10));
	}
	get_random_velocity();
	Bot::move();
	m_shotTempID = hgui::TaskManager::program(std::get<2>(m_weaponStats).first, [this] { shoot(); });
}

Bot::~Bot()
{
	if (hgui::TaskManager::is_program(m_tempID))
		hgui::TaskManager::deprogram(m_tempID);
	if (hgui::TaskManager::is_program(m_shotTempID))
		hgui::TaskManager::deprogram(m_shotTempID);
}

void Bot::move()
{
	if (m_shipTexture)
	{
		m_collisonProcessed = false;
		const auto windowSize = hgui::size(100_em);
		m_hitbox.first += m_velocity;
		if (!m_hasEnteredWindow && is_in_window())
			m_hasEnteredWindow = true;
		else if (m_hasEnteredWindow)
		{
			if (m_hitbox.first.x < 0 || m_hitbox.first.x + m_hitbox.second.width > windowSize.width)
			{
				m_velocity.x = -m_velocity.x;
				m_hitbox.first.em_x = hgui::kernel::EM<float>{} + (m_hitbox.first.x < 0 ? 0 : windowSize.width - m_hitbox.second.width);
			}
			if (m_hitbox.first.y < 0 || m_hitbox.first.y + m_hitbox.second.height > windowSize.height)
			{
				m_velocity.y = -m_velocity.y;
				m_hitbox.first.em_y = hgui::kernel::EM<float>{} + (m_hitbox.first.y < 0 ? 0 : windowSize.height - m_hitbox.second.height);
			}
			m_hitbox.first.update();
		}
		m_shipTexture->set_position(m_hitbox.first);
		m_tempID = hgui::TaskManager::program(std::chrono::milliseconds(20), [this]
			{
				move();
				collide();
			});
	}
}

bool Bot::shoot()
{
	if (m_shipTexture)
	{
		m_shots.erase(std::begin(std::ranges::remove_if(m_shots, [](const std::unique_ptr<SpaceShip::Shot>& shot) { return shot->is_destroyed(); })), m_shots.end());
		m_shotTempID = hgui::TaskManager::program(std::get<2>(m_weaponStats).first, [this] { shoot(); });
		if (m_shots.size() < std::get<1>(m_weaponStats))
		{
			auto image = hgui::image_loader(std::string("assets/textures/lasers/laser_") + (m_level == 3u ? "red" : "green") + ".png");
			const float ratio = image->get_size().width / image->get_size().height;
			const hgui::size size = hgui::size(2_em * ratio, 2_em).set_reference(hgui::reference::HEIGHT);
			hgui::point position = m_hitbox.first + hgui::point(m_hitbox.second.width / 2.f, m_hitbox.second.height + size.height / 2);
			hitbox hitbox = std::make_pair(position - size / 2., size);
			m_shots.emplace_back(std::make_unique<Shot>(image, hitbox, position, m_level));
			return true;
		}
	}
	return false;
}

const std::vector<std::unique_ptr<SpaceShip::Shot>>& Bot::get_shots()
{
	return m_shots;
}

void Bot::destroy()
{
	m_shipTexture = nullptr;
	if (hgui::TaskManager::is_program(m_tempID))
		hgui::TaskManager::deprogram(m_tempID);
	const auto gif = hgui::gif_loader("assets/textures/explosions/explosion_" + m_name + ".gif");
	const float aspectRatio = gif->get_size().width / gif->get_size().height;
	const auto size = hgui::size(5_em).set_reference(hgui::reference::HEIGHT) * aspectRatio;
	const auto position = m_hitbox.first + m_hitbox.second / 2 - size / 2;
	const auto explosion = hgui::SpriteManager::create(gif, size, position);
	explosion->play();
	hgui::kernel::GIFData::delay totalDelay(100);
	for (const auto& delay : gif->get_data().pixels | std::views::values)
		totalDelay += delay;
	hgui::TaskManager::program(totalDelay, [explosion] {});
	if (m_onDeath)
		m_onDeath(std::dynamic_pointer_cast<Bot>(shared_from_this()));
}

void Bot::get_random_velocity()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution randomScreenXCoord(hgui::point(5_em).x, hgui::point(95_em).x),
			randomScreenYCoord(hgui::point(5_em).y, hgui::point(95_em).y);
	m_velocity = static_cast<hgui::vec2>(hgui::point::normalize(hgui::point(randomScreenXCoord(gen), randomScreenYCoord(gen)) - m_hitbox.first)) * static_cast<float>(m_level) * 2.f;
}

void Bot::collide()
{
	std::vector<std::weak_ptr<Entity>> collided;
	std::ranges::copy_if(m_gameEntities, std::back_inserter(collided), [&](const std::weak_ptr<Entity>& ptr)
		{
			if (const auto entity = ptr.lock(); entity && entity->is_alive() && entity != shared_from_this())
				return entity->is_overlapping(m_hitbox);
			return false;
		});
	if (!collided.empty())
	{
		for (const auto& ptr : collided)
		{
			if (const auto entity = ptr.lock())
			{
				if (const auto player = std::dynamic_pointer_cast<Player>(entity))
				{
					player->take_damage();
					take_damage();
					return;
				}

				const auto hitbox = entity->get_hitbox();
				const float overlapX = (hitbox.first.x + hitbox.second.width / 2) - (m_hitbox.first.x + m_hitbox.second.width / 2);
				const float overlapY = (hitbox.first.y + hitbox.second.height / 2) - (m_hitbox.first.y + m_hitbox.second.height / 2);

				if (const auto [absOverlapX, absOverlapY] = std::pair{std::abs(overlapX), std::abs(overlapY)}; absOverlapX > absOverlapY)
				{
					if (overlapX > 0)
					{
						hitbox.first.em_x += absOverlapX;
						m_hitbox.first.em_x -= absOverlapX;
					}
					else
					{
						hitbox.first.em_x -= absOverlapX;
						m_hitbox.first.em_x += absOverlapX;
					}
				}
				else
				{
					if (overlapY > 0)
					{
						hitbox.first.em_y += absOverlapY;
						m_hitbox.first.em_y -= absOverlapY;
					}
					else
					{
						hitbox.first.em_y -= absOverlapY;
						m_hitbox.first.em_y += absOverlapY;
					}
				}
				if (const auto bot = std::dynamic_pointer_cast<Bot>(entity))
				{
					if (bot->m_collisonProcessed || m_collisonProcessed || !m_hasEnteredWindow || !bot->m_hasEnteredWindow)
						return;
					auto temp = m_velocity;
					temp.normalize();
					m_velocity = bot->m_velocity;
					m_velocity.normalize();
					m_velocity *= 2.f * static_cast<float>(m_level);
					bot->m_velocity = temp;
					bot->m_velocity *= 2.f * static_cast<float>(bot->m_level);
					m_hitbox.first.update();
					bot->m_hitbox.first.update();
					m_collisonProcessed = true;
					bot->m_collisonProcessed = true;
				}
				return;
			}
		}
	}
}
