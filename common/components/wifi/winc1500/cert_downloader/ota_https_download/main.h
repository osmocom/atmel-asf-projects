/**
 * \file
 *
 * \brief Certificate Downloader via OTA (HTTPS) Example.
 *
 * Copyright (c) 2017 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/** Wi-Fi AP Settings. */
#define MAIN_WLAN_SSID                       "DEMO_AP" /**< Destination SSID */
#define MAIN_WLAN_AUTH                       M2M_WIFI_SEC_WPA_PSK /**< Security manner */
#define MAIN_WLAN_PSK                        "12345678" /**< Password for Destination SSID */

/** IP address parsing. */
#define IPV4_BYTE(val, index)                ((val >> (index * 8)) & 0xFF)

/** Content URI for download. */
#define MAIN_HTTP_FILE_URL                   "https://192.168.43.34/"

#define MAIN_HTTP_PORT_NUMBER                (4443)

/** Maximum size for packet buffer. */
#define MAIN_BUFFER_MAX_SIZE                 (1446)
/** Maximum file name length. */
#define MAIN_MAX_FILE_NAME_LENGTH            (100)
#define CERTIFICATES_BUFFER_SIZE             (6000)

/** No of certificates to be written to WINC **/
#define NUM_OF_ROOT_TLS_CHAIN_CERTIFICATES  5

typedef enum {
	NOT_READY = 0, /*!< Not ready. */
	WIFI_CONNECTED = 0x01, /*!< Wi-Fi is connected. */
    HTTP_DOWNLOAD_INITIATED = 0x02, /*!< Start download is called */
	GET_REQUESTED = 0x04, /*!< GET request is sent. */
	DOWNLOADING = 0x08, /*!< Running to download. */
	COMPLETED = 0x10, /*!< Download completed. */
	CANCELED = 0x20, /*!< Download canceled. */
	ROOT_CERT_DOWNLOAD = 0x40, /*!< Root cert Download. */
	TLS_CERT_DOWNLOAD = 0x80 /*!< TLS cert Download. */
} download_state;

typedef enum {
	ROOT_CERT = 1, 
	TLS_RSA_CERT = 2, 
	TLS_ECC_CERT = 3, 
} cert_type_t;

typedef struct
{
	cert_type_t cert_type;
	uint8_t numOfChainCert;
	uint8_t *filename;
} certFileInfo;

/** Root Certificates to be uploaded */
/** Chain of TLS Certificates and private key to be uploaded */
certFileInfo root_tls_certs_name[NUM_OF_ROOT_TLS_CHAIN_CERTIFICATES]=
{{ROOT_CERT,0,(uint8_t*)"PROWL_Root.cer"},
{ROOT_CERT,0,(uint8_t*)"NMA_Root.cer"},
{TLS_RSA_CERT,2,(uint8_t*)"winc_rsa.key"},
{TLS_RSA_CERT,0,(uint8_t*)"winc_rsa.cer"},
{TLS_RSA_CERT,0,(uint8_t*)"WincRootCA.cer"}
};

#ifdef __cplusplus
}
#endif

#endif /* MAIN_H_INCLUDED */
