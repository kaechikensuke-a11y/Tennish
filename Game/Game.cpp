#include "stdafx.h"
#include "Game.h"
#include "Source/Character/Player/Player.h"
#include "Source/Stage/TennisCourt.h"
#include "GameCamera/GameCamera.h"

bool Game::Start()
{
	NewGO<TennisCourt>(0, "tenniscourt");
	NewGO<GameCamera>(0, "gamecamera");
	NewGO<Player>(0, "player");

	return true;
}

void Game::Update()
{

}

void Game::Render(RenderContext& rc)
{

}