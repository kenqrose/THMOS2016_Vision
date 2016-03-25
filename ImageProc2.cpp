/* 
   Lua interface 2 to Image Processing utilities
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#ifdef __cplusplus
}
#endif

#include <stdint.h>
#include <math.h>
#include <vector>
#include <string>
#include <algorithm>


static int lua_ball_detection(lua_State *L) {}
static int lua_goal_detection(lua_State *L) {}

static const struct luaL_reg imageProc2_lib [] = {
  {"ball_detection", lua_ball_detection},
  {"goal_detection", lua_goal_detection},
  {NULL, NULL}
};


extern "C"
int luaopen_ImageProc2 (lua_State *L) {
  luaL_register(L, "ImageProc2", imageProc2_lib);

  return 1;
}
