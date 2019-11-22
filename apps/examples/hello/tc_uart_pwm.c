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
#include <iotbus/iotbus_gpio.h>
#include <iotbus/iotbus_pwm.h>
#include <iotbus/iotbus_uart.h>

/****************************************************************************
 * hello_main
 ****************************************************************************/
void write_done(iotbus_error_e ret) {
	printf("UART write callback![%d]\n", ret);
	fflush(stdout);
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int hello_main(int argc, char *argv[])
#endif
{
	// printf("Hello, IOTBUS UART!!\n");

	// char *buf = "ABCDEF";
	// size_t len = 6;

	// char r_buf[100] = { 0, };
	// int ret;

	// iotbus_uart_context_h m_uart = iotbus_uart_init("/dev/ttyS1");

	// ret = iotbus_uart_async_write(m_uart, buf, len, write_done, 1000);
	// if (ret < 0) {
	// 	printf("UART Error 1 [%d]\n", ret);
	// 	return 0;
	// }

	// size_t recv = 0;
	// while (recv < len) {
	// 	ret = iotbus_uart_read(m_uart, r_buf, sizeof(r_buf));
	// 	if (ret < 0) {
	// 		printf("UART Error 2 [%d]\n", ret);
	// 		fflush(stdout);
	// 		break;
	// 	} else if (ret == 0) {
	// 		printf("UART Loop Done\n");
	// 		fflush(stdout);
	// 		break;
	// 	}
	// 	recv += ret;
	// 	printf("Result[%d] : %s\n", ret, r_buf);
	// 	memset(r_buf, 0 , sizeof(r_buf));
	// }

	// printf("UART Done\n");

	// printf("Hello, IOTBUS GPIO!!\n");
	
	// int value;
	// if (argc > 1) {
	// 	value = atoi(argv[1]);
	// }

	// printf("Value : %d\n", value);

	// iotbus_gpio_context_h m_gpio1 = iotbus_gpio_open(45);
	// iotbus_gpio_set_direction(m_gpio1, IOTBUS_GPIO_DIRECTION_OUT);

	// iotbus_gpio_context_h m_gpio2 = iotbus_gpio_open(49);
	// iotbus_gpio_set_direction(m_gpio2, IOTBUS_GPIO_DIRECTION_IN);

	// iotbus_gpio_write(m_gpio1, value);

	// int ret = iotbus_gpio_read(m_gpio2);

	// printf("ret : %d\n");

	// iotbus_gpio_close(m_gpio1);
	// iotbus_gpio_close(m_gpio2);

	// printf("Hello, IOTBUS PWM!!\n");

	// int ret;
	// int i;

	// iotbus_pwm_context_h m_pwm = iotbus_pwm_open(0, 0);
	
	// ret = iotbus_pwm_set_period(m_pwm, 50);
	// if (ret != IOTBUS_ERROR_NONE) {
	// 	printf("PWM ERROR 0 [%d]\n", ret);
	// 	return 0;
	// }

	// ret = iotbus_pwm_set_duty_cycle(m_pwm, 90);
	// if (ret != IOTBUS_ERROR_NONE) {
	// 	printf("PWM ERROR 1 [%d]\n", ret);
	// 	return 0;
	// }

	// iotbus_pwm_set_enabled(m_pwm, IOTBUS_PWM_ENABLE);
	// printf("Enabled!!\n");
	// sleep(5);

	// iotbus_pwm_set_enabled(m_pwm, IOTBUS_PWM_DISABLE);
	// printf("Disabled!!\n");

	// printf("[");
	// fflush(stdout);
	// for (i = 1; i < 100; i++) {
	// 	iotbus_pwm_set_enabled(m_pwm, IOTBUS_PWM_DISABLE);
	// 	ret = iotbus_pwm_set_duty_cycle(m_pwm, i);
	// 	if (ret != IOTBUS_ERROR_NONE) {
	// 		printf("PWM ERROR 1 [%d]\n", ret);
	// 		break;
	// 	}
	// 	ret = iotbus_pwm_set_enabled(m_pwm, IOTBUS_PWM_ENABLE);
	// 	if (ret != IOTBUS_ERROR_NONE) {
	// 		printf("PWM ERROR 0 [%d]\n", ret);
	// 		return 0;
	// 	}
		
	// 	if (i % 10 == 0) {
	// 		printf("=");
	// 		fflush(stdout);
	// 	}
	// 	usleep(10 * 1000);
	// }
	// printf("]\n");
	// fflush(stdout);

	// iotbus_pwm_set_enabled(m_pwm, IOTBUS_PWM_DISABLE);
	
	// iotbus_pwm_close(m_pwm);

	return 0;
}
