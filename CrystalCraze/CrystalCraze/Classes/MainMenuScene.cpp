#include "MainMenuScene.h"
#include "GameScene.h"
#include <string>
#include <cmath>
#include <cstdlib>

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* MainMenuScene::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        MainMenuScene *layer = MainMenuScene::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

MainMenuScene::MainMenuScene()
{
	for (int i = 0; i < 5; i++)
	{
		star[i] = NULL;
	}

	starname[0] = "p0.png";
	starname[1] = "p1.png";
	starname[2] = "p2.png";
	starname[3] = "p3.png";
	starname[4] = "p4.png";
}

MainMenuScene::~MainMenuScene()
{
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
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

		// 获取屏幕尺寸
		size = CCDirector::sharedDirector()->getWinSize();

		// 加载背景图片
		loadBackground();

		// 加载文字
		loadText();

		// 加载开始和关于按钮
		loadButton();

		// 显示分数
		showNumber();

		// 播放背景音乐
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic("raw/loop.mp3", true);

		srand(time(NULL));
		this->scheduleUpdate();

        bRet = true;
    } while (0);

    return bRet;
}

// 加载背景图片
void MainMenuScene::loadBackground()
{
	CCSprite* background = CCSprite::create("background.png", CCRect(0, 0, size.width, size.height));
	background->setPosition(CCPoint(size.width / 2, size.height / 2));
	this->addChild(background);
}

// 加载文字
void MainMenuScene::loadText()
{
	CCSprite* logo = CCSprite::create("logo.png");
	logo->setPosition(CCPoint(size.width / 2, size.height * 4 / 5));
	this->addChild(logo, 1);
}

// 加载开始和关于按钮
void MainMenuScene::loadButton()
{
	CCMenuItemImage *pplay = CCMenuItemImage::create(
		"play.png",
		"playdown.png",
		this,
		menu_selector(MainMenuScene::menuPlayCallback));
	pplay->setPosition(CCPoint(size.width / 2, size.height / 2 - 20));

	CCMenuItemImage *pabout = CCMenuItemImage::create(
		"about.png",
		"aboutdown.png",
		this,
		menu_selector(MainMenuScene::menuAboutCallback));
	pabout->setPosition(CCPoint(size.width / 2, size.height / 4 - 20));

	CCMenu* pMenu = CCMenu::create(pplay, pabout, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu,1);
}

// 显示分数
void MainMenuScene::showNumber()
{
	int result = CCUserDefault::sharedUserDefault()->getIntegerForKey("score");
	char temp[9];
	itoa(result, temp, 10);
	CCLabelTTF* ttfnumber = CCLabelTTF::create(temp, "Arial", 80);
	ttfnumber->setPosition(ccp(size.width / 2, size.height / 15));
	this->addChild(ttfnumber);
}

// 更新星星位置
void MainMenuScene::update(float dt)
{
	updateGem();
}

// 更新星星位置
void MainMenuScene::updateGem()
{
	for (int i = 0; i < 5; i++)
	{
		if (star[i] == NULL)
		{
			int number = rand() % 5;
			star[i] = CCSprite::create(starname[i].c_str());
			star[i]->setTag(i);
			this->addChild(star[i]);
			star[i]->setPositionX(0);
			star[i]->setPositionY(rand() % ((int)size.height * 2 / 3));
			int time = rand() % 3 + 2;
			CCMoveTo* moveto = CCMoveTo::create(time, ccp(size.width + 100, size.height + 100));
			star[i]->runAction(moveto); 		
		}
		if (star[i]->getPositionX() >= size.width && star[i]->getPositionY() >= size.height)
		{
			star[i]->stopAllActions();
			this->removeChildByTag(star[i]->getTag());
			star[i] = NULL;
		}
	}
}

void MainMenuScene::menuPlayCallback(CCObject* pSender)
{
	// 播放点击音乐
	SimpleAudioEngine::sharedEngine()->playEffect("raw/click.wav");

    // 转换到游戏界面
    CCScene* gameView = GameScene::scene();
	CCDirector::sharedDirector()->replaceScene(gameView);
}

void MainMenuScene::menuAboutCallback(CCObject* pSender)
{
	// 播放点击音乐
	SimpleAudioEngine::sharedEngine()->playEffect("raw/click.wav");
}
