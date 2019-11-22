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

#include <fcntl.h>
#include <tinyara/i2c.h>

/****************************************************************************
 * hello_main
 ****************************************************************************/
#define DATA_LENGTH 2100

static struct i2c_dev_s *i2c_dev = NULL;
static struct i2c_config_s configs = { 0, };

static void disp_buf(uint8_t* buf, int len)
{
    int i;
    for (i = 0; i < len; i++) {
        printf("%02x ", buf[i]);
        if (( i + 1 ) % 16 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

#ifdef CONFIG_BUILD_KERNEL
int main(int argc, FAR char *argv[])
#else
int hello_main(int argc, char *argv[])
#endif
{
	printf("Run SE TCs! \n");

	configs.frequency = 100000;
	configs.address = 0x23;
	configs.addrlen = 7;

	int port = 1;
	i2c_dev = up_i2cinitialize(port);
	if (i2c_dev == NULL) {
		printf("i2ctest_main: up_i2cinitialize(i2c:%d) failed\n", port);
		return -1;
	}

	int i = 0;
    int ret = -1;
	int cnt = 0;
	char temp[3] = {0,};
    uint8_t* data = (uint8_t*) malloc(DATA_LENGTH);
    uint8_t* data_wr = (uint8_t*) malloc(DATA_LENGTH);
    uint8_t* data_rd = (uint8_t*) malloc(DATA_LENGTH);

	// for (i = 0; i < DATA_LENGTH; i++) {
	// 	data[i] = 0X00;
	// }	
	// for (i = 0; i < DATA_LENGTH; i++) {
	// 	data_wr[i] = 0X00;
	// }
	// for (i = 0; i < DATA_LENGTH; i++) {
	// 	data_rd[i] = 0X00;
	// }

	// data_wr[0] = 0x02;
	// data_wr[1] = 0x00;
	// data_wr[2] = 0x00;
	// data_wr[3] = 0x00;
	// data_wr[4] = 0x10;

	// cnt = 5;
	// printf("Writing[1]...\n");
	// ret = i2c_write(i2c_dev, &configs, data_wr, cnt);

	// if(ret >= 0) {
	// 	printf("i2c read[1] wait...\n");
	// 	ret = i2c_read(i2c_dev, &configs, data_rd, 2);

	// 	if (ret >= 0) {
	// 		disp_buf(data_rd, 2);

	// 		ret = i2c_read(i2c_dev, &configs, data_rd, 2);
	// 		if(ret >= 0)
	// 		{
	// 			disp_buf(data_rd, 2);
	// 		} else {
	// 			printf("Fail to read2[1] : %d\n", ret);
	// 		}
	// 	} else {
	// 		printf("Fail to read[1] : %d\n", ret);
	// 	}
	// } else {
	// 	printf("Fail to write[1] : %d\n", ret);
	// }

	usleep(300 * 1000);

	for (i = 0; i < DATA_LENGTH; i++) {
		data[i] = 0x00;
	}	
	for (i = 0; i < DATA_LENGTH; i++) {
		data_wr[i] = 0x00;
	}
	for (i = 0; i < DATA_LENGTH; i++) {
		data_rd[i] = 0x00;
	}

	data_wr[0] = 0x08;
	data_wr[1] = 0x00;
	data_wr[2] = 0x00;
	data_wr[3] = 0x00;
	data_wr[4] = 0x00;

	cnt = 5;
	printf("Writing[2]...\n");
	ret = i2c_write(i2c_dev, &configs, data_wr, cnt);

	if(ret >= 0) {
		printf("i2c read[2] wait...\n");
		ret = i2c_read(i2c_dev, &configs, data_rd, 2);

		if (ret >= 0) {
			disp_buf(data_rd, 2);
			
			ret = i2c_read(i2c_dev, &configs, data_rd, 2);
			if(ret >= 0)
			{
				disp_buf(data_rd, 2);
			} else {
				printf("Fail to read2[2]\n");
			}
		} else {
			printf("Fail to read[2]\n");
		}
	} else {
		printf("Fail to write[2]\n");
	}

	usleep(300 * 1000);

	for (i = 0; i < DATA_LENGTH; i++) {
		data[i] = 0x00;
	}	
	for (i = 0; i < DATA_LENGTH; i++) {
		data_wr[i] = 0x00;
	}
	for (i = 0; i < DATA_LENGTH; i++) {
		data_rd[i] = 0x00;
	}

	data_wr[0] = 0x05;
	data_wr[1] = 0x40;
	data_wr[2] = 0x01;
	data_wr[3] = 0x08;
	data_wr[4] = 0x00;

	for(i=0;i < 2048; i++)
	{
		data_wr[i+5] = i;
	}

	cnt = 5 + 2048;
	printf("Writing[3]...\n");
	ret = i2c_write(i2c_dev, &configs, data_wr, cnt);

	if(ret >= 0) {
		disp_buf(data_wr, cnt);

		printf("i2c read[3] wait...\n");
		ret = i2c_read(i2c_dev, &configs, data_rd, 2);

		if (ret >= 0) {
			disp_buf(data_rd, 2);
			ret = i2c_read(i2c_dev, &configs, data_rd, 2);
			if(ret >= 0)
			{
				disp_buf(data_rd, 2);
			} else {
				printf("Fail to read2[3]\n");
			}
		} else {
			printf("Fail to read[3]\n");
		}
	} else {
		printf("Fail to write[3]\n");
	}

	printf("Done SE TCs! \n");

	free(data);
	free(data_wr);
	free(data_rd);

	return 0;
}
