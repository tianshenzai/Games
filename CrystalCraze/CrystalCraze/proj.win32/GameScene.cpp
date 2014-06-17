#include "GameScene.h"
#include <algorithm>
#include <cstdlib>

using namespace cocos2d;
using namespace CocosDenshion;

const int chooseid = 100;
const int ttnumberid = 101;
const int goid = 102;
const int timerid = 103;
const int ttfnumberid = 104;


CCScene* GameScene::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        GameScene *layer = GameScene::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

GameScene::GameScene()
{
	mygem = GemManager::create();
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

GameScene::~GameScene()
{
}

// on "init" you need to initialize your instance
bool GameScene::init()
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

		// 加载背景图片
		loadBackground();

		// 加载头部
		loadHeader();

		// 加载暂停按钮
		loadButton();

		// 加载时间条
		loadTimer();

		// 加载音乐和音效
		loadMusic();

		// 播放背景音乐
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("raw/loop.mp3", true);

		srand(time(NULL));

		// 加载开始画面
		loadStart();

		// 显示初始分数
		initScore();

//		mygem->init();
		this->addChild(mygem);

		// 更新格子
		this->scheduleUpdate();
		

        bRet = true;
    } while (0);

    return bRet;
}

// 加载背景图片
void GameScene::loadBackground()
{
	CCSprite* background = CCSprite::create("background.png", CCRect(0, 0, size.width, size.height));
	background->setPosition(CCPoint(size.width / 2, size.height / 2));
	this->addChild(background);
}

// 加载头部
void GameScene::loadHeader()
{
	CCSprite* header = CCSprite::create("header.png");
	header->setPosition(ccp(size.width / 2, size.height - 44));
	this->addChild(header);
}

// 加载暂停按钮
void GameScene::loadButton()
{
	CCMenuItemImage* pause = CCMenuItemImage::create(
		"pause.png",
		"pausedown.png",
		this,
		menu_selector(GameScene::menuPauseCallback));
	pause->setScale(0.8f);
	pause->setPosition(ccp(20, size.height - 20));
	CCMenu* pMenu = CCMenu::create(pause, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu);
}

// 加载时间条
void GameScene::loadTimer()
{
	CCSprite* timer = CCSprite::create("timer.png");
	timer->setPosition(ccp(size.width / 2, size.height - 70));
	this->addChild(timer, 1, timerid);
}

// 加载音乐和音效
void GameScene::loadMusic()
{
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("raw/loop.mp3");
	SimpleAudioEngine::sharedEngine()->preloadEffect("raw/timer.wav");
	SimpleAudioEngine::sharedEngine()->preloadEffect("raw/gem0.wav");
	SimpleAudioEngine::sharedEngine()->preloadEffect("raw/gem1.wav");
	SimpleAudioEngine::sharedEngine()->preloadEffect("raw/gem2.wav");
	SimpleAudioEngine::sharedEngine()->preloadEffect("raw/gem3.wav");
	SimpleAudioEngine::sharedEngine()->preloadEffect("raw/gem4.wav");
}

// 加载开始画面
void GameScene::loadStart()
{
	CCSprite* ttnumber = CCSprite::create("scorefont.png");
	ttnumber->setTextureRect(CCRect(43*3, 0, 43, 43));
	ttnumber->setPosition(ccp(size.width / 2, size.height / 2));
	ttnumber->setScale(4.0);
	this->addChild(ttnumber, 3, ttnumberid);
}

// 更新开始动画
void GameScene::updateStart()
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
		// 更新时间条
		CCMoveTo* moveto = CCMoveTo::create(30, ccp(15, size.height - 70));
		CCScaleTo* scaleto = CCScaleTo::create(30, 0, 1);
		CCSpawn* actions = CCSpawn::create(moveto, scaleto, NULL);
		temp->runAction(actions);
		// 播放时间音乐
		SimpleAudioEngine::sharedEngine()->playEffect("raw/timer.wav", true);
		number = 0;
		eliminate = true;
		isstart = false;
		return ;
	}
}

// 更新结束动画
void GameScene::updateOver()
{
	isover = true;
	eliminate = false;
	int temp = CCUserDefault::sharedUserDefault()->getIntegerForKey("score");
	if (mygem->getScore() > temp)
	{
		// 存储新高分
		CCUserDefault::sharedUserDefault()->setIntegerForKey("score", mygem->getScore());
	}

	static bool flag = true;
	if (flag)
	{
		// 停止所有音乐音效
		SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
		SimpleAudioEngine::sharedEngine()->stopAllEffects();
		// 播放结束音效
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

// 显示初始分数
void GameScene::initScore()
{
	CCLabelTTF* ttfnumber = CCLabelTTF::create("0", "Arial", 48);
	ttfnumber->setAnchorPoint(ccp(1, 1));
	ttfnumber->setPosition(ccp(size.width, size.height));
	this->addChild(ttfnumber, 3, ttfnumberid);
}

// 更新分数
void GameScene::updateScore()
{
	char stemp[9];
	itoa(mygem->getScore(), stemp, 10);
	CCLabelTTF* temp = (CCLabelTTF*)this->getChildByTag(ttfnumberid);
	temp->setString(stemp);
}

// 更新
void GameScene::update(float dt)
{
	// 开始动画
	if (isstart)
	{
		updateStart();
	}
	// 结束动画
	else if (this->getChildByTag(timerid)->numberOfRunningActions() < 1)
	{
		updateOver();
	}

	if (eliminate)
	{
		if (!mygem->doMatch())
			eliminate = false;
	}

	// 更新分数
	updateScore();
}

// 触摸事件
void GameScene::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent)
{
	if (!isstart && !isover)
	{
		CCTouch* touch = (CCTouch*)pTouches->anyObject();
		CCPoint touchLocation = touch->getLocation();
		int tempchoose = (int)(touchLocation.x) / 60 + (int)(touchLocation.y) / 60 * 8;
		if (tempchoose >= 72)
			return ;
		// 左右交换
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
		// 上下交换
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
		// 重新选择
		else
		{
			if (choose != 100)
				this->removeChildByTag(chooseid);
			choose = tempchoose;
			CCSprite* pchoose = CCSprite::create("hint.png");
			pchoose->setPosition(ccp(choose%8*60 + 30, choose/8*60 + 30));
			this->addChild(pchoose, 3, chooseid);
			// 播放选择音效
			int temp = mygem->getGameMatrix(choose);
			SimpleAudioEngine::sharedEngine()->playEffect(choosename[temp].c_str());
		}
	}
}

// 回调函数
void GameScene::menuPauseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();
}
