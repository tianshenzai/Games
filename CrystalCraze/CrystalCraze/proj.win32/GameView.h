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
	int number;  // ��ʼ����ʱ��
	bool isover; // ������־
	bool isstart; // ��ʼ��־
	bool eliminate; // �������
	int choose;  // ѡ�еĸ���
	int choose2; // �ڶ���ѡ�еĸ���
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

	// �����¼�
	void ccTouchesBegan(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent);

    // implement the "static node()" method manually
    CREATE_FUNC(GameView);
};

#endif  // __GameView_H__