/*
 * Copyright (c) 2021 0x1abin
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef _MULTI_TIMER_H_
#define _MULTI_TIMER_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#ifdef __cplusplus  
extern "C" {  
#endif

typedef uint64_t (*PlatformTicksFunction_t)(void);

typedef struct MultiTimerHandle MultiTimer;

typedef void (*MultiTimerCallback_t)(MultiTimer* timer, void* userData);

struct MultiTimerHandle {
    MultiTimer* next;
    uint64_t deadline;
    MultiTimerCallback_t callback;
    void* userData;
};

/**
 * @brief Platform ticks function.
 * 
 * @param ticksFunc ticks function.
 * @return int 0 on success, -1 on error.
 */
int MultiTimerInstall(PlatformTicksFunction_t ticksFunc);

/**
 * @brief Start the timer work, add the handle into work list.
 * 
 * @param timer target handle strcut.
 * @param timing Set the start time.
 * @param callback deadline callback.
 * @param userData user data.
 * @return int 0: success, -1: fail.
 */
int MultiTimerStart(MultiTimer* timer, uint64_t timing, MultiTimerCallback_t callback, void* userData);

/**
 * @brief Stop the timer work, remove the handle off work list.
 * 
 * @param timer target handle strcut.
 * @return int 0: success, -1: fail.
 */
int MultiTimerStop(MultiTimer* timer);

/**
 * @brief Check the timer expried and call callback.
 * 
 * @return int The next timer expires.
 */
int MultiTimerYield(void);

/*********************************用户代码***************************************/
extern uint64_t systick;
uint64_t systick_get(void);
/* 创建一个定时器对象 */
extern MultiTimer tim1,tim2,tim3,tim4; 
extern bool RefreshFlage;
/* 定义定时器超时回调函数 */
void timer1_callback(MultiTimer* timer, void* userData);
void timer2_callback(MultiTimer* timer, void* userData);

extern uint8_t times;

void timer3_callback(MultiTimer* timer, void* userData);
            
#ifdef __cplusplus
} 
#endif

#endif
