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
	mygem = Gem::create();
	size = CCDirector::sharedDirector()->getWinSize();
	number = 0; 
	isover = false;
	isstart = true;
	eliminate = false;
	choose = 100;
	choose2 = 100;

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
		
		// �������ֺ���Ч
		SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("raw/loop.mp3");
		SimpleAudioEngine::sharedEngine()->preloadEffect("raw/timer.wav");
		SimpleAudioEngine::sharedEngine()->preloadEffect("raw/gem0.wav");
		SimpleAudioEngine::sharedEngine()->preloadEffect("raw/gem1.wav");
		SimpleAudioEngine::sharedEngine()->preloadEffect("raw/gem2.wav");
		SimpleAudioEngine::sharedEngine()->preloadEffect("raw/gem3.wav");
		SimpleAudioEngine::sharedEngine()->preloadEffect("raw/gem4.wav");

		// ���ű�������
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("raw/loop.mp3", true);
	
		srand(time(NULL));

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

//		mygem->init();
		this->addChild(mygem);

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
			SimpleAudioEngine::sharedEngine()->playEffect("raw/timer.wav", true);
			number = 0;
			eliminate = true;
			isstart = false;
			return ;
		}
	}
	// ��������
	else if (this->getChildByTag(timerid)->numberOfRunningActions() < 1)
	{
		isover = true;
		eliminate = false;
		int temp = CCUserDefault::sharedUserDefault()->getIntegerForKey("score");
		if (mygem->getScore() > temp)
		{
			// �洢�¸߷�
			CCUserDefault::sharedUserDefault()->setIntegerForKey("score", mygem->getScore());
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

	if (eliminate)
	{
		if (!mygem->doMatch())
			eliminate = false;
	}

	char stemp[9];
	itoa(mygem->getScore(), stemp, 10);
	CCLabelTTF* temp = (CCLabelTTF*)this->getChildByTag(ttfnumberid);
	temp->setString(stemp);
}

// �����¼�
void GameView::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent)
{
	if (!isstart && !isover)
	{
		CCTouch* touch = (CCTouch*)pTouches->anyObject();
		CCPoint touchLocation = touch->getLocation();
		int tempchoose = (int)(touchLocation.x) / 60 + (int)(touchLocation.y) / 60 * 8;
		if (tempchoose >= 72)
			return ;
		// ���ҽ���
		if (abs(tempchoose - choose) == 1 && tempchoose/8 == choose/8)
		{
			choose2 = tempchoose;
			if (choose2 > choose)
				mygem->swapGem(choose, choose2);
			else
				mygem->swapGem(choose2, choose);
			
			this->removeChildByTag(chooseid);
			eliminate = true;
		}
		// ���½���
		else if (abs(tempchoose/8 - choose/8) == 1 && tempchoose%8 == choose%8)
		{
			choose2 = tempchoose;
			if (choose2 > choose)
				mygem->swapGem(choose, choose2);
			else
				mygem->swapGem(choose2, choose);
			
			this->removeChildByTag(chooseid);
			eliminate = true;
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
			int temp = mygem->getGameMatrix(choose);
			SimpleAudioEngine::sharedEngine()->playEffect(choosename[temp].c_str());
		}
	}
}

// �ص�����
void GameView::menuPauseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();
}
