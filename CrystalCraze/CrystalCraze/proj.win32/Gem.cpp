#include "Gem.h"
#include <algorithm>
#include <cstdlib>

using namespace cocos2d;
using namespace CocosDenshion;

Gem::Gem()
{
	score = 0; 
	number = 0; 
	isover = false;
	isstart = true;
	size = CCDirector::sharedDirector()->getWinSize();
}

Gem::~Gem()
{
}

// on "init" you need to initialize your instance
bool Gem::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////	
	
		this->setTouchEnabled(true);

		// ���ر�ʯ
		CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
		cache->addSpriteFramesWithFile("p.plist");
		spriteSheet = CCSpriteBatchNode::create("p.pvr.ccz");
		this->addChild(spriteSheet);

		srand(time(NULL));

		// ��ʼ����������
		srand(time(NULL));
		for (int i = 0; i < 72; i++)
		{
			gameMatrix[i] = rand() % 5;
		}

		// ��ʼ������
		initMatrix();	

		// ������Ч
		SimpleAudioEngine::sharedEngine()->preloadEffect("raw/powerup.wav");
		SimpleAudioEngine::sharedEngine()->preloadEffect("raw/miss.wav");

        bRet = true;
    } while (0);

    return bRet;
}

// ��ըЧ��
void Gem::explodeSpecial(CCPoint point, bool flag)
{
	float scaleX = 1.0f;
	float scaleY = 0.7f;
	float time = 0.3f;
	CCPoint startPosition = point;
	float speed = 0.6f;

	// ����ը
	if (flag)
	{
		CCSprite* colorHRight = CCSprite::create("colorHRight.png");
		this->addChild(colorHRight, 5);
		colorHRight->setPosition(startPosition);
		colorHRight->runAction(CCSequence::create(
			CCScaleTo::create(time, scaleX, scaleY),
			CCCallFuncN::create(this, callfuncN_selector(Gem::disappear)),
			NULL));

		CCSprite* colorHLeft = CCSprite::create("colorHLeft.png");
		this->addChild(colorHLeft, 5);
		colorHLeft->setPosition(startPosition);
		colorHLeft->runAction(CCSequence::create(
			CCScaleTo::create(time, scaleX, scaleY),
			CCCallFuncN::create(this, callfuncN_selector(Gem::disappear)),
			NULL));
	}
	// ����ը
	else
	{
		CCSprite* colorVUp = CCSprite::create("colorVUp.png");
		this->addChild(colorVUp, 5);
		colorVUp->setPosition(startPosition);
		colorVUp->runAction(CCSequence::create(
			CCScaleTo::create(time, scaleX, scaleY),
			CCCallFuncN::create(this, callfuncN_selector(Gem::disappear)),
			NULL));

		CCSprite* colorVDown = CCSprite::create("colorVDown.png");
		this->addChild(colorVDown, 5);
		colorVDown->setPosition(startPosition);
		colorVDown->runAction(CCSequence::create(
			CCScaleTo::create(time, scaleX, scaleY),
			CCCallFuncN::create(this, callfuncN_selector(Gem::disappear)),
			NULL));
	}
}

// ɾ����ըЧ��
void Gem::disappear(CCNode* who)
{
	who->removeFromParentAndCleanup(true);
}

// ��������
void Gem::swapGem(int& choose, int& choose2)
{
	CCSprite* sp1 = (CCSprite*)spriteSheet->getChildByTag(choose);
	CCSprite* sp2 = (CCSprite*)spriteSheet->getChildByTag(choose2);
	CCPoint p1 = sp1->getPosition();
	CCPoint p2 = sp2->getPosition();

	int tempx;
	tempx = gameMatrix[choose];
	gameMatrix[choose] = gameMatrix[choose2];
	gameMatrix[choose2] = tempx;
	float speed = 0.25;
	sp1->runAction(CCMoveTo::create(speed, p2));
	sp1->setTag(choose2);
	sp2->runAction(CCMoveTo::create(speed, p1));
	sp2->setTag(choose);
	// ƥ�䲻�ɹ�������
	if (!doMatch())
	{
		tempx = gameMatrix[choose];
		gameMatrix[choose] = gameMatrix[choose2];
		gameMatrix[choose2] = tempx;
		sp1->stopAllActions();
		sp1->runAction(CCSequence::create(
			CCMoveTo::create(speed, p2),
			CCMoveTo::create(speed, p1),
			NULL));
		sp1->setTag(choose);
		sp2->stopAllActions();
		sp2->runAction(CCSequence::create(
			CCMoveTo::create(speed, p1),
			CCMoveTo::create(speed, p2),
			NULL));
		sp2->setTag(choose2);

		// ���ŷ�����Ч
		SimpleAudioEngine::sharedEngine()->playEffect("raw/miss.wav");
	}
	choose  = 100;
	choose2 = 100;
}

// ��ʼ������
void Gem::initMatrix()
{
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			createAndDropGem(row, col);
		}
	}
}

// �����¸���
void Gem::createAndDropGem(int row, int col)
{
	CCSprite* gezi = CCSprite::create();
	gezi->initWithSpriteFrameName(gemFile[gameMatrix[row*8 + col]]);
	gezi->setScale(0.75);

	// ���䶯��
	CCPoint endPosition = CCPoint(col * 60 + 30, row * 60 + 30);
	CCPoint startPosition = CCPoint(endPosition.x, endPosition.y + 480);
	gezi->setPosition(startPosition);
	float speed = (startPosition.y - endPosition.y) / (1.5 * size.height);
	gezi->runAction(CCMoveTo::create(speed, endPosition));

	spriteSheet->addChild(gezi, 2, row*8 + col);
}

// �ص�����
void Gem::menuPauseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();
}

// �ж��Ƿ��и�������
bool Gem::doMatch()
{
	if (xiaochu[0] != 1000)
		clearArray();
	int count = 0;
	int arrayPoint = 0;
	number = 0;
	// ���ѯ
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 6;)
		{
			while (col+1 < 8 && gameMatrix[row*8+col] == gameMatrix[row*8+col+1])
			{
				col++;
				count++;
			}
			if (count >= 2)
			{
				for (; count >= 0; count--)
				{
					if (isover)
						return false;
					xiaochu[arrayPoint] = (row * 8 + col - count) * 2 + 1;
					arrayPoint++;
					number++;
				}
			}
			count = 0;
			col++;
		}
	}
		
	// ����ѯ
	for (int col = 0; col < 8; col++)
	{
		for (int row = 0; row < 7; )
		{
			while(row + 1 < 9 && gameMatrix[row * 8 + col] == gameMatrix[(row+1) * 8 + col])
			{
				row++;
				count++;
			}
			if (count >= 2)
			{
				for (; count >= 0; count--)
				{
					if (isover)
						return false;
					xiaochu[arrayPoint] = ((row-count) * 8 + col) * 2;
					arrayPoint++;						
					number++;
				}
			}
			count = 0;
			row++;
		}
	}
		
	serialArray(); // ���л�����
	if (xiaochu[0] == 1000)
		return false;
	else
	{
		doRemove();
		return true;
	}
}
	
// ���xiaochu�����tianbu����
void Gem::clearArray()
{
	for (int i = 0; i < 72; i++)
	{
		xiaochu[i] = 1000;
		tianbu[i] = false;
	}
}
	
// ���л�����
void Gem::serialArray()
{
	sort(xiaochu, xiaochu+72);
	for (int i = 0; i < 71; i++)
	{
		if (xiaochu[i]==xiaochu[i+1]&&xiaochu[i]!=1000) // ȥ���ظ��ĸ���
		{
			xiaochu[i] = 1000;
			number--;
		}
	}
	score += number * 2;
	sort(xiaochu, xiaochu+72);
	for (int i = 0; xiaochu[i]!=1000; i++)
	{
		tianbu[xiaochu[i] / 2] = true;
		gameMatrix[xiaochu[i] / 2] = 100;
		CCPoint temp = ccp(xiaochu[i]/2%8 * 60 + 30, xiaochu[i]/2/8 * 60 + 30);
		if (xiaochu[i] % 2 == 0)
		{
			bool flag = false;
			explodeSpecial(temp, flag);
		}
		else
		{
			bool flag = true;
			explodeSpecial(temp, flag);
		}
		spriteSheet->removeChildByTag(xiaochu[i]/2);
	}
}
	
// ���Ƹ���
void Gem::doRemove()
{
	// ����������Ч
	SimpleAudioEngine::sharedEngine()->playEffect("raw/powerup.wav");

	// �Ϸ��п��������
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			int i = row*8 + col;
			if (tianbu[i])
			{
				int j = i + 8;
				while (tianbu[j] && j < 72)
				{
					j += 8;
				}
				if (j >= 72)
					continue;
				int tempx;
				bool tempt;
				tempx = gameMatrix[i];
				gameMatrix[i] = gameMatrix[j];
				gameMatrix[j] = tempx;
				tempt = tianbu[i];
				tianbu[i] = tianbu[j];
				tianbu[j] = tempt;

				// ���䶯��
				CCSize size = CCDirector::sharedDirector()->getWinSize();
				CCSprite* gezi = (CCSprite*)spriteSheet->getChildByTag(j);
				CCPoint endPosition = CCPoint(col * 60 + 30, row * 60 + 30);
				CCPoint startPosition = gezi->getPosition();
				gezi->setPosition(startPosition);
				float speed = (startPosition.y - endPosition.y) / (1.5 * size.height);
				gezi->stopAllActions();
				gezi->runAction(CCMoveTo::create(speed, endPosition));
				gezi->setTag(i);
			}
		}
	}
	// �����¸���
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			if (tianbu[row*8 +col])
			{
				gameMatrix[row*8 + col] = rand() % 5;
				createAndDropGem(row, col);
				tianbu[row*8 +col] = false;
			}
		}
	}
}

int Gem::getGameMatrix(int i)
{
	return gameMatrix[i];
}

int Gem::getScore()
{
	return score;
}

void Gem::setScore(int newScore)
{
	score = newScore;
}
