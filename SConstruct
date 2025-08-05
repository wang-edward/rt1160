import os
import subprocess
from SCons.Script import Environment, Default, Alias

# Ensure build output dir exists
BUILD_DIR = 'build'
if not os.path.isdir(BUILD_DIR):
    os.makedirs(BUILD_DIR)

gcc = '/opt/homebrew/bin/arm-none-eabi-gcc-8.3.1'  # TODO make this cleaner

env = Environment(
    # Inherit your shell’s PATH so scons can find arm-none-eabi-gcc
    ENV       = os.environ,
    CC        = gcc,
    AS        = gcc,
    AR        = 'arm-none-eabi-ar',
    OBJCOPY   = 'arm-none-eabi-objcopy',
    # C compiler flags (no sysroot override)
    CFLAGS    = [
        '-mcpu=cortex-m7',
        '-mfloat-abi=hard',
        '-mfpu=fpv5-d16',
        '-O0', '-g3',
        '-ffunction-sections', '-fdata-sections'
    ],
    # Linker flags (no sysroot override)
    LINKFLAGS = [
        '-T', 'linker/evk1160.ld',
        '-nostartfiles',
        '-Wl,--gc-sections',
        '-g'
    ],
    # Where to look for your own headers
    CPPPATH   = [
        'sdk/devices',
        'sdk/drivers',
    ],
)

# Source files
sources = [
    'src/startup_MIMXRT1166_cm7.S',
    'src/main.c',
    'sdk/drivers/fsl_clock.c',
    'sdk/drivers/fsl_gpio.c',
    'sdk/drivers/fsl_common.c',    # must exist alongside fsl_common.h
]

# Build ELF
elf = env.Program(target=os.path.join(BUILD_DIR, 'rt1160_app.elf'),
                  source=sources)

# Also build a binary blob if you want
env.Command(os.path.join(BUILD_DIR, 'rt1160_app.bin'), elf,
            '$OBJCOPY -O binary $SOURCE $TARGET')

Default(elf)

# Flash alias
Alias('flash', None,
      'pyocd flash -t mimxrt1160 ' + os.path.join(BUILD_DIR, 'rt1160_app.elf'))

# GDB alias
Alias('gdb', None,
      'arm-none-eabi-gdb ' + os.path.join(BUILD_DIR, 'rt1160_app.elf')
      + ' -ex "target remote :3333"')
