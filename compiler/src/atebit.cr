class Atebit
  VERSION = "0.1.0"
  property codes = {} of String => UInt8

  def initialize
    # opcodes for atebit computer
    self.codes = {
      "NOP"  => 0x00,
      "LDA#" => 0x01,
      "LDA"  => 0x02,
      "LDA*" => 0x03,
      "STA"  => 0x04,
      "STA*" => 0x05,
      "ADC#" => 0x06,
      "ADC"  => 0x07,
      "SBC#" => 0x08,
      "SBC"  => 0x09,
      "AND#" => 0x0A,
      "AND"  => 0x0B,
      "ORA#" => 0x0C,
      "ORA"  => 0x0D,
      "EOR#" => 0x0E,
      "EOR"  => 0x0F,
      "INC"  => 0x10,
      "DEC"  => 0x11,
      "CMP#" => 0x12,
      "CMP"  => 0x13,
      "BCC"  => 0x14,
      "BCS"  => 0x15,
      "BEQ"  => 0x16,
      "BNE"  => 0x17,
      "CLC"  => 0x18,
      "SEC"  => 0x19,
      "JMP"  => 0x1A,
      "INA"  => 0x1B,
      "OUT#" => 0x1C,
      "OUT"  => 0x1D,
      "OUT*" => 0x1E,
      "HLT"  => 0x1F,
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
