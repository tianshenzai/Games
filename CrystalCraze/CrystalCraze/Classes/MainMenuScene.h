#ifndef __MainMenuScene_H__
#define __MainMenuScene_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

class MainMenuScene : public cocos2d::CCLayer
{
private:
	cocos2d::CCSprite* star[5];
	string starname[5];
	cocos2d::CCSize size;

public:
	MainMenuScene();
	~MainMenuScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuPlayCallback(cocos2d::CCObject* pSender);

	void menuAboutCallback(cocos2d::CCObject* pSender);

	// ���ر���ͼƬ
	void loadBackground();

	// ��������
	void loadText();

	// ���ؿ�ʼ�͹��ڰ�ť
	void loadButton();

	// ����
	void update(float dt);

	// ��������λ��
	void updateGem();

	// ��ʾ����
	void showNumber();

    // implement the "static node()" method manually
    CREATE_FUNC(MainMenuScene);
};

#endif  // __MainMenuScene_H__