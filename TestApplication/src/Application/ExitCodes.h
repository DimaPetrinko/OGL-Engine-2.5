#pragma once

#define EXIT_CODE_NOT_INITIALIZED 0xff
#define EXIT_CODE_RUNNING 0xfe
#define EXIT_CODE_NORMAL 0
#define EXIT_CODE_ERROR 1
#define EXIT_CODE_2 2
#define EXIT_CODE_3 3

#define CallAndReturn(f, c) f; if (c != EXIT_CODE_RUNNING) return
#define CallAndReturnCustom(f, c, t) f; if (c != t) return
#define CallAndBreak(f, c) f; if (c != EXIT_CODE_RUNNING) break