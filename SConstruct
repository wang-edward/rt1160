import os
from SCons.Script import Environment, Default, Alias

# Ensure build output dir exists
BUILD_DIR = 'build'
if not os.path.isdir(BUILD_DIR):
    os.makedirs(BUILD_DIR)

# Cross-compiler toolchain settings
env = Environment(
    CC        = 'arm-none-eabi-gcc',
    AS        = 'arm-none-eabi-gcc',
    AR        = 'arm-none-eabi-ar',
    OBJCOPY   = 'arm-none-eabi-objcopy',
    # C compiler flags
    CFLAGS    = [
        '-mcpu=cortex-m7',
        '-mfloat-abi=hard',
        '-mfpu=fpv5-d16',
        '-O0', '-g3',
        '-ffunction-sections', '-fdata-sections'
    ],
    # Linker flags
    LINKFLAGS = [
        '-T', 'linker/evk1160.ld',
        '-nostartfiles',
        '-Wl,--gc-sections',
        '-g'
    ],
    # Include paths for headers
    CPPPATH   = [
        'sdk/devices',
        'sdk/drivers'
    ],
)

# Source files
sources = [
    'src/startup_MIMXRT1166_cm7.S',
    'src/main.c',
    'sdk/drivers/fsl_clock.c',
    'sdk/drivers/fsl_gpio.c',
]

# Build the ELF and an optional binary
elf = env.Program(target=os.path.join(BUILD_DIR, 'rt1160_app.elf'),
                  source=sources)
env.Command(os.path.join(BUILD_DIR, 'rt1160_app.bin'), elf,
            '$OBJCOPY -O binary $SOURCE $TARGET')

# Default build target
Default(elf)

# Flash alias (pyOCD)
Alias('flash', None,
      'pyocd flash -t mimxrt1160 ' + os.path.join(BUILD_DIR, 'rt1160_app.elf'))

# GDB alias
Alias('gdb', None,
      'arm-none-eabi-gdb ' + os.path.join(BUILD_DIR, 'rt1160_app.elf') +
      ' -ex "target remote :3333"')
