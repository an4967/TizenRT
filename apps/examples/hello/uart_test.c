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

#include <iotbus/iotbus_uart.h>

iotbus_uart_context_h m_uart;
static char tx_buf[] = "NMLKJIHGFEDCBA";
static char tx_buf2[] = "987654321";
// static char tx_buf[] = "FEDCBA";
static char rx_buf[30] = { 0, };
static int tx_cnt = 0;
static int tx_size = 0;
static int tx_cnt2 = 0;
static int tx_size2 = 0;
static int rx_cnt = 0;

/****************************************************************************
 * hello_main
 ****************************************************************************/
static void uart_cb0(iotbus_uart_context_h hnd)
{
	printf("[UART] Writing is done!\n");
}

static void uart_cb1(iotbus_uart_context_h hnd)
{
	int ret;
	// dbg("-->TX [%d]!\n", tx_cnt);
	if (tx_cnt >= tx_size) {
		dbg("Write Done1!\n");
		return;
	}
	ret = iotbus_uart_write(hnd, &tx_buf[tx_cnt++], 1);
	if (ret < 0) {
		dbg("Fail to tx[%d]\n", ret);
	}
}

static void uart_cb3(iotbus_uart_context_h hnd)
{
	int ret;
	// dbg("-->TX [%d]!\n", tx_cnt);
	if (tx_cnt2 >= tx_size2) {
		dbg("Write Done2!\n");
		return;
	}
	ret = iotbus_uart_write(hnd, &tx_buf2[tx_cnt2++], 1);
	if (ret < 0) {
		dbg("Fail to tx2[%d]\n", ret);
	}
}

static void uart_cb2(iotbus_uart_context_h hnd)
{
	int ret;
	char tmp;

	// dbg("<--RX [%d]!\n", rx_cnt);
	ret = iotbus_uart_read(hnd, &tmp, 1);
	if (ret < 0) {
		dbg("!! READ ERROR [%d] !!\n", ret);
		return;
	}
	rx_buf[rx_cnt++] = tmp;
}

int uart_test_main(void)
{
	// printf("Hello, UART TEST!!\n");

	int i;

	m_uart = iotbus_uart_open(2);

	int ret;

	if (m_uart == NULL) {
		printf("!!!!!! m_uart is NULL !!!!!!\n");
		return 0;
	}

	ret = iotbus_uart_set_interrupt(m_uart, IOTBUS_UART_TX_EMPTY, uart_cb1, 250);
	if (ret != IOTBUS_ERROR_NONE) {
		printf("!!!!!! iotbus_uart_set_interrupt1 [%d] !!!!!!\n", ret);
		return 0;
	}

	ret = iotbus_uart_set_interrupt(m_uart, IOTBUS_UART_RX_AVAIL, uart_cb2, 250);
	if (ret != IOTBUS_ERROR_NONE) {
		printf("!!!!!! iotbus_uart_set_interrupt2 [%d] !!!!!!\n", ret);
		return 0;
	}

	rx_cnt = 0;
	tx_cnt = 0;
	tx_size = strlen(tx_buf);

	iotbus_uart_write(m_uart, &tx_buf[tx_cnt++], 1);
	
	int cnt = 3;
	while(cnt--) {
		sleep(1);
	}

	ret = iotbus_uart_unset_interrupt(m_uart, IOTBUS_UART_TX_EMPTY);
	if (ret != IOTBUS_ERROR_NONE) {
		printf("!!!!!! iotbus_uart_unset_interrupt1 [%d] !!!!!!\n", ret);
		return 0;
	}

	ret = iotbus_uart_set_interrupt(m_uart, IOTBUS_UART_TX_EMPTY, uart_cb3, 250);
	if (ret != IOTBUS_ERROR_NONE) {
		printf("!!!!!! iotbus_uart_set_interrupt3 [%d] !!!!!!\n", ret);
		return 0;
	}

	tx_cnt2 = 0;
	tx_size2 = strlen(tx_buf2);

	iotbus_uart_write(m_uart, &tx_buf2[tx_cnt2++], 1);

	cnt = 3;
	while(cnt--) {
		sleep(1);
	}
	// usleep(1000 * 1000);

	printf("Tx : %d/%d, %d/%d, Rx : %d\n--> ", tx_cnt, tx_size, tx_cnt2, tx_size2, rx_cnt);
	for (i = 0; i < rx_cnt; i++) {
		printf("%c", rx_buf[i]);
	}
	printf("\n[UART] DONE!!!\n");

	iotbus_uart_stop(m_uart);

	return 0;
}
