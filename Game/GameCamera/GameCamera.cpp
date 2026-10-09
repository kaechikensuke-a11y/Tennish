#include "stdafx.h"
#include "GameCamera.h"
namespace
{
	/** 注視点 */
	const Vector3 TARGET_POS = { 0.0f,0.0f,500.0f };
	/** 固定カメラの座標 */
	const Vector3 FIXEDCAMERA_POS = { 0.0f,1900.0f,-3500.0f };
}


bool GameCamera::Start()
{
	m_CameraPos.Set(FIXEDCAMERA_POS);
	m_CameraTarget.Set(TARGET_POS);

	/** 描画範囲の設定 */
	g_camera3D->SetNear(10.0f);
	g_camera3D->SetFar(20000.0f);

	g_camera3D->SetPosition(m_CameraPos);
	g_camera3D->SetTarget(m_CameraTarget);

	return true;
}

void GameCamera::Update()
{
	g_camera3D->SetPosition(m_CameraPos);
	g_camera3D->SetTarget(m_CameraTarget);
}