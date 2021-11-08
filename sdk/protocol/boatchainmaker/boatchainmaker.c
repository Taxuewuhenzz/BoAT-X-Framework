/******************************************************************************
 * Copyright (C) 2018-2021 aitos.io
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

/*!@brief Perform RAW transaction

@file
chaninmaker.c contains functions to construct a raw transaction, perform it and 
wait for its receipt.
*/

/* self-header include */
#include "boatconfig.h"
#include "boatchainmaker.h"

#include "http2intf.h"
#include "boatplatform_internal.h"

#include "common/request.pb-c.h"
#include "common/transaction.pb-c.h"
#include "common/common.pb-c.h"
#include <time.h>

const char* chainmaker_chainId = "chain1";
const BCHAR *org_id = "wx-org1.chainmaker.org";
char txid_array[65] = {0};

void array_to_str(char* array, char *str, char lenth)
{
    char value_up;
    char value_down;
    int i = 0;
    int n = 0;

    if ((array == NULL) || (str == NULL))
    {
    		return;
    }

    for (i = 0; i < lenth; i++)
    {
        value_up = (array[i] & 0xf0) >> 4;
        if ((value_up >= 0) && (value_up <= 9))
        {
             str[n++] = value_up + 0x30;
        }
        else if ((value_up >= 0xA) && (value_up <= 0xF))
        {
             str[n++] = value_up + 0x57;
        }

        value_down = array[i] & 0x0f;
        if ((value_down >= 0) && (value_down <= 9))
        {    
             str[n++] = value_down + 0x30;
        }
        else if ((value_down >= 0xA) && (value_down <= 0xF))
        {
             str[n++] = value_down + 0x57;
        }
    }
}


BOAT_RESULT generateTxRequestPack(BoatHlchainmakerTx *tx_ptr, char *method, BoatTransactionPara *transaction_para, BoatFieldVariable *output_ptr)
{
	BOAT_RESULT result = BOAT_SUCCESS;

	BUINT32 packed_length_payload;
	Common__TransactPayload transactPayload = COMMON__TRANSACT_PAYLOAD__INIT;
	Common__KeyValuePair keyValuePair       = COMMON__KEY_VALUE_PAIR__INIT;
	transactPayload.contract_name           = tx_ptr->wallet_ptr->node_info.claim_contract_name;
	transactPayload.method                  = method;

	transactPayload.parameters = (Common__KeyValuePair**)BoatMalloc(sizeof(Common__KeyValuePair*) * transaction_para->n_parameters);
	
	int i;
	for (i = 0; i < transaction_para->n_parameters; i++)
	{
		Common__KeyValuePair* tkvp = BoatMalloc(sizeof(Common__KeyValuePair));
		memcpy(tkvp, &keyValuePair, sizeof(Common__KeyValuePair));
		tkvp->key   = transaction_para->parameters[i].key;
		tkvp->value = transaction_para->parameters[i].value;

		transactPayload.parameters[i] = tkvp;
	}	
	transactPayload.n_parameters  = transaction_para->n_parameters;

	/* pack the Common__TransactPayload */
	packed_length_payload = common__transact_payload__get_packed_size(&transactPayload);
	
	output_ptr->field_ptr = BoatMalloc(packed_length_payload);
	common__transact_payload__pack(&transactPayload, output_ptr->field_ptr);
	output_ptr->field_len = packed_length_payload;
//liuzhenhe
	printf("payload contract_name contract_name = %s\n",transactPayload.contract_name );
	printf("payload contract_name method = %s\n",transactPayload.method);
	for(i = 0; i < transactPayload.n_parameters; i++)
	{
		printf("payloadpara = %s\n",transactPayload.parameters[i]->key);
		printf("payloadpara = %s\n",transactPayload.parameters[i]->value);
	}

	printf("payload to proto= %d\n", output_ptr->field_len);
	for (i = 0; i < output_ptr->field_len; i++)
	{
		printf("%02x",output_ptr->field_ptr[i]);
	}
	printf("payload to proto\n");

	for (i = 0; i < transaction_para->n_parameters; i++)
	{	
		BoatFree(transactPayload.parameters[i]);
	}	

	BoatFree(transactPayload.parameters);	
	return result;
}

void get_tx_header_data(BoatHlchainmakerTx *tx_ptr, TxType tx_type, Accesscontrol__SerializedMember *sender, Common__TxHeader *tx_header) {

	long int timesec = 0;
	time(&timesec);
	//32 byte randrom generate
	BoatFieldMax32B  random_data;
	random_data.field_len = 32;
	BoatRandom(random_data.field, random_data.field_len, NULL);
	array_to_str(random_data.field, txid_array, random_data.field_len );

	tx_header->chain_id        = chainmaker_chainId;
	tx_header->tx_type         = tx_type;
	tx_header->tx_id           = txid_array;
	tx_header->timestamp       = timesec;
	tx_header->sender          = sender;
}

BOAT_RESULT hlchainmakerTransactionPacked(BoatHlchainmakerTx *tx_ptr, char* method, BoatTransactionPara *transaction_para, TxType tx_type)
{
	Common__TxRequest  tx_request  = COMMON__TX_REQUEST__INIT;
	Common__TxHeader   tx_header   = COMMON__TX_HEADER__INIT;
	Accesscontrol__SerializedMember sender = ACCESSCONTROL__SERIALIZED_MEMBER__INIT;

	BoatFieldVariable payloadPacked = {NULL, 0};
	BoatFieldVariable hash_data = {NULL, 0};
	BoatSignatureResult signatureResult;
	BUINT8   grpcHeader[5];
	BUINT8   hash[32];
	BUINT32  packedLength;
	BUINT32  packedHeaderLength;
	BUINT32  packedPayloadLength;
	BUINT8  *packedData = NULL;
	
	BOAT_RESULT result = BOAT_SUCCESS;
	boat_try_declare;
		
	/* step-0: param in check */
	if( ( tx_ptr == NULL ) || ( tx_ptr->wallet_ptr == NULL ) || \
		( tx_ptr->wallet_ptr->http2Context_ptr == NULL ) )
	{
		BoatLog(BOAT_LOG_CRITICAL, "parameter should not be NULL.");
		return BOAT_ERROR;
	}

	sender.org_id             = org_id;
	sender.member_info.len    = tx_ptr->wallet_ptr->user_client_info.cert.field_len;
	sender.member_info.data   = tx_ptr->wallet_ptr->user_client_info.cert.field_ptr;
	sender.is_full_cert       = true;

	get_tx_header_data(tx_ptr, tx_type, &sender, &tx_header);
	//liuzhenhe
	printf("get_tx_header_data origin id = %s\n",tx_header.chain_id);
	printf("get_tx_header_data org_id  = %s\n",tx_header.sender->org_id);
	printf("get_tx_header_data member_info  = %s\n",tx_header.sender->member_info.data);
	printf("get_tx_header_data is_full_cert  = %d\n",tx_header.sender->is_full_cert);
	printf("get_tx_header_data tx_id  = %s\n",tx_header.tx_id);
	printf("get_tx_header_data timestamp  = %d\n",tx_header.timestamp);
	printf("get_tx_header_data tx_type  = %d\n",tx_header.tx_type);
	/* step-1: compute payload packed length */
	result = generateTxRequestPack(tx_ptr,method,transaction_para, &payloadPacked);

	/* step-2: compute payload packed length */
	packedHeaderLength = common__tx_header__get_packed_size(&tx_header);
	packedLength = packedHeaderLength + payloadPacked.field_len;

	hash_data.field_ptr = BoatMalloc(packedLength);
	hash_data.field_len = packedLength;
	common__tx_header__pack(&tx_header, hash_data.field_ptr);
	 int i = 0;
	printf("tx_header to proto\n");
	for (i = 0; i < packedHeaderLength; i++)
	{
		printf("%02x",hash_data.field_ptr[i]);
	}
	printf("tx_header to proto\n");
	hash_data.field_ptr += packedHeaderLength;
	memcpy(hash_data.field_ptr, payloadPacked.field_ptr, payloadPacked.field_len);
	hash_data.field_ptr -= packedHeaderLength;

	printf("hash data start\n");
	for (i = 0; i < hash_data.field_len; i++)
	{
		printf("%02x", hash_data.field_ptr[i]);
	}
     printf("hash data end\n");
	/* step-3: compute hash */
	result = BoatHash(BOAT_HASH_SHA256,hash_data.field_ptr, 
					   hash_data.field_len, hash, NULL, NULL );

	if (result != BOAT_SUCCESS)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec BoatHash.");
		boat_throw(result, chainmakerProposalTransactionPacked_exception);
	}

	/* step-4: signature */
	result = BoatSignature(tx_ptr->wallet_ptr->user_client_info.prikeyCtx,
							hash, sizeof(hash), &signatureResult, NULL );

	if( result != BOAT_SUCCESS )
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to exec BoatSignature.");
		boat_throw(BOAT_ERROR_GEN_SIGNATURE_FAILED, chainmakerProposalTransactionPacked_exception);
	}
	printf("BoatSignature finish\n");
	if (!signatureResult.pkcs_format_used)
	{
		BoatLog(BOAT_LOG_CRITICAL, "Fail to find expect signature.");
		boat_throw(BOAT_ERROR_GEN_SIGNATURE_FAILED, chainmakerProposalTransactionPacked_exception);
	}

	/* step-5: pack the envelope */
	tx_request.header          = &tx_header;
	tx_request.payload.len     = payloadPacked.field_len;
	tx_request.payload.data    = payloadPacked.field_ptr;
	tx_request.signature.len   = signatureResult.pkcs_sign_length;
	tx_request.signature.data  = signatureResult.pkcs_sign;
	packedLength               = common__tx_request__get_packed_size(&tx_request);
	printf("envelope finish\n");
	/* step-6: packed length assignment */
	tx_ptr->wallet_ptr->http2Context_ptr->sendBuf.field_len = packedLength + sizeof(grpcHeader);
	if( tx_ptr->wallet_ptr->http2Context_ptr->sendBuf.field_len > BOAT_HLCHAINMAKER_HTTP2_SEND_BUF_MAX_LEN )
	{
		BoatLog(BOAT_LOG_CRITICAL, "packed length out of sendbuffer size limit.");
	}
	
		printf("envelope finish\n");
	/* step-7: packed data assignment */
	/* ---grpcHeader compute */
	grpcHeader[0] = 0x00;//uncompressed
	for(int i = 0 ; i < 4; i++)
	{
		grpcHeader[i + 1] = (packedLength >> (32 - 8*(i+1)))&0xFF;
	}
	/* ---generate packed data */
	packedData = tx_ptr->wallet_ptr->http2Context_ptr->sendBuf.field_ptr;
	memcpy(packedData, grpcHeader, sizeof(grpcHeader));
	common__tx_request__pack(&tx_request, &packedData[sizeof(grpcHeader)]);

	/* boat catch handle */
	boat_catch(chainmakerProposalTransactionPacked_exception)
	{
        BoatLog(BOAT_LOG_CRITICAL, "Exception: %d", boat_exception);
        result = boat_exception;
     }
	/* free malloc */
	BoatFree(payloadPacked.field_ptr);
	return result;
}











