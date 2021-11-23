/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: common/transaction.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "transaction.pb-c.h"
void   common__transaction__init
                     (Common__Transaction         *message)
{
  static const Common__Transaction init_value = COMMON__TRANSACTION__INIT;
  *message = init_value;
}
size_t common__transaction__get_packed_size
                     (const Common__Transaction *message)
{
  assert(message->base.descriptor == &common__transaction__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t common__transaction__pack
                     (const Common__Transaction *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &common__transaction__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t common__transaction__pack_to_buffer
                     (const Common__Transaction *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &common__transaction__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Common__Transaction *
       common__transaction__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Common__Transaction *)
     protobuf_c_message_unpack (&common__transaction__descriptor,
                                allocator, len, data);
}
void   common__transaction__free_unpacked
                     (Common__Transaction *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &common__transaction__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   common__transaction_info__init
                     (Common__TransactionInfo         *message)
{
  static const Common__TransactionInfo init_value = COMMON__TRANSACTION_INFO__INIT;
  *message = init_value;
}
size_t common__transaction_info__get_packed_size
                     (const Common__TransactionInfo *message)
{
  assert(message->base.descriptor == &common__transaction_info__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t common__transaction_info__pack
                     (const Common__TransactionInfo *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &common__transaction_info__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t common__transaction_info__pack_to_buffer
                     (const Common__TransactionInfo *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &common__transaction_info__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Common__TransactionInfo *
       common__transaction_info__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Common__TransactionInfo *)
     protobuf_c_message_unpack (&common__transaction_info__descriptor,
                                allocator, len, data);
}
void   common__transaction_info__free_unpacked
                     (Common__TransactionInfo *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &common__transaction_info__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor common__transaction__field_descriptors[4] =
{
  {
    "header",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    0,   /* quantifier_offset */
    offsetof(Common__Transaction, header),
    &common__tx_header__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "request_payload",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_BYTES,
    0,   /* quantifier_offset */
    offsetof(Common__Transaction, request_payload),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "request_signature",
    3,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_BYTES,
    0,   /* quantifier_offset */
    offsetof(Common__Transaction, request_signature),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "result",
    4,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    0,   /* quantifier_offset */
    offsetof(Common__Transaction, result),
    &common__result__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned common__transaction__field_indices_by_name[] = {
  0,   /* field[0] = header */
  1,   /* field[1] = request_payload */
  2,   /* field[2] = request_signature */
  3,   /* field[3] = result */
};
static const ProtobufCIntRange common__transaction__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 4 }
};
const ProtobufCMessageDescriptor common__transaction__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "common.Transaction",
  "Transaction",
  "Common__Transaction",
  "common",
  sizeof(Common__Transaction),
  4,
  common__transaction__field_descriptors,
  common__transaction__field_indices_by_name,
  1,  common__transaction__number_ranges,
  (ProtobufCMessageInit) common__transaction__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor common__transaction_info__field_descriptors[4] =
{
  {
    "transaction",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    0,   /* quantifier_offset */
    offsetof(Common__TransactionInfo, transaction),
    &common__transaction__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "block_height",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT64,
    0,   /* quantifier_offset */
    offsetof(Common__TransactionInfo, block_height),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "block_hash",
    3,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_BYTES,
    0,   /* quantifier_offset */
    offsetof(Common__TransactionInfo, block_hash),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "tx_index",
    4,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(Common__TransactionInfo, tx_index),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned common__transaction_info__field_indices_by_name[] = {
  2,   /* field[2] = block_hash */
  1,   /* field[1] = block_height */
  0,   /* field[0] = transaction */
  3,   /* field[3] = tx_index */
};
static const ProtobufCIntRange common__transaction_info__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 4 }
};
const ProtobufCMessageDescriptor common__transaction_info__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "common.TransactionInfo",
  "TransactionInfo",
  "Common__TransactionInfo",
  "common",
  sizeof(Common__TransactionInfo),
  4,
  common__transaction_info__field_descriptors,
  common__transaction_info__field_indices_by_name,
  1,  common__transaction_info__number_ranges,
  (ProtobufCMessageInit) common__transaction_info__init,
  NULL,NULL,NULL    /* reserved[123] */
};
