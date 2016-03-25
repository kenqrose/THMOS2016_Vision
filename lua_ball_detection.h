#ifndef luaBallDetection_h_DEFINED
#define luaBallDetection_H_DEFINED

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}


extern "C"
int luaopen_BallDetection(lua_State *L);

#endif
