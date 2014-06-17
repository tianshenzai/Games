#ifndef __EffectManager_H__
#define __EffectManager_H__

#include "cocos2d.h"

class EffectManager : public cocos2d::CCLayer
{
public:
	EffectManager();
	~EffectManager();

	virtual bool init(); 

	// 爆炸效果
	void explodeSpecial(cocos2d::CCPoint point, bool flag);

	// 删除爆炸效果
	void disappear(cocos2d::CCNode* who);

	CREATE_FUNC(EffectManager);
};

#endif // __EffectManager_H__