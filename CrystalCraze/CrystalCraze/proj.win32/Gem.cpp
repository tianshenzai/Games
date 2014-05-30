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

		// 加载宝石
		CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
		cache->addSpriteFramesWithFile("p.plist");
		spriteSheet = CCSpriteBatchNode::create("p.pvr.ccz");
		this->addChild(spriteSheet);

		srand(time(NULL));

		// 初始化格子类型
		srand(time(NULL));
		for (int i = 0; i < 72; i++)
		{
			gameMatrix[i] = rand() % 5;
		}

		// 初始化格子
		initMatrix();	

		// 加载音效
		SimpleAudioEngine::sharedEngine()->preloadEffect("raw/powerup.wav");
		SimpleAudioEngine::sharedEngine()->preloadEffect("raw/miss.wav");

        bRet = true;
    } while (0);

    return bRet;
}

// 爆炸效果
void Gem::explodeSpecial(CCPoint point, bool flag)
{
	float scaleX = 1.0f;
	float scaleY = 0.7f;
	float time = 0.3f;
	CCPoint startPosition = point;
	float speed = 0.6f;

	// 横向爆炸
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
	// 竖向爆炸
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

// 删除爆炸效果
void Gem::disappear(CCNode* who)
{
	who->removeFromParentAndCleanup(true);
}

// 交换格子
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
	// 匹配不成功，返回
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

		// 播放返回音效
		SimpleAudioEngine::sharedEngine()->playEffect("raw/miss.wav");
	}
	choose  = 100;
	choose2 = 100;
}

// 初始化格子
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

// 产生新格子
void Gem::createAndDropGem(int row, int col)
{
	CCSprite* gezi = CCSprite::create();
	gezi->initWithSpriteFrameName(gemFile[gameMatrix[row*8 + col]]);
	gezi->setScale(0.75);

	// 下落动画
	CCPoint endPosition = CCPoint(col * 60 + 30, row * 60 + 30);
	CCPoint startPosition = CCPoint(endPosition.x, endPosition.y + 480);
	gezi->setPosition(startPosition);
	float speed = (startPosition.y - endPosition.y) / (1.5 * size.height);
	gezi->runAction(CCMoveTo::create(speed, endPosition));

	spriteSheet->addChild(gezi, 2, row*8 + col);
}

// 回调函数
void Gem::menuPauseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();
}

// 判断是否有格子相连
bool Gem::doMatch()
{
	if (xiaochu[0] != 1000)
		clearArray();
	int count = 0;
	int arrayPoint = 0;
	number = 0;
	// 横查询
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
		
	// 竖查询
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
		
	serialArray(); // 序列化数组
	if (xiaochu[0] == 1000)
		return false;
	else
	{
		doRemove();
		return true;
	}
}
	
// 清空xiaochu数组和tianbu数组
void Gem::clearArray()
{
	for (int i = 0; i < 72; i++)
	{
		xiaochu[i] = 1000;
		tianbu[i] = false;
	}
}
	
// 序列化数组
void Gem::serialArray()
{
	sort(xiaochu, xiaochu+72);
	for (int i = 0; i < 71; i++)
	{
		if (xiaochu[i]==xiaochu[i+1]&&xiaochu[i]!=1000) // 去除重复的格子
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
	
// 下移格子
void Gem::doRemove()
{
	// 播放消除音效
	SimpleAudioEngine::sharedEngine()->playEffect("raw/powerup.wav");

	// 上方有可下落格子
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

				// 下落动画
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
	// 补充新格子
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
