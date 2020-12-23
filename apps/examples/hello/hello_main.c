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
#include <semaphore.h>
#include <time.h>
#include <wifi_manager/wifi_manager.h>

/****************************************************************************
 * hello_main
 ****************************************************************************/
static sem_t g_wm_sem = SEM_INITIALIZER(0);
static sem_t g_wm_func_sem = SEM_INITIALIZER(0);

#define WM_TEST_SIGNAL								\
	do {											\
		sem_post(&g_wm_sem);						\
		printf("[WT] T%d send signal\n", getpid());	\
	} while (0)

#define WM_TEST_WAIT								\
	do {											\
		printf("[WT] T%d wait signal\n", getpid());	\
		sem_wait(&g_wm_sem);                        \
	} while (0)

#define WM_TEST_FUNC_SIGNAL									\
	do {													\
		sem_post(&g_wm_func_sem);							\
		printf("[WT]  T%d send func signal\n", getpid());	\
	} while (0)

#define WM_TEST_FUNC_WAIT									\
	do {													\
		printf("[WT]  T%d wait func signal\n", getpid());	\
		sem_wait(&g_wm_func_sem);							\
	} while (0)

#define WM_TEST_LOG_START						\
	do {										\
		printf("[WT] -->%s\n", __FUNCTION__);	\
	} while (0)


#define WM_TEST_LOG_END							\
	do {										\
		printf("[WT] <--%s\n", __FUNCTION__);	\
	} while (0)


void wm_sta_connected(wifi_manager_result_e res)
{
	printf("[WT]  T%d --> %s res(%d)\n", getpid(), __FUNCTION__, res);
	WM_TEST_SIGNAL;
}

void wm_sta_disconnected(wifi_manager_disconnect_e disconn)
{
	sleep(2);
	printf("[WT]  T%d --> %s\n", getpid(), __FUNCTION__);
	WM_TEST_SIGNAL;
}

void wm_softap_sta_join(void)
{
	printf("[WT]  T%d --> %s\n", getpid(), __FUNCTION__);
	WM_TEST_SIGNAL;
}

void wm_softap_sta_leave(void)
{
	printf("[WT]  T%d --> %s\n", getpid(), __FUNCTION__);
	WM_TEST_SIGNAL;
}

void wm_scan_done(wifi_manager_scan_info_s **scan_result, wifi_manager_scan_result_e res)
{
	printf("[WT]  T%d --> %s\n", getpid(), __FUNCTION__);
	/* Make sure you copy the scan results onto a local data structure.
	 * It will be deleted soon eventually as you exit this function.
	 */
	if (scan_result == NULL) {
		WM_TEST_SIGNAL;
		return;
	}
	wifi_manager_scan_info_s *wifi_scan_iter = *scan_result;
	int cnt = 0;
	while (wifi_scan_iter != NULL) {
		cnt++;
		wifi_scan_iter = wifi_scan_iter->next;
	}
	printf("[WT] WiFi AP Scan Result : %d\n", cnt);
	WM_TEST_SIGNAL;
}


static wifi_manager_cb_s wifi_callbacks = {
	wm_sta_connected,
	wm_sta_disconnected,
	wm_softap_sta_join,
	wm_softap_sta_leave,
	wm_scan_done,
};

static struct timespec now;

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int hello_main(int argc, char *argv[])
#endif
{
	wifi_manager_result_e res = WIFI_MANAGER_SUCCESS;

	(void)clock_gettime(CLOCK_REALTIME, &now);
	printf("-- [Test] App Start : (%lu sec, %lu nsec)\n", (unsigned long)now.tv_sec, (unsigned long)now.tv_nsec);

	res = wifi_manager_init(&wifi_callbacks);
	if (res != WIFI_MANAGER_SUCCESS) {
		printf("[WT]  wifi_manager_init fail\n");
	}

	(void)clock_gettime(CLOCK_REALTIME, &now);
	printf("-- [Test] WiFI Init : (%lu sec, %lu nsec)\n", (unsigned long)now.tv_sec, (unsigned long)now.tv_nsec);

	res = wifi_manager_scan_ap();
	if (res != WIFI_MANAGER_SUCCESS) {
		printf("[WT]  scan Fail\n");
		return;
	}
	WM_TEST_WAIT; // wait the scan result

	(void)clock_gettime(CLOCK_REALTIME, &now);
	printf("-- [Test] WiFI Scan : (%lu sec, %lu nsec)\n", (unsigned long)now.tv_sec, (unsigned long)now.tv_nsec);

	wifi_manager_softap_config_s ap_config;
	strncpy(ap_config.ssid, "WiFiTest123", WIFIMGR_SSID_LEN-1);
	ap_config.ssid[WIFIMGR_SSID_LEN-1] = '\0';
	strncpy(ap_config.passphrase, "12345678", WIFIMGR_PASSPHRASE_LEN-1);
	ap_config.passphrase[WIFIMGR_PASSPHRASE_LEN-1] = '\0';
	ap_config.channel = 1;

	res = wifi_manager_set_mode(SOFTAP_MODE, &ap_config);
	if (res != WIFI_MANAGER_SUCCESS) {
		printf("[WT]  Run SoftAP Fail\n");
	}

	(void)clock_gettime(CLOCK_REALTIME, &now);
	printf("-- [Test] WiFI SoftAP : (%lu sec, %lu nsec)\n", (unsigned long)now.tv_sec, (unsigned long)now.tv_nsec);

	res = wifi_manager_set_mode(STA_MODE, NULL);
	if (res != WIFI_MANAGER_SUCCESS) {
		printf("[WT]  Set STA mode Fail\n");
		return;
	}

	(void)clock_gettime(CLOCK_REALTIME, &now);
	printf("-- [Test] WiFI STA : (%lu sec, %lu nsec)\n", (unsigned long)now.tv_sec, (unsigned long)now.tv_nsec);

	printf("-- WiFi Test Done!!\n");

	return 0;
}
