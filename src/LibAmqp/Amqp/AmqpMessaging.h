/*
   Copyright 2011 StormMQ Limited

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 */
 
/*
  WARNING: This file was generated by scripts/generate-amqp-types.rb. DO NOT HAND EDIT.
 */
#ifndef LIBAMQP_AMQP_AMQP_MESSAGING_H
#define LIBAMQP_AMQP_AMQP_MESSAGING_H
#ifdef __cplusplus
extern "C" {
#endif

#include "AmqpTypes/AmqpTypes.h"
    
typedef struct amqp_addressing_source_t amqp_addressing_source_t;
typedef struct amqp_addressing_target_t amqp_addressing_target_t;
typedef struct amqp_addressing_delete_on_close_t amqp_addressing_delete_on_close_t;
typedef struct amqp_addressing_delete_on_no_links_t amqp_addressing_delete_on_no_links_t;
typedef struct amqp_addressing_delete_on_no_messages_t amqp_addressing_delete_on_no_messages_t;
typedef struct amqp_addressing_delete_on_no_links_or_messages_t amqp_addressing_delete_on_no_links_or_messages_t;

struct amqp_addressing_source_t {
    // 11 fields
    amqp_wildcard_t address; 
    amqp_terminus_durability_t durable; /* default="none"; */
    amqp_terminus_expiry_policy_t expiry_policy; /* default="session-end"; */
    amqp_seconds_t timeout; /* default="0"; */
    int dynamic; /* default="false"; */
    amqp_fields_t dynamic_node_properties; 
    amqp_symbol_t distribution_mode; 
    amqp_filter_set_t filter; 
    amqp_wildcard_t default_outcome; 
    amqp_multiple_symbol_t outcomes; 
    amqp_multiple_symbol_t capabilities; 
};

struct amqp_addressing_target_t {
    // 7 fields
    amqp_wildcard_t address; 
    amqp_terminus_durability_t durable; /* default="none"; */
    amqp_terminus_expiry_policy_t expiry_policy; /* default="session-end"; */
    amqp_seconds_t timeout; /* default="0"; */
    int dynamic; /* default="false"; */
    amqp_fields_t dynamic_node_properties; 
    amqp_multiple_symbol_t capabilities; 
};

struct amqp_addressing_delete_on_close_t {
    // described amqp:delete-on-close:list
    amqp_list_t delete_on_close; 
};

struct amqp_addressing_delete_on_no_links_t {
    // described amqp:delete-on-no-links:list
    amqp_list_t delete_on_no_links; 
};

struct amqp_addressing_delete_on_no_messages_t {
    // described amqp:delete-on-no-messages:list
    amqp_list_t delete_on_no_messages; 
};

struct amqp_addressing_delete_on_no_links_or_messages_t {
    // described amqp:delete-on-no-links-or-messages:list
    amqp_list_t delete_on_no_links_or_messages; 
};
typedef struct amqp_delivery_state_received_t amqp_delivery_state_received_t;
typedef struct amqp_delivery_state_accepted_t amqp_delivery_state_accepted_t;
typedef struct amqp_delivery_state_rejected_t amqp_delivery_state_rejected_t;
typedef struct amqp_delivery_state_released_t amqp_delivery_state_released_t;
typedef struct amqp_delivery_state_modified_t amqp_delivery_state_modified_t;

struct amqp_delivery_state_received_t {
    // 2 fields
    uint32_t section_number; /* mandatory; */
    uint64_t section_offset; /* mandatory; */
};

struct amqp_delivery_state_accepted_t {
    // described amqp:accepted:list
    amqp_list_t accepted; 
};

struct amqp_delivery_state_rejected_t {
    // 1 fields
    amqp_definition_error_t error; 
};

struct amqp_delivery_state_released_t {
    // described amqp:released:list
    amqp_list_t released; 
};

struct amqp_delivery_state_modified_t {
    // 3 fields
    int delivery_failed; 
    int undeliverable_here; 
    amqp_fields_t message_annotations; 
};
typedef struct amqp_messaging_header_t amqp_messaging_header_t;
typedef struct amqp_messaging_delivery_annotations_t amqp_messaging_delivery_annotations_t;
typedef struct amqp_messaging_message_annotations_t amqp_messaging_message_annotations_t;
typedef struct amqp_messaging_properties_t amqp_messaging_properties_t;
typedef struct amqp_messaging_application_properties_t amqp_messaging_application_properties_t;
typedef struct amqp_messaging_data_t amqp_messaging_data_t;
typedef struct amqp_messaging_amqp_sequence_t amqp_messaging_amqp_sequence_t;
typedef struct amqp_messaging_amqp_value_t amqp_messaging_amqp_value_t;
typedef struct amqp_messaging_footer_t amqp_messaging_footer_t;

struct amqp_messaging_header_t {
    // 5 fields
    int durable; 
    uint8_t priority; 
    amqp_milliseconds_t ttl; 
    int first_acquirer; 
    uint32_t delivery_count; 
};

struct amqp_messaging_delivery_annotations_t {
    // described amqp:delivery-annotations:map
    amqp_map_t delivery_annotations; 
};

struct amqp_messaging_message_annotations_t {
    // described amqp:message-annotations:map
    amqp_map_t message_annotations; 
};

struct amqp_messaging_properties_t {
    // 13 fields
    amqp_wildcard_t message_id; 
    amqp_binary_t user_id; 
    amqp_wildcard_t to; 
    amqp_string_t subject; 
    amqp_wildcard_t reply_to; 
    amqp_wildcard_t correlation_id; 
    amqp_symbol_t content_type; 
    amqp_symbol_t content_encoding; 
    amqp_timestamp_t absolute_expiry_time; 
    amqp_timestamp_t creation_time; 
    amqp_string_t group_id; 
    amqp_sequence_no_t group_sequence; 
    amqp_string_t reply_to_group_id; 
};

struct amqp_messaging_application_properties_t {
    // described amqp:application-properties:map
    amqp_map_t application_properties; 
};

struct amqp_messaging_data_t {
    // described amqp:data:binary
    amqp_binary_t data; 
};

struct amqp_messaging_amqp_sequence_t {
    // described amqp:amqp-sequence:list
    amqp_list_t amqp_sequence; 
};

struct amqp_messaging_amqp_value_t {
    // described amqp:amqp-value:*
    amqp_wildcard_t amqp_value; 
};

struct amqp_messaging_footer_t {
    // described amqp:footer:map
    amqp_map_t footer; 
};
#ifdef __cplusplus
}
#endif
#endif