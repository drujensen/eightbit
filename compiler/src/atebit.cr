class Atebit
  VERSION = "0.1.0"
  property codes = {} of String => UInt8

  def initialize
    # opcodes for atebit computer
    self.codes = {
      "HLT"  => 0x00,
      "NOP"  => 0x04,
      "LDA#" => 0x05,
      "LDA"  => 0x06,
      "LDA*" => 0x07,
      "STA"  => 0x08,
      "STA*" => 0x09,

      "ADD#" => 0x10,
      "ADD"  => 0x11,
      "SUB#" => 0x12,
      "SUB"  => 0x13,
      "ADC#" => 0x14,
      "ADC"  => 0x15,
      "SBC#" => 0x16,
      "SBC"  => 0x17,
      "AND#" => 0x18,
      "AND"  => 0x19,
      "ORA#" => 0x1A,
      "ORA"  => 0x1B,
      "EOR#" => 0x1C,
      "EOR"  => 0x1D,
      "INC"  => 0x1E,
      "DEC"  => 0x1F,

      "CMP#" => 0x20,
      "CMP"  => 0x21,
      "BCC"  => 0x22,
      "BCS"  => 0x23,
      "BEQ"  => 0x24,
      "BNE"  => 0x25,
      "CLC"  => 0x26,
      "SEC"  => 0x27,

      "JMP" => 0x2A,
      "JSR" => 0x2B,
      "RET" => 0x2C,
      "PSH" => 0x2D,
      "POP" => 0x2E,

      "INA"  => 0x30,
      "OUTA" => 0x31,
      "OUT#" => 0x32,
      "OUT"  => 0x33,
      "OUT*" => 0x34,
    } of String => UInt8
  end

  def compile
    # split into lines
    program = ARGF.gets_to_end.split("\n")

    # remove comments, extra space and empty lines
    program = program.map { |line| line.split(";")[0].strip }.reject(&.empty?)

    # split strings into chars and flatten to memory layout
    program = program.map do |line|
      if line.includes? "\""
        parts = line.split(" \"")
        values = parts[1].gsub("\"", "").bytes.map(&.to_s(16))
        values[0] = parts[0] + values[0]
        values
      else
        line
      end
    end.flatten

    # split non label whitespace and flatten to memory layout
    program = program.map do |line|
      line.split(/(?<=[^:\s+])[ ]/)
    end.flatten

    # track labels with positions
    labels = {} of String => UInt8
    program = program.map_with_index do |line, idx|
      if line.includes? ":"
        parts = line.split(":")
        labels[parts[0].strip] = idx.to_u8
        line = parts[1].strip
      end

      line
    end

    puts "labels: #{labels}"
    puts "program: #{program}"

    file = File.new("a.out", "wb")
    program.each_with_index do |line, idx|
      if codes.has_key? line
        file.write_byte codes[line]
      elsif labels.has_key? line
        file.write_byte labels[line]
      elsif line.hexbytes?
        file.write_byte line.hexbytes[0]
      else
        puts "Error on line #{idx}: #{line} not recognized"
      end
    end
    file.close
  end
end

atebit = Atebit.new
atebit.compile
