#pragma once

#define ECHELLE_COUNT 4
#define ENEMY_COUNT 3
#define HEART_COUNT 3
#define BLOCK_COUNT_X 8
#define BLOCK_COUNT_Y 5
#define BLOCK_SPACE 110.f
#define MAX_JUMP 30.f

#include "EntityManager.h"

class Game
{
public:
	Game();
	~Game() { };
	void run();

private:
	void processEvents();
	void update(sf::Time elapsedTime);
	void render();

	void updateStatistics(sf::Time elapsedTime);
	void gameText(sf::Time startingTime);
	void goingDown(sf::Time elapsedTime, std::shared_ptr<Entity> player);
	bool canGoDown(sf::Time elapsedTime, std::shared_ptr<Entity> player);
	bool canGoUp(sf::Time elapsedTime, std::shared_ptr<Entity> player);
	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
	void updateTextures(sf::Time elapsedTime);
	void animateBanana();
	void animateEnemies();
	bool endOfGame();
	void shoot();
	void handleLife();
	void checkForPersonalRecord();
	void checkForRecordFile();

private:
	static const float		PlayerSpeed;
	static const sf::Time	TimePerFrame;

	sf::RenderWindow		mWindow;
	sf::Texture	mTexture;
	sf::Sprite	mPlayer;
	sf::Sprite	mPeach;
	sf::Sprite  mDK;
	sf::Sprite  mBanana;
	sf::Font	mFont;
	sf::Font	marioFont;
	sf::Text	mStatisticsText;
	sf::Text	mGameText;
	sf::Text	mGameTime;
	sf::Text	mGameRecord;
	sf::Text	mGameEnd;
	sf::Text	mGameEndSub;
	sf::Text	mGameCommand;
	sf::Time	mStatisticsUpdateTime;
	sf::Vector2f bananaStart;

	std::size_t	mStatisticsNumFrames;
	bool mIsMovingUp;
	bool mIsMovingDown;
	bool mIsMovingRight;
	bool mIsMovingLeft;
	bool mIsShooting;
	bool gameIsEnded;
	bool lookingAt;
	bool shotFired;
	int record;
	int life;

	sf::Texture	_TextureEchelle;
	sf::Sprite	_Echelle[ECHELLE_COUNT];
	sf::Sprite	_Enemies[ENEMY_COUNT];
	sf::Sprite	_Hearts[HEART_COUNT];
	sf::Texture	_TextureBlock;
	sf::Texture	_TexturePeach;
	sf::Texture	_TextureDK;
	sf::Texture	_TextureEnemy;
	sf::Texture	_TextureHeart;
	sf::Texture _TextureBanana;
	sf::Sprite	_Block[BLOCK_COUNT_X][BLOCK_COUNT_Y];
	sf::Texture	_TextureWeapon;
	sf::Sprite	_Weapon;
	sf::Vector2u _sizeBlock;
	sf::Vector2u _sizeMario;
	sf::Vector2u _sizePeach;
	sf::Vector2u _sizeDK;
	sf::Vector2u _sizeEnemy;
	sf::Vector2u _sizeHeart;
	sf::Vector2u _sizeBanana;
};

