#include "Gem.h"

using namespace cocos2d;

Gem::Gem()
{
	x = y = 0;
}

Gem::~Gem()
{

}

bool Gem::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(! CCSprite::init());



		bRet = true;
	} while (0);

	return bRet;
}

// �ƶ���ʯ
void Gem::removeGem(int speed, int tag, CCPoint p)
{
	this->stopAllActions();
	this->runAction(CCMoveTo::create(speed, p));
	this->setTag(tag);

}

// �ƶ���ʯ������
void Gem::removeGemAndBack(float speed, int tag, CCPoint source, CCPoint dest)
{
	this->stopAllActions();
	this->runAction(CCSequence::create(
		CCMoveTo::create(speed, dest),
		CCMoveTo::create(speed, source),
		NULL));
	this->setTag(tag);
}