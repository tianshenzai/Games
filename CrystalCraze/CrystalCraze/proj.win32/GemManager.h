#ifndef __GemManager_H__
#define __GemManager_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

static const char* gemFile[5] = { "p0.png", "p1.png", "p2.png", "p3.png", "p4.png" };

class GemManager : public cocos2d::CCLayer
{
private:
	int gameMatrix[72]; // ��ʯ����ɫ
	int xiaochu[72]; // ��������
	bool tianbu[72]; // �����
	int score; // �ܷ���
	int number; // һ�εķ���	
	bool isover; // ������־
	bool isstart; // ��ʼ��־
	cocos2d::CCSpriteBatchNode* spriteSheet;
	cocos2d::CCSize size;

public:
	GemManager();
	~GemManager();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuPauseCallback(cocos2d::CCObject* pSender);

	// �ж��Ƿ��б�ʯ����
	bool doMatch();

	// ���xiaochu�����tianbu����
	void clearArray();

	// ���л�����
	void serialArray();

	// ���Ʊ�ʯ
	void doRemove();

	// ��ʼ����ʯ����
	void initMatrix();

	// �����±�ʯ
	void createAndDropGem(int row, int col);

	// ������ʯ
	void swapGem(int& choose, int& choose2);

	// ɾ����ʯ
	void deleteGem(int tag);

	// �õ��±�i�ı�ʯ
	int getGameMatrix(int i);

	// �õ�����
	int getScore();

	// ���÷���
	void setScore(int newScore);

    // implement the "static node()" method manually
    CREATE_FUNC(GemManager);
};

#endif  // __GemManager_H__