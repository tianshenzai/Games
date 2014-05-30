#ifndef __GameView_H__
#define __GameView_H__

#include "cocos2d.h"

#include "Gem.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

class GameView : public cocos2d::CCLayer
{
private:
	Gem* mygem;
	cocos2d::CCSize size;
	int number;  // 开始动画时间
	bool isover; // 结束标志
	bool isstart; // 开始标志
	bool eliminate; // 消除标记
	int choose;  // 选中的格子
	int choose2; // 第二次选中的格子
	string choosename[5]; // 选择音乐

public:
	GameView();
	~GameView();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuPauseCallback(cocos2d::CCObject* pSender);

	// 更新
	void update(float dt);

	// 触摸事件
	void ccTouchesBegan(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent);

    // implement the "static node()" method manually
    CREATE_FUNC(GameView);
};

#endif  // __GameView_H__