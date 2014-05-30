#ifndef __GameView_H__
#define __GameView_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

static const char* geziFile[5] = { "p0.png", "p1.png", "p2.png", "p3.png", "p4.png" };

class GameView : public cocos2d::CCLayer
{
private:
	int gameMatrix[72]; // 格子类型
	bool eliminate; // 消除标记
	int choose;  // 选中的格子
	int choose2; // 第二次选中的格子
	int xiaochu[72]; // 消除数组
	bool tianbu[72]; // 填补数组
	int score; // 总分数
	int number; // 一次的分数	
	bool isover; // 结束标志
	bool isstart; // 开始标志
	cocos2d::CCSpriteBatchNode* spriteSheet;
	cocos2d::CCSize size;
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

	// 判断是否有格子相连
	bool doMatch();

	// 清空xiaochu数组和tianbu数组
	void clearArray();

	// 序列化数组
	void serialArray();

	// 下移格子
	void doRemove();

	// 初始化格子
	void initMatrix();

	// 产生新格子
	void createAndDropGeZi(int row, int col);

	// 触摸事件
	void ccTouchesBegan(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent);

	// 交换格子
	void swapGeZi(int& choose, int& choose2);

	// 爆炸效果
	void explodeSpecial(cocos2d::CCPoint point, bool flag);

	// 删除爆炸效果
	void disappear(cocos2d::CCNode* who);

    // implement the "static node()" method manually
    CREATE_FUNC(GameView);
};

#endif  // __GameView_H__