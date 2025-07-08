# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.

module Arrow
  class Field
    class << self
      # @api private
      def try_convert(value)
        case value
        when Hash
          begin
            new(value)
          rescue ArgumentError
            nil
          end
        else
          nil
        end
      end
    end

    alias_method :initialize_raw, :initialize
    private :initialize_raw

    # Creates a new {Arrow::Field}.
    #
    # @overload initialize(name, data_type, nullable=false)
    #
    #   @param name [String, Symbol] The name of the field.
    #
    #   @param data_type [Arrow::DataType, Hash, String, Symbol] The
    #     data type of the field.
    #
    #     You can specify data type as a description by `Hash`.
    #
    #     See {Arrow::DataType.resolve} how to specify data type
    #     description.
    #
    #   @param nullable [Boolean, nil] (false) Whether the field may contain
    #     zero or more nulls.
    #
    #     `nil` is processed as `true` (null may be contained).
    #
    #   @example Create a field with {Arrow::DataType}s
    #     Arrow::Field.new("visible", Arrow::BooleanDataType.new)
    #
    #   @example Create a field with data type description
    #     Arrow::Field.new("visible", :boolean)
    #
    #   @example Create a field with name as `Symbol`
    #     Arrow::Field.new(:visible, :boolean)
    #
    # @overload initialize(description)
    #
    #   @param description [Hash] The description of the field.
    #
    #     Field description is a raw `Hash`. Field description must
    #     have `:name` and `:data_type` values. `:name` is the name of
    #     the field. `:data_type` is the data type of the field. You
    #     can use {Arrow::DataType} or data type description as
    #     `:data_type` value.
    #
    #     See {Arrow::DataType.resolve} how to specify data type
    #     description.
    #
    #     There is a shortcut for convenience. If field description
    #     doesn't have `:data_type`, all keys except `:name` are
    #     processes as data type description. For example, the
    #     following field descriptions are the same:
    #
    #     ```ruby
    #     {name: "visible", data_type: {type: :boolean}}
    #     {name: "visible", type: :boolean} # Shortcut version
    #     ```
    #
    #   @option description [String, Symbol] :name The name of the field.
    #
    #   @option description [Arrow::DataType, Hash] :data_type The
    #     data type of the field. You can specify data type description
    #     by `Hash`.
    #
    #     See {Arrow::DataType.resolve} how to specify data type
    #     description.
    #
    #   @option description [Boolean, nil] :nullable Whether the field
    #     may contain zero or more nulls.
    #
    #     `nil` is processed as `true` (null may be contained).
    #
    #   @example Create a field with {Arrow::DataType}s
    #     Arrow::Field.new(name: "visible",
    #                      data_type: Arrow::BooleanDataType.new)
    #
    #   @example Create a field with data type description
    #     Arrow::Field.new(name: "visible", data_type: {type: :boolean}
    #
    #   @example Create a field with shortcut form
    #     Arrow::Field.new(name: "visible", type: :boolean)
    def initialize(*args)
      n_args = args.size
      case n_args
      when 1
        description = args[0]
        name = nil
        data_type = nil
        data_type_description = {}
        nullable = nil
        description.each do |key, value|
          key = key.to_sym
          case key
          when :name
            name = value
          when :data_type
            data_type = DataType.resolve(value)
          when :nullable
            nullable = value
          else
            data_type_description[key] = value
          end
        end
        data_type ||= DataType.resolve(data_type_description)
      when 2
        name = args[0]
        data_type = args[1]
        if data_type.is_a?(Hash)
          data_type = data_type.dup
          nullable = data_type.delete(:nullable)
        else
          nullable = nil
        end
        data_type = DataType.resolve(data_type)
      when 3
        name = args[0]
        data_type = DataType.resolve(args[1])
        nullable = args[2]
      else
        message = "wrong number of arguments (given #{n_args}, expected 1..3)"
        raise ArgumentError, message
      end
      nullable = true if nullable.nil?

      initialize_raw(name, data_type, nullable)
    end
  end
end
