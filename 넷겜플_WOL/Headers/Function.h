#pragma once

#ifndef __FUNCTION_H__
#define __FUNCTION_H__

template <typename T>
void Safe_Delete(T& _obj)
{
	if (_obj)
	{
		delete _obj;
		_obj = nullptr;
	}
}

template <typename T>
bool CompareY(T& _Dst, T& _Src)
{
	return _Dst->Get_Info().fY < _Src->Get_Info().fY;
}
#endif // !__FUNCTION_H__
