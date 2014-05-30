#ifndef __Gem_H__
#define __Gem_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

static const char* gemFile[5] = { "p0.png", "p1.png", "p2.png", "p3.png", "p4.png" };

class Gem : public cocos2d::CCLayer
{
private:
	int gameMatrix[72]; // 格子类型
	int xiaochu[72]; // 消除数组
	bool tianbu[72]; // 填补数组
	int score; // 总分数
	int number; // 一次的分数	
	bool isover; // 结束标志
	bool isstart; // 开始标志
	cocos2d::CCSpriteBatchNode* spriteSheet;
	cocos2d::CCSize size;

public:
	Gem();
	~Gem();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuPauseCallback(cocos2d::CCObject* pSender);

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
	void createAndDropGem(int row, int col);

	// 交换格子
	void swapGem(int& choose, int& choose2);

	// 爆炸效果
	void explodeSpecial(cocos2d::CCPoint point, bool flag);

	// 删除爆炸效果
	void disappear(cocos2d::CCNode* who);

	int getGameMatrix(int i);

	int getScore();

	void setScore(int newScore);

    // implement the "static node()" method manually
    CREATE_FUNC(Gem);
};

#endif  // __GameView_H__