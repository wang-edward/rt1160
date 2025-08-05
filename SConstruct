import os
from SCons.Script import Environment, Default, Alias

BUILD_DIR = 'build'
os.makedirs(BUILD_DIR, exist_ok=True)

gcc = '/opt/homebrew/bin/arm-none-eabi-gcc-8.3.1'

env = Environment(
    ENV     = os.environ,
    CC      = gcc,
    AS      = gcc,
    AR      = 'arm-none-eabi-ar',
    OBJCOPY = 'arm-none-eabi-objcopy',
    CFLAGS  = [
        '-mcpu=cortex-m7','-mfloat-abi=hard','-mfpu=fpv5-d16',
        '-DCPU_MIMXRT1166DVM6A_cm7',
        '-O0','-g3','-ffunction-sections','-fdata-sections',
    ],
    LINKFLAGS = [
        '-T','linker/evk1160.ld','-nostartfiles','-Wl,--gc-sections','-g'
    ],
    CPPPATH = [
        'sdk/CMSIS/Core/Include',
        'sdk/devices/MIMXRT1166',                # entire device tree
        'sdk/devices/MIMXRT1166/periph',         # peripherals 
        'sdk/devices/MIMXRT1166/CMSIS/Include',  # core_cm7, etc.
        'sdk/devices/MIMXRT1166/gcc',            # for linker scripts if needed
        'sdk/drivers',                           # your fsl_*.h files
    ],
)

# Your application + only the drivers you need:
sources = [
    'src/startup_MIMXRT1166_cm7.S',
    'src/main.c',
    'sdk/drivers/fsl_clock.c',
    'sdk/drivers/fsl_pmu.c',
    'sdk/drivers/fsl_anatop_ai.c',
    'sdk/drivers/fsl_gpio.c',
    # then, for audio:
    # 'sdk/drivers/fsl_sai.c',
    # 'sdk/drivers/fsl_edma.c',
    # 'sdk/drivers/fsl_dmamux.c',
    # 'sdk/drivers/fsl_cache.c',
]

elf = env.Program(target=os.path.join(BUILD_DIR,'rt1160_app.elf'),
                  source=sources)
env.Command(os.path.join(BUILD_DIR,'rt1160_app.bin'), elf,
            '$OBJCOPY -O binary $SOURCE $TARGET')
Default(elf)

Alias('flash', None,
      f'pyocd flash -t mimxrt1160 {BUILD_DIR}/rt1160_app.elf')
Alias('gdb', None,
      f'arm-none-eabi-gdb {BUILD_DIR}/rt1160_app.elf -ex "target remote :3333"')
