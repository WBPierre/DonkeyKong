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
	, marioFont()
	, mPeach()
	, mDK()
	, mBanana()
	, mStatisticsText()
	, mStatisticsUpdateTime()
	, mStatisticsNumFrames(0)
	, mGameText()
	, mGameTime()
	, mGameEnd()
	, mGameEndSub()
	, mGameCommand()
	, mIsMovingUp(false)
	, mIsMovingDown(false)
	, mIsMovingRight(false)
	, mIsMovingLeft(false)
	, mIsShooting(false)
	, lookingAt(true)
	, shotFired(false)
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

	_TextureEchelle.loadFromFile("Media/Textures/Echelle_resized.png");

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

	// Draw Enemies

	_TextureEnemy.loadFromFile("Media/Textures/enemy.png");
	for (int i = 0; i < ENEMY_COUNT; i++) {
		_Enemies[i].setTexture(_TextureEnemy);
		_Enemies[i].setPosition(150.f + 150.f *(i + 1), 0.f + BLOCK_SPACE * (i + 1)+ 25 + _sizeBlock.y);
		std::shared_ptr<Entity> se = std::make_shared<Entity>();
		se->m_sprite = _Enemies[i];
		se->m_type = EntityType::enemy;
		se->m_size = _TextureEnemy.getSize();

		se->m_position = _Enemies[i].getPosition();
		EntityManager::m_Entities.push_back(se);
	}

	

	// Draw hearts

	_TextureHeart.loadFromFile("Media/Textures/heart.png");
	for (int i = 0; i < HEART_COUNT; i++) {
		_Hearts[i].setTexture(_TextureHeart);
		_Hearts[i].setPosition(5.f + (i * 30.f), 160.f);
		std::shared_ptr<Entity> se = std::make_shared<Entity>();
		se->m_sprite = _Hearts[i];
		se->m_type = EntityType::heart;
		se->m_size = _TextureHeart.getSize();
		se->m_position = _Hearts[i].getPosition();
		EntityManager::m_Entities.push_back(se);
	}

	// Draw DK

	_TextureDK.loadFromFile("Media/Textures/dkjr.png");
	_sizeDK = _TextureDK.getSize();
	_TextureDK.setSmooth(true);
	mDK.setTexture(_TextureDK);
	sf::Vector2f posDK;
	posDK.x = 600.f;
	posDK.y = BLOCK_SPACE - _sizeDK.y;

	mDK.setPosition(posDK);

	std::shared_ptr<Entity> dk = std::make_shared<Entity>();
	dk->m_sprite = mDK;
	dk->m_type = EntityType::DK;
	dk->m_size = _TextureDK.getSize();
	dk->m_position = mDK.getPosition();
	EntityManager::m_Entities.push_back(dk);

	// Draw Peach
	
	_TexturePeach.loadFromFile("Media/Textures/peach.png");
	_sizePeach = _TexturePeach.getSize();
	_TexturePeach.setSmooth(true);
	mPeach.setTexture(_TexturePeach);
	sf::Vector2f posPeach;
	posPeach.x = 680.f;
	posPeach.y = BLOCK_SPACE - _sizePeach.y;

	mPeach.setPosition(posPeach);

	std::shared_ptr<Entity> peach = std::make_shared<Entity>();
	peach->m_sprite = mPeach;
	peach->m_type = EntityType::peach;
	peach->m_size = _TexturePeach.getSize();
	peach->m_position = mPeach.getPosition();
	EntityManager::m_Entities.push_back(peach);
	

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

	// Draw banana

	_TextureBanana.loadFromFile("Media/Textures/banana.png");
	_sizeBanana = _TextureBanana.getSize();
	_TextureBanana.setSmooth(true);
	mBanana.setTexture(_TextureBanana);
	mBanana.setPosition(player->m_sprite.getPosition());
	std::shared_ptr<Entity> banana = std::make_shared<Entity>();
	banana->m_sprite = mBanana;
	banana->m_type = EntityType::banana;
	banana->m_size = _TextureBanana.getSize();
	banana->m_position = mBanana.getPosition();
	banana->m_enabled = false;
	EntityManager::m_Entities.push_back(banana);


	// Draw Statistic Font 

	mFont.loadFromFile("Media/Sansation.ttf");
	mStatisticsText.setString("Welcome to Donkey Kong 1981");
	mStatisticsText.setFont(mFont);
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10);

	marioFont.loadFromFile("Media/SuperMarioBros.ttf");
	mGameText.setString("Dunkey Kong 1981 - Remastered");
	mGameText.setFont(marioFont);
	mGameText.setPosition(250.f, 20.f);
	mGameText.setCharacterSize(24);


	mGameTime.setFont(marioFont);
	mGameTime.setPosition(5.f, 120.f);
	mGameTime.setCharacterSize(18);

	mGameRecord.setFont(marioFont);
	mGameRecord.setPosition(5.f, 140.f);
	mGameRecord.setCharacterSize(12);

	mGameCommand.setFont(marioFont);
	mGameCommand.setPosition(5.f, 200.f);
	mGameCommand.setCharacterSize(14);
	mGameCommand.setString("Press E to fire");
	
	mGameEnd.setFont(marioFont);
	mGameEnd.setPosition(400.f, 250.f);
	mGameEnd.setCharacterSize(32);
	mGameEnd.setString("You won !");

	mGameEndSub.setFont(marioFont);
	mGameEndSub.setPosition(375.f, 300.f);
	mGameEndSub.setCharacterSize(24);
	mGameEndSub.setString("You can now leave");

}

void Game::run()
{
	sf::Clock clock;
	sf::Clock inGameClock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time startingTime = inGameClock.getElapsedTime();
	gameIsEnded = false;
	life = HEART_COUNT;
	checkForPersonalRecord();
	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			processEvents();
			if (!gameIsEnded) {
				update(TimePerFrame);
				animateEnemies();
				animateBanana();
			}
		}
		if (!gameIsEnded) {
			startingTime = inGameClock.getElapsedTime();
		}
		gameText(startingTime);
		updateStatistics(elapsedTime);
		render();
	}
}



void Game::animateEnemies() {
	sf::Vector2f movement(-2.f, 0.f);
	for (std::shared_ptr<Entity> block : EntityManager::m_Entities) {
		if (block->m_enabled == false) {
			continue;
		}
		if (block->m_type == EntityType::enemy) {
			block->m_sprite.move(movement);
			if (block->m_sprite.getPosition().x == 170.f) {
				block->m_sprite.setPosition(700.f, block->m_sprite.getPosition().y);
			}
		}
	}
}

bool Game::endOfGame() {
	std::shared_ptr<Entity> player = EntityManager::GetPlayer();
	sf::FloatRect boundPlayer = player->m_sprite.getGlobalBounds();

	for (std::shared_ptr<Entity> block : EntityManager::m_Entities) {
		if (block->m_enabled == false) {
			continue;
		}
		if (block->m_type == EntityType::peach) {
			sf::FloatRect boundPeach = block->m_sprite.getGlobalBounds();
			if (boundPlayer.intersects(boundPeach) == true) {
				gameIsEnded = true;
				return true;
			}
		}
	}
	return false;
}

void Game::gameText(sf::Time startingTime) {
	if (Game::endOfGame() == false) {
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
				mGameTime.setString("Time : " + toString(min) + "'" + toString(seconds).substr(0, 5));
			}
			else {
				mGameTime.setString("Time : " + toString(min) + "'" + toString(seconds).substr(0, 4));
			}

		}
	} else {
		int timer = startingTime.asSeconds();
		if (record == -1 || record > timer) {
			std::fstream recordFile;
			recordFile.open("record.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
			recordFile << "record=" + toString(timer);
			recordFile.close();
			mGameEnd.setString("You won ! New best record !");
		}
		else {
			mGameEnd.setString("You won !");
		}
		
		mGameEndSub.setString("You can now leave");
	}
}

void Game::checkForRecordFile() {
	std::fstream recordFile;
	recordFile.open("record.txt", std::fstream::in | std::fstream::out);
	if (!recordFile) {
		recordFile.open("record.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
		recordFile << "record=null";
	}
	recordFile.close();
}

void Game::checkForPersonalRecord() {
	checkForRecordFile();
	std::fstream recordFile;
	recordFile.open("record.txt", std::fstream::in | std::fstream::out | std::fstream::app);
	std::string line;
	std::getline(recordFile, line);
	line = line.substr(7);
	if (line == "null") {
		mGameRecord.setString("No record");
		record = - 1;
	}
	else {
		int lastRecord = std::stoi(line);
		record = lastRecord;
		std::cout << lastRecord;
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

void Game::handleLife() {
	for (std::shared_ptr<Entity> block : EntityManager::m_Entities) {
		if (block->m_enabled == false) continue;
		if (block->m_type == EntityType::heart) {
			block->m_enabled = false;
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
	if (mIsShooting) shoot();

	if (player->m_enabled != false) {
		if (player->m_jumping) {
			if (player->m_sprite.getPosition().y > player->initial_y - MAX_JUMP) {
				movement.y -= 1.5 * PlayerSpeed;
			}
			else {
				player->complete_jump = true;
				player->m_jumping = false;
			}	
		}
		if (player->complete_jump) {
			if (player->m_sprite.getPosition().y < player->initial_y) {
				movement.y += 1.5 * PlayerSpeed;
			}
			else {
				player->complete_jump = false;
			}
		}
		
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


		// Enemy collision
		sf::FloatRect boundPlayer = player->m_sprite.getGlobalBounds();

		for (std::shared_ptr<Entity> block : EntityManager::m_Entities) {
			if (block->m_enabled == false) {
				continue;
			}
			if (block->m_type == EntityType::enemy){
				sf::FloatRect boundEnemy = block->m_sprite.getGlobalBounds();
				if (boundPlayer.intersects(boundEnemy) == true) {
					player->m_sprite.setPosition(170.f, BLOCK_SPACE * 5 - _sizeMario.y);
					life--;
					Game::handleLife();
				}
			}
		}
	}
}

void Game::animateBanana() {
	sf::Vector2f movement;
	movement.y = 0.f;
	if (lookingAt) {
		movement.x = 3.f;
	}
	else {
		movement.x = -3.f;
	}
	
	for (std::shared_ptr<Entity> block : EntityManager::m_Entities) {
		if (block->m_type == EntityType::banana && block->m_enabled) {
			if (block->m_sprite.getPosition().x <= (bananaStart.x+ 150.f) ) {
				block->m_sprite.move(movement);
				std::shared_ptr<Entity> player = EntityManager::GetPlayer();
				sf::FloatRect boundBanana = block->m_sprite.getGlobalBounds();
				for (std::shared_ptr<Entity> enemy : EntityManager::m_Entities) {
					if (enemy->m_enabled && enemy->m_type == EntityType::DK) {
						sf::FloatRect boundEnemy = enemy->m_sprite.getGlobalBounds();
						if (boundBanana.intersects(boundEnemy) == true) {
							enemy->m_enabled = false;
							block->m_enabled = false;
							shotFired = false;
						}
					}
				}
			} else {
				block->m_enabled = false;
				shotFired = false;
			}
		}
	}
}

void Game::shoot() {
	if (!shotFired) {
		std::shared_ptr<Entity> player = EntityManager::GetPlayer();
		if (player->m_climbing == false) {
			for (std::shared_ptr<Entity> block : EntityManager::m_Entities) {
				if (block->m_type == EntityType::banana) {
					block->m_enabled = true;
					block->m_sprite.setPosition(player->m_sprite.getPosition().x + 40.f, player->m_sprite.getPosition().y);
					bananaStart.x = player->m_sprite.getPosition().x + 40.f;
					bananaStart.y = player->m_sprite.getPosition().y;
					shotFired = true;
				}
			}
		}
	}
	mIsShooting = false;
}

void Game::updateTextures(sf::Time elapsedTime) {
	std::shared_ptr<Entity> player = EntityManager::GetPlayer();
	if (mIsMovingLeft) {
		lookingAt = false;
		mTexture.loadFromFile("Media/Textures/marioLeft.png");
	}
	if (mIsMovingRight) {
		lookingAt = true;
		mTexture.loadFromFile("Media/Textures/marioRight.png");
	}
	if (mIsMovingUp && player->m_climbing) {
		mTexture.loadFromFile("Media/Textures/marioClimb1.png");
	}
}

void Game::render()
{
	mWindow.clear();
	mWindow.draw(mGameTime);
	mWindow.draw(mGameRecord);
	if (!gameIsEnded) {
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
		mWindow.draw(mGameCommand);
	}
	else {
		mWindow.draw(mGameEnd);
		mWindow.draw(mGameEndSub);
	}
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
	if (key == sf::Keyboard::E && mIsShooting == false) {
		mIsShooting = isPressed;
	}

	if (key == sf::Keyboard::Space)
	{
		if (isPressed == false) {
			player->m_jumping = true;
			player->initial_y = player->m_sprite.getPosition().y;		
		}
	}
}
