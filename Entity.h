#pragma once

enum EntityType
{
	player,
	block,
	echelle,
	peach,
	DK,
	enemy,
	heart,
	banana
};

class Entity
{
public:
	Entity() { };
	~Entity() { };

public:
	sf::Sprite m_sprite;
	sf::Vector2u m_size;
	sf::Vector2f m_position;
	EntityType m_type;
	bool m_enabled = true;

	// Enemy only
	bool m_bLeftToRight = true;
	int m_times = 0;

	// Player only
	bool m_jumping = false;
	bool complete_jump = false;
	float initial_y;
	bool m_climbing = false;
	bool m_lastMove = true;
	int m_current_texture = 0;
};

