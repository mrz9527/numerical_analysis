// author : zhoukang
// date   : 2022-08-17 11:58:48

#ifndef NUMERICAL_ANALYSIS_BASICDEFINE_H
#define NUMERICAL_ANALYSIS_BASICDEFINE_H

#include "assert.h"

#ifdef ENABLE_DEBUG
#define ASSERT(expression, msg) assert(expression, msg)
#else
#define ASSERT(expression, msg)
#endif

#endif //NUMERICAL_ANALYSIS_BASICDEFINE_H
