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
	int number;  // ��ʼ����ʱ��
	bool isover; // ������־
	bool isstart; // ��ʼ��־
	bool eliminate; // �������
	int choose;  // ѡ�еĸ���
	int choose2; // �ڶ���ѡ�еĸ���
	string choosename[5]; // ѡ������

public:
	GameScene();
	~GameScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuPauseCallback(cocos2d::CCObject* pSender);

	// ���ر���ͼƬ
	void loadBackground();

	// ����ͷ��
	void loadHeader();

	// ������ͣ��ť
	void loadButton();

	// ����ʱ����
	void loadTimer();

	// �������ֺ���Ч
	void loadMusic();

	// ���ؿ�ʼ����
	void loadStart();

	// ���¿�ʼ����
	void updateStart();

	// ���½�������
	void updateOver();

	// ��ʾ��ʼ����
	void initScore();

	// ���·���
	void updateScore();

	// ����
	void update(float dt);

	// �����¼�
	void ccTouchesBegan(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent);

    // implement the "static node()" method manually
    CREATE_FUNC(GameScene);
};

#endif  // __GameScene_H__