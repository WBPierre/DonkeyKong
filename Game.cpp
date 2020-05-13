#include "pch.h"
#include "StringHelpers.h"
#include "Game.h"
#include "EntityManager.h"
#include <iostream>
#include <chrono>
#include <fstream>

const float Game::PlayerSpeed = 100.f;
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
	: mWindow(sf::VideoMode(840, 600), "Donkey Kong - 1981 Remastered", sf::Style::Close)
	, mTexture()
	, mPlayer()
	, mFont()
	, mStatisticsText()
	, mStatisticsUpdateTime()
	, mStatisticsNumFrames(0)
	, mGameText()
	, mGameTime()
	, mIsMovingUp(false)
	, mIsMovingDown(false)
	, mIsMovingRight(false)
	, mIsMovingLeft(false)
{
	mWindow.setFramerateLimit(160);

	// Draw blocks

	_TextureBlock.loadFromFile("Media/Textures/Block.png");
	_sizeBlock = _TextureBlock.getSize();

	for (int i = 0; i < BLOCK_COUNT_X; i++)
	{
		for (int j = 0; j < BLOCK_COUNT_Y; j++)
		{
			_Block[i][j].setTexture(_TextureBlock);
			_Block[i][j].setPosition(100.f + 70.f * (i + 1), 0.f + BLOCK_SPACE * (j + 1));

			std::shared_ptr<Entity> se = std::make_shared<Entity>();
			se->m_sprite = _Block[i][j];
			se->m_type = EntityType::block;
			se->m_size = _TextureBlock.getSize();
			se->m_position = _Block[i][j].getPosition();
			EntityManager::m_Entities.push_back(se);
		}
	}

	// Draw Echelles

	_TextureEchelle.loadFromFile("Media/Textures/Echelle.png");

	for (int i = 0; i < ECHELLE_COUNT; i++)
	{
		_Echelle[i].setTexture(_TextureEchelle);
		_Echelle[i].setPosition(100.f + 70.f * (i + 1), 0.f + BLOCK_SPACE * (i + 1) + _sizeBlock.y);

		std::shared_ptr<Entity> se = std::make_shared<Entity>();
		se->m_sprite = _Echelle[i];
		se->m_type = EntityType::echelle;
		se->m_size = _TextureEchelle.getSize();

		se->m_position = _Echelle[i].getPosition();
		EntityManager::m_Entities.push_back(se);
	}

	// Draw Mario

	mTexture.loadFromFile("Media/Textures/marioRight.png"); // Mario_small.png");
	_sizeMario = mTexture.getSize();
	mTexture.setSmooth(true);
	mPlayer.setTexture(mTexture);
	sf::Vector2f posMario;
	posMario.x = 100.f + 70.f;
	posMario.y = BLOCK_SPACE * 5 - _sizeMario.y;

	mPlayer.setPosition(posMario);

	std::shared_ptr<Entity> player = std::make_shared<Entity>();
	player->m_sprite = mPlayer;
	player->m_type = EntityType::player;
	player->m_size = mTexture.getSize();
	player->m_position = mPlayer.getPosition();
	EntityManager::m_Entities.push_back(player);

	// Draw Statistic Font 

	mFont.loadFromFile("Media/Sansation.ttf");
	mStatisticsText.setString("Welcome to Donkey Kong 1981");
	mStatisticsText.setFont(mFont);
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10);

	mGameText.setString("Dunkey Kong 1981 - Remastered");
	mGameText.setFont(mFont);
	mGameText.setPosition(300.f, 20.f);
	mGameText.setCharacterSize(24);

	mGameTime.setFont(mFont);
	mGameTime.setPosition(5.f, 120.f);
	mGameTime.setCharacterSize(18);

	mGameRecord.setFont(mFont);
	mGameRecord.setPosition(5.f, 140.f);
	mGameRecord.setCharacterSize(12);
}

void Game::run()
{
	sf::Clock clock;
	sf::Clock inGameClock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time startingTime = inGameClock.getElapsedTime();
	checkForPersonalRecord();
	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processEvents();
			update(TimePerFrame);
		}
		startingTime = inGameClock.getElapsedTime();
		gameText(startingTime);
		updateStatistics(elapsedTime);
		render();
	}
}

void Game::gameText(sf::Time startingTime) {
	if (startingTime.asSeconds() < 60) {
		if (startingTime.asSeconds() > 10) {
			mGameTime.setString("Time : " + toString(startingTime.asSeconds()).substr(0, 5));
		}
		else {
			mGameTime.setString("Time : " + toString(startingTime.asSeconds()).substr(0, 4));
		}
		
	}
	else {
		int min = startingTime.asSeconds() / 60;
		float seconds = fmod(startingTime.asSeconds(), 60);
		if (seconds > 10) {
			mGameTime.setString("Time : " + toString(min) + "'" + toString(seconds).substr(0,5));
		}
		else {
			mGameTime.setString("Time : " + toString(min) + "'" + toString(seconds).substr(0, 4));
		}
		
	}
	
}

void Game::checkForRecordFile() {
	std::fstream recordFile;
	recordFile.open("record.txt", std::fstream::in | std::fstream::out);
	if (!recordFile) {
		recordFile.open("record.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
		recordFile << "record=null";
		recordFile.close();
	}
	else {
		recordFile.close();
	}
}

void Game::checkForPersonalRecord() {
	checkForRecordFile();
	std::fstream recordFile;
	recordFile.open("record.txt", std::fstream::in | std::fstream::out | std::fstream::app);
	std::string line;
	std::getline(recordFile, line);
	line = line.substr(7);
	std::cout << line;
	if (line == "null") {
		mGameRecord.setString("No record");
	}
	else {
		int lastRecord = std::stoi(line);
		mGameRecord.setString("Personal record : " + toString(lastRecord/60) + "'" + toString(lastRecord%60));
	}
	recordFile.close();
}

void Game::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			handlePlayerInput(event.key.code, true);
			break;

		case sf::Event::KeyReleased:
			handlePlayerInput(event.key.code, false);
			break;

		case sf::Event::Closed:
			mWindow.close();
			break;
		}
	}
}

void Game::update(sf::Time elapsedTime)
{

	std::shared_ptr<Entity> player = EntityManager::GetPlayer();
	sf::Vector2f movement(0.f, 0.f);
	if (mIsMovingUp) movement.y -= PlayerSpeed;
	if (mIsMovingDown) movement.y += PlayerSpeed;
	if (mIsMovingLeft) movement.x -= PlayerSpeed;
	if (mIsMovingRight) movement.x += PlayerSpeed;

	if (player->m_enabled != false) {
		/*
		if (player->velocityY > 0) {
			movement.y -= 1.5 * PlayerSpeed;
			player->velocityY -= 1.5 * PlayerSpeed;
		} else {
			goingDown(elapsedTime, player);
		}
		*/
		if (mIsMovingDown && !canGoDown(elapsedTime, player)) {
			movement.y -= PlayerSpeed;
		}
		if (mIsMovingUp && !canGoUp(elapsedTime, player)) {
			movement.y += PlayerSpeed;
		}
		if (mIsMovingLeft && player->m_climbing) {
			movement.x += PlayerSpeed;
		}
		if (mIsMovingRight && player->m_climbing) {
			movement.x -= PlayerSpeed;
		}

		updateTextures(elapsedTime);

		player->m_sprite.move(movement * elapsedTime.asSeconds());
	}
}

void Game::updateTextures(sf::Time elapsedTime) {
	std::shared_ptr<Entity> player = EntityManager::GetPlayer();
	if (mIsMovingLeft) {
		mTexture.loadFromFile("Media/Textures/marioLeft.png");
	}
	if (mIsMovingRight) {
			mTexture.loadFromFile("Media/Textures/marioRight.png");
	}
	if (mIsMovingUp) {
		mTexture.loadFromFile("Media/Textures/marioClimb1.png");
	}
	
}

void Game::render()
{
	mWindow.clear();

	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		mWindow.draw(entity->m_sprite);
	}

	mWindow.draw(mStatisticsText);
	mWindow.draw(mGameText);
	mWindow.draw(mGameTime);
	mWindow.draw(mGameRecord);
	mWindow.display();
}

void Game::updateStatistics(sf::Time elapsedTime)
{
	mStatisticsUpdateTime += elapsedTime;
	mStatisticsNumFrames += 1;

	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString(
			"Frames / Second = " + toString(mStatisticsNumFrames) + "\n" +
			"Time / Update = " + toString(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us");

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}

	//
	// Handle collision
	//

	if (mStatisticsUpdateTime >= sf::seconds(0.050f))
	{
		// Handle collision weapon enemies
	}
}

void Game::goingDown(sf::Time elapsedTime, std::shared_ptr<Entity> player) {
	bool grounded = false;
	sf::Vector2f movement(0.f, 0.f);

	sf::FloatRect boundPlayer = player->m_sprite.getGlobalBounds();

	for (std::shared_ptr<Entity> block : EntityManager::m_Entities) {
		if (block->m_enabled == false) {
			continue;
		}

		if (block->m_type != EntityType::block) {
			continue;
		}

		sf::FloatRect boundBlock = block->m_sprite.getGlobalBounds();

		if (boundPlayer.intersects(boundBlock) == true) {
			grounded = true;
			break;
		}
	}
	
	player->m_jumping = false;

	if (grounded == false) {
		movement.y += 2 * PlayerSpeed;
		player->m_sprite.move(movement * elapsedTime.asSeconds());
	}
}

bool Game::canGoDown(sf::Time elapsedTime, std::shared_ptr<Entity> player) {
	sf::FloatRect boundPlayer = player->m_sprite.getGlobalBounds();
	for (std::shared_ptr<Entity> block : EntityManager::m_Entities) {
		if (block->m_enabled == false) continue;
		if (block->m_type == EntityType::block) {
			sf::FloatRect boundBlock = block->m_sprite.getGlobalBounds();
			if (boundPlayer.intersects(boundBlock) == true && (boundPlayer.top < boundBlock.top)) {
				return false;
			}
		}
	}
	return true;
}

bool Game::canGoUp(sf::Time elapsedTime, std::shared_ptr<Entity> player) {
	sf::FloatRect boundPlayer = player->m_sprite.getGlobalBounds();
	bool shouldGoUp = false;
	for (std::shared_ptr<Entity> block : EntityManager::m_Entities) {
		if (block->m_enabled == false) continue;
		if (block->m_type == EntityType::echelle) {
			sf::FloatRect boundBlock = block->m_sprite.getGlobalBounds();
			if (boundPlayer.intersects(boundBlock) == true) {
				player->m_climbing = true;
				shouldGoUp = true;
			}
		}
		if (block->m_type == EntityType::block) {
			sf::FloatRect boundBlock = block->m_sprite.getGlobalBounds();
			if (boundPlayer.intersects(boundBlock) == true) {
				shouldGoUp = true;
			}
		}
	}
	if (shouldGoUp) {
		player->m_climbing = true;
		return true;
	}
	else {
		player->m_climbing = false;
		return false;
	}
	
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	std::shared_ptr<Entity> player = EntityManager::GetPlayer();

	if (key == sf::Keyboard::Up) {
		mIsMovingUp = isPressed;
	} else if (key == sf::Keyboard::Down) {
		mIsMovingDown = isPressed;
				
	} else if (key == sf::Keyboard::Left) {
		mIsMovingLeft = isPressed;
	} else if (key == sf::Keyboard::Right) {
		mIsMovingRight = isPressed;
	}

	if (key == sf::Keyboard::Space)
	{
		if (isPressed == false) {
			if (player->m_enabled != false) {
				if (player->m_jumping == false) {
					player->velocityY += 1200.f;
					player->m_jumping = true;
				}
			}

			//printf("jump: %d", player->m_jumping);			
		}
	}
}
