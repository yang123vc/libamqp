#!/usr/bin/ruby
#
require 'rexml/document'

$header=<<eos
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
eos

module Outputters
  def mandatory(o)
    "mandatory;" if o[:mandatory]
  end  
  def default(o)
    "default=\"#{o[:default]}\";" if o[:default]
  end
  def comment(o)
    c = "#{mandatory(o)} #{default(o)}".strip
    "/* #{c} */" if c.length > 0
  end
end

class SimpleMapper
  include Outputters
  def initialize(t)
    @tag = t
  end
  def output(n, t, o = {})
      puts "    #{@tag} #{n}; #{comment(o)}"
  end
  
end

class TypedefedMapper
  include Outputters
  def initialize(t)
    @tag = t
  end
  def output(n, t, o = {})
    puts "    amqp_#{@tag}_t #{n}; #{comment(o)}" unless o[:multiple]
    puts "    amqp_multiple_#{@tag}_t #{n}; #{comment(o)}" if o[:multiple]
  end
end

class WildcardMapper
  include Outputters
  def output(n, t, o = {})
    puts "    amqp_#{n}_t #{n}; #{comment(o)}" 
  end
end

$mappings = {}
$mappings['boolean'] = SimpleMapper.new('int')
$mappings['uint'] = SimpleMapper.new('uint32_t')
$mappings['ushort'] = SimpleMapper.new('uint16_t')
$mappings['ulong'] = SimpleMapper.new('uint64_t')
$mappings['string'] = TypedefedMapper.new('string')
$mappings['binary'] = TypedefedMapper.new('binary')
$mappings['milliseconds'] = TypedefedMapper.new('milliseconds')
$mappings['ietf_language_tag'] = TypedefedMapper.new('ietf_language_tag')
$mappings['symbol'] = TypedefedMapper.new('symbol')
$mappings['fields'] = TypedefedMapper.new('symbol')
$mappings['transfer_number'] = TypedefedMapper.new('transfer_number')
$mappings['handle'] = SimpleMapper.new('uint32_t')
$mappings['role'] = TypedefedMapper.new('role')
$mappings['sender_settle_mode'] = TypedefedMapper.new('sender_settle_mode')
$mappings['receiver_settle_mode'] = TypedefedMapper.new('receiver_settle_mode')
# $mappings['sender_settle_mode'] = TypedefedMapper.new('settle_mode')
# $mappings['receiver_settle_mode'] = TypedefedMapper.new('settle_mode')
$mappings['*'] = WildcardMapper.new
$mappings['map'] = TypedefedMapper.new('map')
$mappings['sequence_no'] = TypedefedMapper.new('sequence_no')     # Why none a primitive type
$mappings['error'] = TypedefedMapper.new('error')
$mappings['delivery_number'] = TypedefedMapper.new('delivery_number')
$mappings['delivery_tag'] = TypedefedMapper.new('delivery_tag')
$mappings['sasl_code'] = TypedefedMapper.new('sasl_code')

$xpaths = {}
$xpaths['transport'] = '//section[@name=\'performatives\']/type[@provides=\'frame\']'
$xpaths['security'] = '//section[@name=\'sasl\']/type[@provides=\'sasl-frame\']'

class Parser
  def initialize(xml_file)
    @xml_file = xml_file
  end      
  def emit_field(name, type, options = {})
    mapper = $mappings[type]
    mapper.output(name, type, options) if mapper
    puts("// TODO - define #{type}") unless mapper
  end
  def field(f)
    name = f.attributes['name'].gsub(/-/, '_')
    type = f.attributes['type'].gsub(/-/, '_')
    options = {}
    options[:mandatory] = f.attributes['mandatory'] == 'true'
    options[:multiple] = f.attributes['multiple'] == 'true'
    options[:default] = f.attributes['default']
    options[:requires] = f.attributes['requires']
    emit_field(name, type, options) unless name == 'properties' and type == 'fields'
    puts("    amqp_property_fields_t properties;") if  name == 'properties' and type == 'fields'
  end
  def typedefs(n)
    name = n.attributes['name'].gsub(/-/, '_')
    puts "typedef struct amqp_frame_#{name}_t amqp_frame_#{name}_t;"
  end 
  def frame(n)
    name = n.attributes['name'].gsub(/-/, '_')
    puts "struct amqp_frame_#{name}_t {"
    n.elements.each('field') {|f| field(f) }
    puts "};"
  end

  def parse
    guard = File.basename(@xml_file, ".bare.xml").downcase
    puts($header)
    puts(<<-eos)
\#ifndef LIBAMQP_AMQP_AMQP_#{guard.upcase}_H
\#define LIBAMQP_AMQP_AMQP_#{guard.upcase}_H
    
\#include "Amqp/Types.h"
    
    eos
    xml_file = File.new(@xml_file)
    document = REXML::Document.new(xml_file)
    xpath = $xpaths[guard]
    document.root.each_element(xpath) { |e| typedefs e }
    puts
    document.root.each_element(xpath) { |e| frame e }
    puts("\#endif")
  end
end

if ARGV[0] 
  Parser.new(ARGV[0]).parse
else
  puts('No XML file provided')
end
