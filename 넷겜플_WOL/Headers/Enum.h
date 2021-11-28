#pragma once

#ifndef __ENUM_H__
#define __ENUM_H__

namespace BULLET
{
	enum DIR { LEFT, RIGHT, UP, DOWN, END };
}

namespace OBJID
{
	enum ID { PLAYER, SHIELD, ATTACK, PRISON, GOLD, SHOP, EFFECT, INGAME_UI, UI, MOUSE, END };
}

namespace GROUPID
{
	enum ID { BACKGROUND, OBJECT, EFFECT, UI, END };
}

#endif // !__ENUM_H__
