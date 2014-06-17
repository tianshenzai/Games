#ifndef __GameScene_H__
#define __GameScene_H__

#include "cocos2d.h"

#include "GemManager.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

class GameScene : public cocos2d::CCLayer
{
private:
	GemManager* mygem;
	cocos2d::CCSize size;
	int number;  // 开始动画时间
	bool isover; // 结束标志
	bool isstart; // 开始标志
	bool eliminate; // 消除标记
	int choose;  // 选中的格子
	int choose2; // 第二次选中的格子
	string choosename[5]; // 选择音乐

public:
	GameScene();
	~GameScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuPauseCallback(cocos2d::CCObject* pSender);

	// 加载背景图片
	void loadBackground();

	// 加载头部
	void loadHeader();

	// 加载暂停按钮
	void loadButton();

	// 加载时间条
	void loadTimer();

	// 加载音乐和音效
	void loadMusic();

	// 加载开始画面
	void loadStart();

	// 更新开始动画
	void updateStart();

	// 更新结束动画
	void updateOver();

	// 显示初始分数
	void initScore();

	// 更新分数
	void updateScore();

	// 更新
	void update(float dt);

	// 触摸事件
	void ccTouchesBegan(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent);

    // implement the "static node()" method manually
    CREATE_FUNC(GameScene);
};

#endif  // __GameScene_H__