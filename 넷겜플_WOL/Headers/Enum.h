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

namespace ATTACK
{
	enum NAME { NORMAL, FIRE, ICE, BLAST, END };
}
namespace TEAMNUM
{
	enum TEAM { NONE, TEAM1, TEAM2 };
}
#endif // !__ENUM_H__
