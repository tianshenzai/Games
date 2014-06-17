#ifndef __EffectManager_H__
#define __EffectManager_H__

#include "cocos2d.h"

class EffectManager : public cocos2d::CCLayer
{
public:
	EffectManager();
	~EffectManager();

	virtual bool init(); 

	// ��ըЧ��
	void explodeSpecial(cocos2d::CCPoint point, bool flag);

	// ɾ����ըЧ��
	void disappear(cocos2d::CCNode* who);

	CREATE_FUNC(EffectManager);
};

#endif // __EffectManager_H__