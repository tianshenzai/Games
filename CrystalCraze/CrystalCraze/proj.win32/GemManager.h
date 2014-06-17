#ifndef __GemManager_H__
#define __GemManager_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

static const char* gemFile[5] = { "p0.png", "p1.png", "p2.png", "p3.png", "p4.png" };

class GemManager : public cocos2d::CCLayer
{
private:
	int gameMatrix[72]; // 宝石的颜色
	int xiaochu[72]; // 消除数组
	bool tianbu[72]; // 填补数组
	int score; // 总分数
	int number; // 一次的分数	
	bool isover; // 结束标志
	bool isstart; // 开始标志
	cocos2d::CCSpriteBatchNode* spriteSheet;
	cocos2d::CCSize size;

public:
	GemManager();
	~GemManager();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuPauseCallback(cocos2d::CCObject* pSender);

	// 判断是否有宝石相连
	bool doMatch();

	// 清空xiaochu数组和tianbu数组
	void clearArray();

	// 序列化数组
	void serialArray();

	// 下移宝石
	void doRemove();

	// 初始化宝石数组
	void initMatrix();

	// 产生新宝石
	void createAndDropGem(int row, int col);

	// 交换宝石
	void swapGem(int& choose, int& choose2);

	// 删除宝石
	void deleteGem(int tag);

	// 得到下标i的宝石
	int getGameMatrix(int i);

	// 得到分数
	int getScore();

	// 设置分数
	void setScore(int newScore);

    // implement the "static node()" method manually
    CREATE_FUNC(GemManager);
};

#endif  // __GemManager_H__