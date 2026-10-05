#include "stdafx.h"
#include "Game.h"
#include "Source/Character/Player/Player.h"
#include "Source/Stage/TennisCourt.h"
#include "GameCamera/GameCamera.h"
#include "Ball/Ball.h"

bool Game::Start()
{
	NewGO<TennisCourt>(0, "tenniscourt");
	NewGO<GameCamera>(0, "gamecamera");
	auto* player = NewGO<Player>(0, "player");
	player->SetPosition(Vector3(1500.0f, 0.0f,-1000.0f));/** サーブの位置 */
	NewGO<Ball>(0, "ball");

	return true;
}

void Game::Update()
{

}

void Game::Render(RenderContext& rc)
{

}