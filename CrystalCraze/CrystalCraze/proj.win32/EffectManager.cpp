#include "EffectManager.h"

using namespace cocos2d;

EffectManager::EffectManager()
{

}

EffectManager::~EffectManager()
{

}

bool EffectManager::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(! CCLayer::init());
		bRet = true;
	} while (0);

	return bRet;
}

// ±¬Õ¨Ð§¹û
void EffectManager::explodeSpecial(CCPoint point, bool flag)
{
	float scaleX = 1.0f;
	float scaleY = 0.7f;
	float time = 0.3f;
	CCPoint startPosition = point;
	float speed = 0.6f;

	// ºáÏò±¬Õ¨
	if (flag)
	{
		CCSprite* colorHRight = CCSprite::create("colorHRight.png");
		this->addChild(colorHRight);
		colorHRight->setPosition(startPosition);
		colorHRight->runAction(CCSequence::create(
			CCScaleTo::create(time, scaleX, scaleY),
			CCCallFuncN::create(this, callfuncN_selector(EffectManager::disappear)),
			NULL));

		CCSprite* colorHLeft = CCSprite::create("colorHLeft.png");
		this->addChild(colorHLeft);
		colorHLeft->setPosition(startPosition);
		colorHLeft->runAction(CCSequence::create(
			CCScaleTo::create(time, scaleX, scaleY),
			CCCallFuncN::create(this, callfuncN_selector(EffectManager::disappear)),
			NULL));
	}
	// ÊúÏò±¬Õ¨
	else
	{
		CCSprite* colorVUp = CCSprite::create("colorVUp.png");
		this->addChild(colorVUp);
		colorVUp->setPosition(startPosition);
		colorVUp->runAction(CCSequence::create(
			CCScaleTo::create(time, scaleX, scaleY),
			CCCallFuncN::create(this, callfuncN_selector(EffectManager::disappear)),
			NULL));

		CCSprite* colorVDown = CCSprite::create("colorVDown.png");
		this->addChild(colorVDown);
		colorVDown->setPosition(startPosition);
		colorVDown->runAction(CCSequence::create(
			CCScaleTo::create(time, scaleX, scaleY),
			CCCallFuncN::create(this, callfuncN_selector(EffectManager::disappear)),
			NULL));
	}
}

// É¾³ý±¬Õ¨Ð§¹û
void EffectManager::disappear(CCNode* who)
{
	who->removeFromParentAndCleanup(true);
}