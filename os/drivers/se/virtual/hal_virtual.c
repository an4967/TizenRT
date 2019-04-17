/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
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
#include <tinyara/config.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tinyara/security_hal.h>

#define _IN_
#define _OUT_
#define _INOUT_

#define VH_TAG "[VH]"

#define VH_LOG printf

#define VH_ENTER														\
	do {																\
		VH_LOG(VH_TAG"[INFO] %s %s:%d\n", __FUNCTION__, __FILE__, __LINE__ ); \
	} while (0)

#define VH_ERR(fd)														\
	do {																\
		VH_LOG("[ERR:%s] %s %s:%d ret(%d) code(%s)\n",					\
			   VH_TAG, __FUNCTION__, __FILE__, __LINE__, fd, strerror(errno)); \
	} while(0)



//NOTE: THIS FILE IS AN EMPTY SHELL.
int hal_free_data(_IN_ hal_data *data)
{
	if(data) {
		if(data->data) {
			free(data->data);
		}
	}
	return 0;
}

int hal_get_status(void)
{
	return 0;
}

int hal_init(_IN_ hal_init_param *params)
{
	VH_ENTER;

	return 0;
}

int hal_deinit(void)
{
	VH_ENTER;

	return 0;
}


/**
 * Key Manager
 */
int hal_set_key(_IN_ hal_key_type mode, _IN_ uint32_t key_idx, _IN_ hal_data *key, _IN_ hal_data *prikey)
{
	VH_ENTER;
	VH_LOG("mode(%d) key index(%d) key(%p) private key(%p)\n", mode, key_idx, key, prikey);

	return 0;
}

int hal_get_key(_IN_ hal_key_type mode, _IN_ uint32_t key_idx, _OUT_ hal_data *key)
{
	VH_ENTER;

	VH_LOG("mode(%d) key index(%d) key(%p)\n", mode, key_idx, key);

	// return dummy key data to check data is sent well.
	uint8_t pubkey_data[] = {"hal_key_sssssssssssstttttttttt"};
	key->data_len = sizeof(pubkey_data);
	uint8_t *data = (uint8_t *)malloc(key->data_len);
	if (!data) {
		return -1;
	}

	memcpy(data, pubkey_data, key->data_len);

	key->data = data;

	return 0;
}

int hal_remove_key(_IN_ hal_key_type mode, _IN_ uint32_t key_idx)
{
	VH_ENTER;
	VH_LOG("mode(%d) key index(%d)\n", mode, key_idx);

	return 0;
}

int hal_generate_key(_IN_ hal_key_type mode, _IN_ uint32_t key_idx)
{
	VH_ENTER;
	VH_LOG("mode(%d) key index(%d)\n", mode, key_idx);

	return 0;
}

/**
 * Authenticate
 */
int hal_generate_random(_IN_ uint32_t len, _OUT_ hal_data *random)
{
	VH_ENTER;
	return 0;
}

int hal_get_hash(_IN_ hal_hash_type mode, _IN_ hal_data *input, _OUT_ hal_data *hash)
{
	VH_ENTER;
	return 0;
}

int hal_get_hmac(_IN_ hal_hmac_type mode, _IN_ hal_data *input, _IN_ uint32_t key_idx, _OUT_ hal_data *hmac)
{
	VH_ENTER;
	return 0;
}

int hal_rsa_sign_md(_IN_ hal_rsa_mode mode, _IN_ hal_data *hash, _IN_ uint32_t key_idx, _OUT_ hal_data *md)
{
	VH_ENTER;
	return 0;
}

int hal_rsa_verify_md(_IN_ hal_rsa_mode mode, _IN_ hal_data *hash, _IN_ hal_data *sign, _IN_ uint32_t key_idx)
{
	VH_ENTER;
	return 0;
}

int hal_ecdsa_sign_md(_IN_ hal_ecdsa_mode mode, _IN_ hal_data *hash, _IN_ uint32_t key_idx, _OUT_ hal_data *sign)
{
	VH_ENTER;
	return 0;
}

int hal_ecdsa_verify_md(_IN_ hal_ecdsa_mode mode, _IN_ hal_data *hash, _IN_ hal_data *sign, _IN_ uint32_t key_idx)
{
	VH_ENTER;
	return 0;
}

int hal_dh_generate_param(_IN_ uint32_t dh_idx, _INOUT_ hal_dh_data *dh_param)
{
	VH_ENTER;
	return 0;
}

int hal_dh_compute_shared_secret(_IN_ hal_dh_data *param, _IN_ uint32_t dh_idx, _OUT_ hal_data *shared_secret)
{
	VH_ENTER;
	return 0;
}

int hal_ecdh_compute_shared_secret(_IN_ hal_ecdh_data *ecdh_mode, _IN_ uint32_t key_idx, _OUT_ hal_data *shared_secret)
{
	VH_ENTER;
	return 0;
}



int hal_set_certificate(_IN_ uint32_t cert_idx, _IN_ hal_data *cert_in)
{
	VH_ENTER;

	if (!cert_in || !cert_in->data) {
		return -1;
	}
	VH_LOG("cert len(%d)\n", cert_in->data_len);

	return 0;
}

int hal_get_certificate(_IN_ uint32_t cert_idx, _OUT_ hal_data *cert_out)
{
	VH_ENTER;

	char *cert_data = {"cert_ccccccccccccceeeeeeeeeeeerrrrrrrrrrrttttttttttt"};
	hal_data cert_s;
	cert_s.data = (void *)cert_data;
	cert_s.data_len = sizeof(cert_data);

	cert_out->data = (unsigned char *)malloc(cert_s.data_len);
	memcpy(cert_out->data, cert_s.data, cert_s.data_len);
	cert_out->data_len = cert_s.data_len;

	return 0;
}

int hal_remove_certificate(_IN_ uint32_t cert_idx)
{
	VH_ENTER;

	return 0;
}

int hal_get_factory_key(_IN_ uint32_t key_idx, _IN_ hal_data *key)
{
	VH_ENTER;

	hal_data factory_s = {"factory", 7, NULL};

	key->data = (unsigned char *)malloc(7);
	memcpy(key->data, factory_s.data, 7);

	return 0;
}

int hal_get_factory_cert(_IN_ uint32_t cert_idx, _IN_ hal_data *cert)
{
	VH_ENTER;

	hal_data factory_s = {"factory", 7, NULL};

	cert->data = (unsigned char *)malloc(7);
	memcpy(cert->data, factory_s.data, 7);

	return 0;
}

int hal_get_factory_data(_IN_ uint32_t data_idx, _IN_ hal_data *data)
{
	VH_ENTER;

	hal_data factory_s = {"factory", 7, NULL};

	data->data = (unsigned char *)malloc(7);
	memcpy(data->data, factory_s.data, 7);

	return 0;
}

/**
 * Crypto
 */
int hal_aes_encrypt(_IN_ hal_data *dec_data, _IN_ hal_aes_param *aes_param, _IN_ uint32_t key_idx, _OUT_ hal_data *enc_data)
{
	VH_ENTER;

	enc_data->data = (unsigned char *)malloc(dec_data->data_len);
	memcpy(enc_data->data, dec_data->data,dec_data->data_len);
	enc_data->data_len = dec_data->data_len;

	return 0;
}

int hal_aes_decrypt(_IN_ hal_data *enc_data, _IN_ hal_aes_param *aes_param, _IN_ uint32_t key_idx, _OUT_ hal_data *dec_data)
{
	VH_ENTER;

	dec_data->data = (unsigned char *)malloc(enc_data->data_len);
	memcpy(dec_data->data, enc_data->data,enc_data->data_len);
	dec_data->data_len = enc_data->data_len;

	return 0;
}

int hal_rsa_encrypt(_IN_ hal_data *dec_data, _IN_ uint32_t key_idx, _OUT_ hal_data *enc_data)
{
	VH_ENTER;

	enc_data->data = (unsigned char *)malloc(dec_data->data_len);
	memcpy(enc_data->data, dec_data->data,dec_data->data_len);
	enc_data->data_len = dec_data->data_len;

	return 0;
}

int hal_rsa_decrypt(_IN_ hal_data *enc_data, _IN_ uint32_t key_idx, _OUT_ hal_data *dec_data)
{
	VH_ENTER;

	dec_data->data = (unsigned char *)malloc(enc_data->data_len);
	memcpy(dec_data->data, enc_data->data,enc_data->data_len);
	dec_data->data_len = enc_data->data_len;

	return 0;
}

/**
 * Secure Storage
 */
int hal_write_storage(_IN_ uint32_t ss_idx, _IN_ hal_data *data)
{
	VH_ENTER;
	return 0;
}

int hal_read_storage(_IN_ uint32_t ss_idx, _OUT_ hal_data *data)
{
	VH_ENTER;
	return 0;
}

int hal_delete_storage(_IN_ uint32_t ss_idx)
{
	VH_ENTER;
	return 0;
}
