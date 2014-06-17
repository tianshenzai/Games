#ifndef __Gem_H__
#define __GemH__

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

class Gem : public cocos2d::CCSprite
{
private:
	int x;
	int y;
public:
	Gem();
	~Gem();

	// 移动宝石
	void removeGem(int speed, int tag, cocos2d::CCPoint p);

	// 移动宝石并返回
	void removeGemAndBack(float speed, int tag, cocos2d::CCPoint source, cocos2d::CCPoint dest);

	virtual bool init(); 

	CREATE_FUNC(Gem);
};

#endif