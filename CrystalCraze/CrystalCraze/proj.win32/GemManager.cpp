#include "GemManager.h"
#include "EffectManager.h"
#include "Gem.h"
#include <algorithm>
#include <cstdlib>

using namespace cocos2d;
using namespace CocosDenshion;

GemManager::GemManager()
{
	score = 0; 
	number = 0; 
	isover = false;
	isstart = true;
	size = CCDirector::sharedDirector()->getWinSize();
}

GemManager::~GemManager()
{
}

// on "init" you need to initialize your instance
bool GemManager::init()
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

		// 加载宝石列表
		CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
		cache->addSpriteFramesWithFile("p.plist");
		spriteSheet = CCSpriteBatchNode::create("p.pvr.ccz");
		this->addChild(spriteSheet);

		srand(time(NULL));

		// 初始化宝石颜色
		srand(time(NULL));
		for (int i = 0; i < 72; i++)
		{
			gameMatrix[i] = rand() % 5;
		}

		// 初始化宝石
		initMatrix();	

		// 加载音效
		SimpleAudioEngine::sharedEngine()->preloadEffect("raw/powerup.wav");
		SimpleAudioEngine::sharedEngine()->preloadEffect("raw/miss.wav");

        bRet = true;
    } while (0);

    return bRet;
}

// 交换宝石
void GemManager::swapGem(int& choose, int& choose2)
{
	Gem* sp1 = (Gem*)spriteSheet->getChildByTag(choose);
	Gem* sp2 = (Gem*)spriteSheet->getChildByTag(choose2);
	CCPoint p1 = sp1->getPosition();
	CCPoint p2 = sp2->getPosition();

	int tempx;
	tempx = gameMatrix[choose];
	gameMatrix[choose] = gameMatrix[choose2];
	gameMatrix[choose2] = tempx;
	float speed = 0.25;
	sp1->removeGem(speed, choose2, p2);
	sp2->removeGem(speed, choose, p1);
	// 匹配不成功，返回
	if (!doMatch())
	{
		tempx = gameMatrix[choose];
		gameMatrix[choose] = gameMatrix[choose2];
		gameMatrix[choose2] = tempx;

		sp1->removeGemAndBack(speed, choose,  p1, p2);
		sp2->removeGemAndBack(speed, choose2, p2, p1);

		// 播放返回音效
		SimpleAudioEngine::sharedEngine()->playEffect("raw/miss.wav");
	}
	choose  = 100;
	choose2 = 100;
}

// 初始化宝石数组
void GemManager::initMatrix()
{
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			createAndDropGem(row, col);
		}
	}
}

// 产生新宝石
void GemManager::createAndDropGem(int row, int col)
{
	Gem* gem = Gem::create();
	gem->initWithSpriteFrameName(gemFile[gameMatrix[row*8 + col]]);
	gem->setScale(0.75);
	gem->setPositionX(col * 60 + 30);
	gem->setPositionY(row * 60 + 30);

	// 下落动画
	CCPoint endPosition = CCPoint(gem->getPositionX(), gem->getPositionY());
	CCPoint startPosition = CCPoint(endPosition.x, endPosition.y + 480);
	gem->setPosition(startPosition);
	float speed = (startPosition.y - endPosition.y) / (1.5 * size.height);
	gem->runAction(CCMoveTo::create(speed, endPosition));

	spriteSheet->addChild(gem, 2, row*8 + col);
}

// 回调函数
void GemManager::menuPauseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();
}

// 判断是否有宝石相连
bool GemManager::doMatch()
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
void GemManager::clearArray()
{
	for (int i = 0; i < 72; i++)
	{
		xiaochu[i] = 1000;
		tianbu[i] = false;
	}
}
	
// 序列化数组
void GemManager::serialArray()
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
	EffectManager* em = EffectManager::create();
	for (int i = 0; xiaochu[i]!=1000; i++)
	{
		tianbu[xiaochu[i] / 2] = true;
		gameMatrix[xiaochu[i] / 2] = 100;
		CCPoint temp = ccp(xiaochu[i]/2%8 * 60 + 30, xiaochu[i]/2/8 * 60 + 30);
		if (xiaochu[i] % 2 == 0)
		{
			bool flag = false;
			em->explodeSpecial(temp, flag);
		}
		else
		{
			bool flag = true;
			em->explodeSpecial(temp, flag);
		}
		deleteGem(xiaochu[i]/2);
	}
	this->addChild(em, 5);
}

// 删除宝石
void GemManager::deleteGem(int tag)
{
	spriteSheet->removeChildByTag(tag);
}
	
// 下移宝石
void GemManager::doRemove()
{
	// 播放消除音效
	SimpleAudioEngine::sharedEngine()->playEffect("raw/powerup.wav");

	// 上方有可下落宝石
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
	// 补充新宝石
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

// 得到下标i的宝石
int GemManager::getGameMatrix(int i)
{
	return gameMatrix[i];
}

// 得到分数
int GemManager::getScore()
{
	return score;
}

// 设置分数
void GemManager::setScore(int newScore)
{
	score = newScore;
}
