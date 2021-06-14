# atebit compiler

Simple Assembly to executable for the atebit computer

## Installation

Install Crystal 1.0
```
brew install crystal
```

Build the atebit compiler
```
shards build
```

## Usage

Write your assembly file.  This will use the 32 opcodes made available by the 256 byte computer.  You can find examples in the `/examples` folder.

Then compile the application:
```
bin/atebit ./examples/multiply.s
```

This will generate an a0.out binary file.  Every 64 bytes, another binary will be generated.  This helps with uploading the application since the Arduino has a limit of 64 byte buffer.

## Upload

To upload the application, we will use screen to connect to the Arduino:
```
screen /dev/tty.usbmodem14101 9600
```

At the menu select `u 00` to upload the first file.  You will need to connect to the `a0.out` for the bufferfile.

Now press Ctrl+a and type:
```
:bufferfile a0.out
```

Now press Ctrl+a and then <.  This will slurp in the file to the buffer.
Now paste the Ctrl+a and ] to upload the first binary.

Repeat these steps for `40`, `80`, and `C0` for the other 64 byte files if they exist.

Now you can perform the `upload` and paste in the bytecode for the application.

To run, press `r` and enter

## Kill screen

To exit screen, press Ctrl+a and k. Confirm with y to exit.

## Development

This is a minimalistic solution.  There is no error checking and the assembly code is laid out to match memory one-to-one.

## Contributing

1. Fork it (<https://github.com/your-github-user/atebit-compiler/fork>)
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create a new Pull Request

## Contributors

- [Dru Jensen](https://github.com/drujensen) - creator and maintainer
