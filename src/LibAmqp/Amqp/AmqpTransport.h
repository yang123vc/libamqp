/*
   Copyright 2011-2012 StormMQ Limited

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
  WARNING: This file was generated by scripts/generate. DO NOT HAND EDIT.
     scripts/generate uses:
  	scripts/generate-amqp-types.rb to generate AMQP typedefs
  	scripts/generate-frame-decode-methods.rb to generate frame decode methods
  	scripts/generate-type-decode-methods.rb to generate messaging type decode methods
 */
#ifndef LIBAMQP_AMQP_AMQP_TRANSPORT_H
#define LIBAMQP_AMQP_AMQP_TRANSPORT_H
#ifdef __cplusplus
extern "C" {
#endif

#include "AmqpTypes/AmqpTypes.h"
    
typedef struct amqp_transport_open_t amqp_transport_open_t;
typedef struct amqp_transport_begin_t amqp_transport_begin_t;
typedef struct amqp_transport_attach_t amqp_transport_attach_t;
typedef struct amqp_transport_flow_t amqp_transport_flow_t;
typedef struct amqp_transport_transfer_t amqp_transport_transfer_t;
typedef struct amqp_transport_disposition_t amqp_transport_disposition_t;
typedef struct amqp_transport_detach_t amqp_transport_detach_t;
typedef struct amqp_transport_end_t amqp_transport_end_t;
typedef struct amqp_transport_close_t amqp_transport_close_t;
    
/*
    <type name='open' provides='frame' class='composite' source='list'>
      <descriptor name='amqp:open:list' code='0x00000000:0x00000010'/>
      <field name='container-id' mandatory='true' type='string'/>
      <field name='hostname' type='string'/>
      <field name='max-frame-size' default='4294967295' type='uint'/>
      <field name='channel-max' default='65535' type='ushort'/>
      <field name='idle-time-out' type='milliseconds'/>
      <field name='outgoing-locales' type='ietf-language-tag' multiple='true'/>
      <field name='incoming-locales' type='ietf-language-tag' multiple='true'/>
      <field name='offered-capabilities' type='symbol' multiple='true'/>
      <field name='desired-capabilities' type='symbol' multiple='true'/>
      <field name='properties' type='fields'/>
    </type>
*/
struct amqp_transport_open_t {
    // 10 fields
    amqp_string_t container_id; /* mandatory; */
    amqp_string_t hostname; 
    int max_frame_size_is_null;
    uint32_t max_frame_size; /* default="4294967295"; */
    int channel_max_is_null;
    uint16_t channel_max; /* default="65535"; */
    int idle_time_out_is_null;
    amqp_milliseconds_t idle_time_out; 
    amqp_multiple_ietf_language_tag_t outgoing_locales; 
    amqp_multiple_ietf_language_tag_t incoming_locales; 
    amqp_multiple_symbol_t offered_capabilities; 
    amqp_multiple_symbol_t desired_capabilities; 
    amqp_fields_t properties; 
};
    
/*
    <type name='begin' provides='frame' class='composite' source='list'>
      <descriptor name='amqp:begin:list' code='0x00000000:0x00000011'/>
      <field name='remote-channel' type='ushort'/>
      <field name='next-outgoing-id' mandatory='true' type='transfer-number'/>
      <field name='incoming-window' mandatory='true' type='uint'/>
      <field name='outgoing-window' mandatory='true' type='uint'/>
      <field name='handle-max' default='4294967295' type='handle'/>
      <field name='offered-capabilities' type='symbol' multiple='true'/>
      <field name='desired-capabilities' type='symbol' multiple='true'/>
      <field name='properties' type='fields'/>
    </type>
*/
struct amqp_transport_begin_t {
    // 8 fields
    int remote_channel_is_null;
    uint16_t remote_channel; 
    int next_outgoing_id_is_null;
    amqp_sequence_no_t next_outgoing_id; /* mandatory; */
    uint32_t incoming_window; /* mandatory; */
    uint32_t outgoing_window; /* mandatory; */
    int handle_max_is_null;
    amqp_handle_t handle_max; /* default="4294967295"; */
    amqp_multiple_symbol_t offered_capabilities; 
    amqp_multiple_symbol_t desired_capabilities; 
    amqp_fields_t properties; 
};
    
/*
    <type name='attach' provides='frame' class='composite' source='list'>
      <descriptor name='amqp:attach:list' code='0x00000000:0x00000012'/>
      <field name='name' mandatory='true' type='string'/>
      <field name='handle' mandatory='true' type='handle'/>
      <field name='role' mandatory='true' type='role'/>
      <field name='snd-settle-mode' default='mixed' type='sender-settle-mode'/>
      <field name='rcv-settle-mode' default='first' type='receiver-settle-mode'/>
      <field name='source' requires='source' type='*'/>
      <field name='target' requires='target' type='*'/>
      <field name='unsettled' type='map'/>
      <field name='incomplete-unsettled' default='false' type='boolean'/>
      <field name='initial-delivery-count' type='sequence-no'/>
      <field name='max-message-size' type='ulong'/>
      <field name='offered-capabilities' type='symbol' multiple='true'/>
      <field name='desired-capabilities' type='symbol' multiple='true'/>
      <field name='properties' type='fields'/>
    </type>
*/
struct amqp_transport_attach_t {
    // 14 fields
    amqp_string_t name; /* mandatory; */
    int handle_is_null;
    amqp_handle_t handle; /* mandatory; */
    int role_is_null;
    amqp_role_t role; /* mandatory; */
    int snd_settle_mode_is_null;
    amqp_sender_settle_mode_t snd_settle_mode; /* default="mixed"; */
    int rcv_settle_mode_is_null;
    amqp_receiver_settle_mode_t rcv_settle_mode; /* default="first"; */
    amqp_wildcard_t source; 
    amqp_wildcard_t target; 
    amqp_map_t unsettled; 
    int incomplete_unsettled_is_null;
    int incomplete_unsettled; /* default="false"; */
    int initial_delivery_count_is_null;
    amqp_sequence_no_t initial_delivery_count; 
    int max_message_size_is_null;
    uint64_t max_message_size; 
    amqp_multiple_symbol_t offered_capabilities; 
    amqp_multiple_symbol_t desired_capabilities; 
    amqp_fields_t properties; 
};
    
/*
    <type name='flow' provides='frame' class='composite' source='list'>
      <descriptor name='amqp:flow:list' code='0x00000000:0x00000013'/>
      <field name='next-incoming-id' type='transfer-number'/>
      <field name='incoming-window' mandatory='true' type='uint'/>
      <field name='next-outgoing-id' mandatory='true' type='transfer-number'/>
      <field name='outgoing-window' mandatory='true' type='uint'/>
      <field name='handle' type='handle'/>
      <field name='delivery-count' type='sequence-no'/>
      <field name='link-credit' type='uint'/>
      <field name='available' type='uint'/>
      <field name='drain' default='false' type='boolean'/>
      <field name='echo' default='false' type='boolean'/>
      <field name='properties' type='fields'/>
    </type>
*/
struct amqp_transport_flow_t {
    // 11 fields
    int next_incoming_id_is_null;
    amqp_sequence_no_t next_incoming_id; 
    uint32_t incoming_window; /* mandatory; */
    int next_outgoing_id_is_null;
    amqp_sequence_no_t next_outgoing_id; /* mandatory; */
    uint32_t outgoing_window; /* mandatory; */
    int handle_is_null;
    amqp_handle_t handle; 
    int delivery_count_is_null;
    amqp_sequence_no_t delivery_count; 
    int link_credit_is_null;
    uint32_t link_credit; 
    int available_is_null;
    uint32_t available; 
    int drain_is_null;
    int drain; /* default="false"; */
    int echo_is_null;
    int echo; /* default="false"; */
    amqp_fields_t properties; 
};
    
/*
    <type name='transfer' provides='frame' class='composite' source='list'>
      <descriptor name='amqp:transfer:list' code='0x00000000:0x00000014'/>
      <field name='handle' mandatory='true' type='handle'/>
      <field name='delivery-id' type='delivery-number'/>
      <field name='delivery-tag' type='delivery-tag'/>
      <field name='message-format' type='message-format'/>
      <field name='settled' type='boolean'/>
      <field name='more' default='false' type='boolean'/>
      <field name='rcv-settle-mode' type='receiver-settle-mode'/>
      <field name='state' requires='delivery-state' type='*'/>
      <field name='resume' default='false' type='boolean'/>
      <field name='aborted' default='false' type='boolean'/>
      <field name='batchable' default='false' type='boolean'/>
    </type>
*/
struct amqp_transport_transfer_t {
    // 11 fields
    int handle_is_null;
    amqp_handle_t handle; /* mandatory; */
    int delivery_id_is_null;
    amqp_sequence_no_t delivery_id; 
    amqp_delivery_tag_t delivery_tag; 
    int message_format_is_null;
    amqp_message_format_t message_format; 
    int settled_is_null;
    int settled; 
    int more_is_null;
    int more; /* default="false"; */
    int rcv_settle_mode_is_null;
    amqp_receiver_settle_mode_t rcv_settle_mode; 
    amqp_wildcard_t state; 
    int resume_is_null;
    int resume; /* default="false"; */
    int aborted_is_null;
    int aborted; /* default="false"; */
    int batchable_is_null;
    int batchable; /* default="false"; */
};
    
/*
    <type name='disposition' provides='frame' class='composite' source='list'>
      <descriptor name='amqp:disposition:list' code='0x00000000:0x00000015'/>
      <field name='role' mandatory='true' type='role'/>
      <field name='first' mandatory='true' type='delivery-number'/>
      <field name='last' type='delivery-number'/>
      <field name='settled' default='false' type='boolean'/>
      <field name='state' requires='delivery-state' type='*'/>
      <field name='batchable' default='false' type='boolean'/>
    </type>
*/
struct amqp_transport_disposition_t {
    // 6 fields
    int role_is_null;
    amqp_role_t role; /* mandatory; */
    int first_is_null;
    amqp_sequence_no_t first; /* mandatory; */
    int last_is_null;
    amqp_sequence_no_t last; 
    int settled_is_null;
    int settled; /* default="false"; */
    amqp_wildcard_t state; 
    int batchable_is_null;
    int batchable; /* default="false"; */
};
    
/*
    <type name='detach' provides='frame' class='composite' source='list'>
      <descriptor name='amqp:detach:list' code='0x00000000:0x00000016'/>
      <field name='handle' mandatory='true' type='handle'/>
      <field name='closed' default='false' type='boolean'/>
      <field name='error' type='error'/>
    </type>
*/
struct amqp_transport_detach_t {
    // 3 fields
    int handle_is_null;
    amqp_handle_t handle; /* mandatory; */
    int closed_is_null;
    int closed; /* default="false"; */
    amqp_error_t error; 
};
    
/*
    <type name='end' provides='frame' class='composite' source='list'>
      <descriptor name='amqp:end:list' code='0x00000000:0x00000017'/>
      <field name='error' type='error'/>
    </type>
*/
struct amqp_transport_end_t {
    // 1 fields
    amqp_error_t error; 
};
    
/*
    <type name='close' provides='frame' class='composite' source='list'>
      <descriptor name='amqp:close:list' code='0x00000000:0x00000018'/>
      <field name='error' type='error'/>
    </type>
*/
struct amqp_transport_close_t {
    // 1 fields
    amqp_error_t error; 
};
#ifdef __cplusplus
}
#endif
#endif
