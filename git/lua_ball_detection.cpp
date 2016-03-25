/* 
   Lua interface to Ball Detection utility

 */

//Input: YUYV image, LUT, int ball_detected
//If ball_detected=0, call Ball_detection_init
//If ball_detected=1, call Ball_detection
//If ball is found, set ball_detected=1, 
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

static int lua_lua_ball_detection(lua_State *L) {}

extern "C"
int luaopen_BallDetection (lua_State *L) {
  luaL_register(L, "BallDetection", BallDetection_lib);

  return 1;
}
