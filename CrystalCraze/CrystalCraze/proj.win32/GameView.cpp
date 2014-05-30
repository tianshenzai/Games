#include "GameView.h"
#include <algorithm>
#include <cstdlib>

using namespace cocos2d;
using namespace CocosDenshion;

const int chooseid = 100;
const int ttnumberid = 101;
const int goid = 102;
const int timerid = 103;
const int ttfnumberid = 104;


CCScene* GameView::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        GameView *layer = GameView::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

GameView::GameView()
{
	eliminate = false;
	choose = 100;
	choose2 = 100;
	score = 0; 
	number = 0; 
	isover = false;
	isstart = true;
	size = CCDirector::sharedDirector()->getWinSize();
	choosename[0] = "raw/gem0.wav";
	choosename[1] = "raw/gem1.wav";
	choosename[2] = "raw/gem2.wav";
	choosename[3] = "raw/gem3.wav";
	choosename[4] = "raw/gem4.wav";
}

GameView::~GameView()
{

}

// on "init" you need to initialize your instance
bool GameView::init()
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

		// ���ر���ͼƬ
		CCSprite* background = CCSprite::create("background.png", CCRect(0, 0, size.width, size.height));
		CC_BREAK_IF(! background);
		background->setPosition(CCPoint(size.width / 2, size.height / 2));
		this->addChild(background);

		// ���ر�ʯ
		CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
		cache->addSpriteFramesWithFile("p.plist");
		spriteSheet = CCSpriteBatchNode::create("p.pvr.ccz");
		this->addChild(spriteSheet);

		// ����ͷ��
		CCSprite* header = CCSprite::create("header.png");
		CC_BREAK_IF(! header);
		header->setPosition(ccp(size.width / 2, size.height - 44));
		this->addChild(header);

		// ������ͣ��ť
		CCMenuItemImage* pause = CCMenuItemImage::create(
			"pause.png",
			"pausedown.png",
			this,
			menu_selector(GameView::menuPauseCallback));
		CC_BREAK_IF(! pause);
		pause->setScale(0.8f);
		pause->setPosition(ccp(20, size.height - 20));
		CCMenu* pMenu = CCMenu::create(pause, NULL);
		pMenu->setPosition(CCPointZero);
		CC_BREAK_IF(! pMenu);
		this->addChild(pMenu);

		// ����ʱ����
		CCSprite* timer = CCSprite::create("timer.png");
		CC_BREAK_IF(! timer);
		timer->setPosition(ccp(size.width / 2, size.height - 70));
		this->addChild(timer, 1, timerid);

		// ���ű�������
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("raw/loop.mp3", true);

		srand(time(NULL));

		// ��ʼ����������
		srand(time(NULL));
		for (int i = 0; i < 72; i++)
		{
			gameMatrix[i] = rand() % 5;
		}

		// ���ؿ�ʼ����
		CCSprite* ttnumber = CCSprite::create("scorefont.png");
		ttnumber->setTextureRect(CCRect(43*3, 0, 43, 43));
		ttnumber->setPosition(ccp(size.width / 2, size.height / 2));
		ttnumber->setScale(4.0);
		this->addChild(ttnumber, 3, ttnumberid);

		// ��ʾ����
		CCLabelTTF* ttfnumber = CCLabelTTF::create("0", "Arial", 48);
		ttfnumber->setAnchorPoint(ccp(1, 1));
		ttfnumber->setPosition(ccp(size.width, size.height));
		this->addChild(ttfnumber, 3, ttfnumberid);

		// ��ʼ������
		initMatrix();

		// ���¸���
		this->scheduleUpdate();
		

        bRet = true;
    } while (0);

    return bRet;
}

// ����
void GameView::update(float dt)
{
	// ��ʼ����
	if (isstart)
	{
		if (number < 180)
		{
			if (number == 0)
				SimpleAudioEngine::sharedEngine()->playEffect("raw/tap0.wav");
			else if (number == 60)
				SimpleAudioEngine::sharedEngine()->playEffect("raw/tap1.wav");
			else if (number == 120)
				SimpleAudioEngine::sharedEngine()->playEffect("raw/tap2.wav");
			CCSprite* pnumber = (CCSprite*)this->getChildByTag(ttnumberid);
			pnumber->setTextureRect(CCRect(80*(1 - 1.0 / 180 * number), 0, 40, 40));
			number++;
		}
		else if (number == 180)
		{
			SimpleAudioEngine::sharedEngine()->playEffect("raw/tap3.wav");
			this->removeChildByTag(ttnumberid);
			CCSprite* go = CCSprite::create("go.png");
			go->setPosition(ccp(size.width / 2, size.height / 2));
			this->addChild(go, 3, goid);	
			number++;
		}
		else if (number < 200)
		{
			number++;
		}
		else
		{
			this->removeChildByTag(goid);
			CCSprite* temp = (CCSprite*)this->getChildByTag(timerid);
			// ����ʱ����
			CCMoveTo* moveto = CCMoveTo::create(30, ccp(15, size.height - 70));
			CCScaleTo* scaleto = CCScaleTo::create(30, 0, 1);
			CCSpawn* actions = CCSpawn::create(moveto, scaleto, NULL);
			temp->runAction(actions);
			// ����ʱ������
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic("raw/timer.wav", true);
			number = 0;
			doMatch();
			isstart = false;
			return ;
		}
	}
	// ��������
	else if (this->getChildByTag(timerid)->numberOfRunningActions() < 1)
	{
		isover = true;
		int temp = CCUserDefault::sharedUserDefault()->getIntegerForKey("score");
		if (score > temp)
		{
			// �洢�¸߷�
			CCUserDefault::sharedUserDefault()->setIntegerForKey("score", score);
		}

		static bool flag = true;
		if (flag)
		{
			// ֹͣ����������Ч
			SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
			SimpleAudioEngine::sharedEngine()->stopAllEffects();
			// ���Ž�����Ч
			SimpleAudioEngine::sharedEngine()->playEffect("raw/endgame.wav");
			CCLabelTTF* ttf = (CCLabelTTF*)this->getChildByTag(ttfnumberid);
			ttf->setAnchorPoint(ccp(0.5, 0.5));
			CCMoveTo* movettf = CCMoveTo::create(2, ccp(size.width / 2, size.height / 2));
			CCScaleTo* scalettf = CCScaleTo::create(2, 4, 4);
			CCSpawn* actionttf = CCSpawn::create(movettf, scalettf, NULL);
			ttf->runAction(actionttf);
			flag = false;
		}

	}
	// ����ƥ��
	else if (eliminate)
	{
		if (!doMatch())
		{
			eliminate = false;
		}
	}	

}

// ��ըЧ��
void GameView::explodeSpecial(CCPoint point, bool flag)
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
			CCCallFuncN::create(this, callfuncN_selector(GameView::disappear)),
			NULL));

		CCSprite* colorHLeft = CCSprite::create("colorHLeft.png");
		this->addChild(colorHLeft, 5);
		colorHLeft->setPosition(startPosition);
		colorHLeft->runAction(CCSequence::create(
			CCScaleTo::create(time, scaleX, scaleY),
			CCCallFuncN::create(this, callfuncN_selector(GameView::disappear)),
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
			CCCallFuncN::create(this, callfuncN_selector(GameView::disappear)),
			NULL));

		CCSprite* colorVDown = CCSprite::create("colorVDown.png");
		this->addChild(colorVDown, 5);
		colorVDown->setPosition(startPosition);
		colorVDown->runAction(CCSequence::create(
			CCScaleTo::create(time, scaleX, scaleY),
			CCCallFuncN::create(this, callfuncN_selector(GameView::disappear)),
			NULL));
	}
}

// ɾ����ըЧ��
void GameView::disappear(CCNode* who)
{
	who->removeFromParentAndCleanup(true);
}

// �����¼�
void GameView::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent)
{
	if (!isstart && !isover)
	{
		CCTouch* touch = (CCTouch*)pTouches->anyObject();
		CCPoint touchLocation = touch->getLocation();
		int tempchoose = (int)(touchLocation.x) / 60 + (int)(touchLocation.y) / 60 * 8;
		// ���ҽ���
		if (abs(tempchoose - choose) == 1 && tempchoose/8 == choose/8)
		{
			choose2 = tempchoose;
			if (choose2 > choose)
				swapGeZi(choose, choose2);
			else
				swapGeZi(choose2, choose);
		}
		// ���½���
		else if (abs(tempchoose/8 - choose/8) == 1 && tempchoose%8 == choose%8)
		{
			choose2 = tempchoose;
			if (choose2 > choose)
				swapGeZi(choose, choose2);
			else
				swapGeZi(choose2, choose);
		}
		// ����ѡ��
		else
		{
			if (choose != 100)
				this->removeChildByTag(chooseid);
			choose = tempchoose;
			CCSprite* pchoose = CCSprite::create("hint.png");
			pchoose->setPosition(ccp(choose%8*60 + 30, choose/8*60 + 30));
			this->addChild(pchoose, 3, chooseid);
			// ����ѡ����Ч
			int temp = gameMatrix[choose];
			SimpleAudioEngine::sharedEngine()->playEffect(choosename[temp].c_str());
		}
	}
}

// ��������
void GameView::swapGeZi(int& choose, int& choose2)
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
	if (choose != 100)
		this->removeChildByTag(chooseid);
	choose  = 100;
	choose2 = 100;
}

// ��ʼ������
void GameView::initMatrix()
{
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			createAndDropGeZi(row, col);
		}
	}
}

// �����¸���
void GameView::createAndDropGeZi(int row, int col)
{
	CCSprite* gezi = CCSprite::create();
	gezi->initWithSpriteFrameName(geziFile[gameMatrix[row*8 + col]]);
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
void GameView::menuPauseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();
}

// �ж��Ƿ��и�������
bool GameView::doMatch()
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
void GameView::clearArray()
{
	for (int i = 0; i < 72; i++)
	{
		xiaochu[i] = 1000;
		tianbu[i] = false;
	}
}
	
// ���л�����
void GameView::serialArray()
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
	char stemp[9];
	score += number * 2;
	itoa(score, stemp, 10);
	CCLabelTTF* temp = (CCLabelTTF*)this->getChildByTag(ttfnumberid);
	temp->setString(stemp);
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
void GameView::doRemove()
{
	// �������
	eliminate = true;
	
	// ����������Ч
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("raw/powerup.wav");

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
				createAndDropGeZi(row, col);
				tianbu[row*8 +col] = false;
			}
		}
	}
}

