#ifndef __GameView_H__
#define __GameView_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

static const char* geziFile[5] = { "p0.png", "p1.png", "p2.png", "p3.png", "p4.png" };

class GameView : public cocos2d::CCLayer
{
private:
	int gameMatrix[72]; // ��������
	bool eliminate; // �������
	int choose;  // ѡ�еĸ���
	int choose2; // �ڶ���ѡ�еĸ���
	int xiaochu[72]; // ��������
	bool tianbu[72]; // �����
	int score; // �ܷ���
	int number; // һ�εķ���	
	bool isover; // ������־
	bool isstart; // ��ʼ��־
	cocos2d::CCSpriteBatchNode* spriteSheet;
	cocos2d::CCSize size;
	string choosename[5]; // ѡ������

public:
	GameView();
	~GameView();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuPauseCallback(cocos2d::CCObject* pSender);

	// ����
	void update(float dt);

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
	void createAndDropGeZi(int row, int col);

	// �����¼�
	void ccTouchesBegan(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent);

	// ��������
	void swapGeZi(int& choose, int& choose2);

	// ��ըЧ��
	void explodeSpecial(cocos2d::CCPoint point, bool flag);

	// ɾ����ըЧ��
	void disappear(cocos2d::CCNode* who);

    // implement the "static node()" method manually
    CREATE_FUNC(GameView);
};

#endif  // __GameView_H__