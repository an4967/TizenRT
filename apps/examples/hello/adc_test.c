/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * examples/hello/hello_main.c
 *
 *   Copyright (C) 2008, 2011-2012 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>
#include <stdio.h>
#include <stdlib.h>

#include <iotbus/iotbus_error.h>
#include <iotbus/iotbus_adc.h>

/****************************************************************************
 * hello_main
 ****************************************************************************/
static void read_cb(int channel, uint32_t data)
{
	printf("Channel : %d, Data : %d\n", channel, data);
}

int adc_test_main(void)
{
	// printf("Hello, IOTBUS ADC!!\n");

	int ret;

	iotbus_adc_context_h m_adc1 = iotbus_adc_init(0, 0);
	iotbus_adc_context_h m_adc2 = iotbus_adc_init(0, 1);
	iotbus_adc_context_h m_adc3 = iotbus_adc_init(0, 2);

	// ret = iotbus_adc_get_state(m_adc);
	// if (ret < 0) {
	// 	printf("[ERROR] iotbus_adc_get_state1 : %d\n", ret);
	// 	return 0;
	// }
	// printf("iotbus_adc_get_state1 : %d\n", ret);

	// ret = iotbus_adc_start(m_adc, read_cb);
	// if (ret != IOTBUS_ERROR_NONE) {
	// 	printf("[ERROR] iotbus_adc_start : %d\n", ret);
	// 	return 0;
	// }

	// ret = iotbus_adc_get_state(m_adc);
	// if (ret < 0) {
	// 	printf("[ERROR] iotbus_adc_get_state2 : %d\n", ret);
	// 	return 0;
	// }
	// printf("iotbus_adc_get_state2 : %d\n", ret);

	// sleep(3);

	// if (ret == IOTBUS_ADC_BUSY) {
	// 	ret = iotbus_adc_stop(m_adc);
	// 	if (ret != IOTBUS_ERROR_NONE) {
	// 		printf("[ERROR] iotbus_adc_stop : %d\n", ret);
    //         return 0;
	// 	}
	// }

	// ret = iotbus_adc_get_state(m_adc);
	// if (ret < 0) {
	// 	printf("[ERROR] iotbus_adc_get_state3 : %d\n", ret);
	// 	return 0;
	// }
	// printf("iotbus_adc_get_state3 : %d\n", ret);

	ret = iotbus_adc_get_sample(m_adc1, 1000);
	if (ret < 0) {
		printf("[ERROR] iotbus_adc_get_sample1 : %d\n", ret);
        return 0;
	}
    printf("iotbus_adc_get_sample1 : %d\n", ret);

	ret = iotbus_adc_get_sample(m_adc2, 1000);
	if (ret < 0) {
		printf("[ERROR] iotbus_adc_get_sample2 : %d\n", ret);
        return 0;
	}
    printf("iotbus_adc_get_sample2 : %d\n", ret);

	ret = iotbus_adc_get_sample(m_adc3, 1000);
	if (ret < 0) {
		printf("[ERROR] iotbus_adc_get_sample3 : %d\n", ret);
        return 0;
	}
    printf("iotbus_adc_get_sample3 : %d\n", ret);

	iotbus_adc_deinit(m_adc1);
	iotbus_adc_deinit(m_adc2);
	iotbus_adc_deinit(m_adc3);

	return 0;
}
