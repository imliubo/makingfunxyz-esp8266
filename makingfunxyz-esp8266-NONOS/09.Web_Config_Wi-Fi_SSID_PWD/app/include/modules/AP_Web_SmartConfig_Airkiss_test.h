/*MIT License
Copyright (c) 2018 imliubo
Github  https://github.com/imliubo
Website https://www.makingfun.xyz
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef AP_WEB_SMARTCONFIG_AIRKISS_TEST_H
#define AP_WEB_SMARTCONFIG_AIRKISS_TEST_H

/*********************
 *      INCLUDES
 *********************/
#include "osapi.h"
#include "espconn.h"
#include "mem.h"
#include "os_type.h"
#include "driver/uart.h"
#include "airkiss.h"
#include "smartconfig.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

void AP_test(void);
void AP_recv_data_process(uint8 *pdata);

void SmartConfig_test(void);

void Airkiss_test(void);

void SmartConfig_Airkiss_test(void);

void Web_config_test(void);


/**********************
 *      MACROS
 **********************/

#endif /*  AP_WEB_SMARTCONFIG_AIRKISS_TEST_H  */
