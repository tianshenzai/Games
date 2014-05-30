#ifndef __Gem_H__
#define __Gem_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

static const char* gemFile[5] = { "p0.png", "p1.png", "p2.png", "p3.png", "p4.png" };

class Gem : public cocos2d::CCLayer
{
private:
	int gameMatrix[72]; // ��������
	int xiaochu[72]; // ��������
	bool tianbu[72]; // �����
	int score; // �ܷ���
	int number; // һ�εķ���	
	bool isover; // ������־
	bool isstart; // ��ʼ��־
	cocos2d::CCSpriteBatchNode* spriteSheet;
	cocos2d::CCSize size;

public:
	Gem();
	~Gem();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuPauseCallback(cocos2d::CCObject* pSender);

	// �ж��Ƿ��и�������
	bool doMatch();

	// ���xiaochu�����tianbu����
	void clearArray();

	// ���л�����
	void serialArray();

	// ���Ƹ���
	void doRemove();

	// ��ʼ������
	void initMatrix();

	// �����¸���
	void createAndDropGem(int row, int col);

	// ��������
	void swapGem(int& choose, int& choose2);

	// ��ըЧ��
	void explodeSpecial(cocos2d::CCPoint point, bool flag);

	// ɾ����ըЧ��
	void disappear(cocos2d::CCNode* who);

	int getGameMatrix(int i);

	int getScore();

	void setScore(int newScore);

    // implement the "static node()" method manually
    CREATE_FUNC(Gem);
};

#endif  // __GameView_H__