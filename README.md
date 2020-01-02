# A bootloader for STM32F103-based keyboards

Based on [STM32duino-bootloader](https://github.com/rogerclarkmelbourne/STM32duino-bootloader).

## Compiling

This projects can be compiled either with GCC or with Clang. Clang is known to produce smaller binaries (by about ~100 bytes), however you need a fairly recent version.

### GCC

```
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain/gcc.cmake ..
make -j8
```

### Clang

```
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain/clang.cmake ..
make -j8
```

## Flashing the bootloader

The bootloader is flashed with [stlink](https://github.com/texane/stlink). You need an external SWD programmer to flash the bootloader; I'm using a $2 st-link clone which works well enough. It looks like this:

![](/img/stlink.jpg)

To program the board, connect the adapter to SWD pads and execute:

```
sudo st-flash erase
sudo st-flash write /path/to/your/bootloader-your_keyboard.bin 0x8000000
```

After the device reboots, it should jump into the bootloader. You should see something similar to:

```
[4543676.396262] usb 2-1.5.3.4: new full-speed USB device number 78 using ehci-pci
[4543676.505971] usb 2-1.5.3.4: New USB device found, idVendor=6b62, idProduct=0000, bcdDevice= 2.01
[4543676.505976] usb 2-1.5.3.4: New USB device strings: Mfr=1, Product=1, SerialNumber=1
[4543676.505978] usb 2-1.5.3.4: Product: DFU
[4543676.505981] usb 2-1.5.3.4: Manufacturer: DFU
[4543676.505983] usb 2-1.5.3.4: SerialNumber: DFU
```

## Entering the bootloader

The bootloader is entered when any of the following is true during power up:

- A bootloader entry flag is set - this would be set by QMK when you press the `RESET` keycode.
- There is no firmware uploaded yet - this works by checking the validity of the reset vector (only checks the stack pointer)
- A button is pressed (inherited from the parent project, but not really used)
- A button from the keyboard matrix is pressed - this should be the main method used to enter the bootloader.

The last method is new to this fork. It enters the bootloader if a keyboard button is held during power up, i.e. a user holds a specific button while plugging in the keyboard. This is safer compared to QMK bootmagic feature as it does not rely on a valid firmware being present.

The specific button to be held will be different depending on the keyboard. See below on how to configure it.

## Flashing keyboard firmware

The QMK firmware can be flashed with `dfu-util`. This bootloader uses a different vendor and product IDs; it also doesn't implement multiple alternate settings present in the parent project. The `DFU_ARGS` you want to use in QMK are as follows:

```
DFU_ARGS = -d 6b62:0000 -R
```

Check out this sample keyboard for more information: https://github.com/xyzz/qmk_firmware/tree/basicpad/keyboards/handwired/basicpad

## Adding a new keyboard

To add a new keyboard, first edit `CMakeLists.txt` and append a new `add_bootloader()` entry at the end, e.g.

```
add_bootloader(new_keyboard)
```

Then, edit `src/config.h` and before the `#else` with the `#error` branch, add:

```
#elif defined(TARGET_NEW_KEYBOARD)
/* Configuration options - see other entries for a reference */
```

Finally, recompile the project and you should see your very own `bootloader-new_keyboard.bin` in the build folder. See flashing instructions above for how to flash.

Before releasing your product, you should configure a way to enter the bootloader. The recommended way is by holding a keyboard matrix button on power up. Consider this schematic:

![](/img/schematic.png)

Suppose you want to enter the bootloader when K_4, the middle button in the matrix is held during power up. To scan that single keycode, we need the bootloader to output on `col1` (PB10) and read the result from `row1` (PA4). The following snippet from `config.h` properly sets it up:

```
#elif defined(TARGET_BASICPAD_V1)
    /* Middle key in the 3x3 keymatrix */
    #define BL_OUTPUT_BANK GPIOB
    #define BL_OUTPUT_PIN 10
    #define BL_INPUT_BANK GPIOA
    #define BL_INPUT_PIN 4
```
