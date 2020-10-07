#include "LionheartEngine.h"
#include <iostream>
bool LionheartEngine::Initialise(HINSTANCE hInstance, std::string strWinClass, std::string strWinTitle, int width, int height)
{
	timer.Start();
	this->kl.SetAutoRepeatChars(false);
	this->kl.SetAutoRepeatKeys(false);
	if(!rw.Initialise(this, hInstance, strWinClass, strWinTitle, width, height)) return false;
	if(!gfx.Initialise(this->rw.GetHWin(), width, height)) return false;

	return true;
}

bool LionheartEngine::ProcessMessage()
{
	return this->rw.ProcessMessage();
}

void LionheartEngine::Update()
{
	float dt = (float) timer.GetMillisecondsElapsed();
	timer.Restart();

	while (!this->kl.CharQueueIsEmpty())
	{
		unsigned char ch = this->kl.NextChar();
	}
	while (!this->kl.EventQueueIsEmpty())
	{
		KeyEvent ke = this->kl.NextKey();
		unsigned char keycode = ke.GetKey();

	}
	while (!ml.EventQueueIsEmpty())
	{
		MouseEvent me = ml.NextEvent();

		if (me.GetType() == MouseEvent::MouseEventType::RAW_MOVE)
		{
			gfx.mainView.AdjustRotation((float)me.GetPosY() * 0.001f, (float)me.GetPosX() * 0.001f, 0.0f);
		}
	}

	float baseViewSpeed = 0.001f;

	if (kl.KeyIsDown(VK_SHIFT)) baseViewSpeed *= 5;

	const float viewSpeed = baseViewSpeed * dt;

	if (kl.KeyIsDown('W')) gfx.mainView.AdjustPosition(gfx.mainView.GetForwardVector() * viewSpeed);
	if (kl.KeyIsDown('S')) gfx.mainView.AdjustPosition(gfx.mainView.GetBackVector() * viewSpeed);
	if (kl.KeyIsDown('A')) gfx.mainView.AdjustPosition(gfx.mainView.GetLeftVector() * viewSpeed);
	if (kl.KeyIsDown('D')) gfx.mainView.AdjustPosition(gfx.mainView.GetRightVector() * viewSpeed);
	if (kl.KeyIsDown('Q')) gfx.model.AdjustRotation(0.0f, -0.001f, 0.0f);
	if (kl.KeyIsDown('E')) gfx.model.AdjustRotation(0.0f, 0.001f, 0.0f);
	if (kl.KeyIsDown(VK_SPACE)) gfx.mainView.AdjustPosition(gfx.mainView.GetUpVector() * viewSpeed);
	if (kl.KeyIsDown(VK_CONTROL)) gfx.mainView.AdjustPosition(gfx.mainView.GetDownVector() * viewSpeed);
	if (ml.IsLeftDown()) gfx.model.SetLookAtPos(gfx.mainView.GetPositionFloat3());
}

void LionheartEngine::Render()
{
	this->gfx.RenderFrame();
}
