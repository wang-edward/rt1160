#!/usr/bin/env bash
set -e
ELF=${1:-build/rt1160_app.elf}
echo "== Entry =="; arm-none-eabi-readelf -h "$ELF" | grep 'Entry point'
echo "== Sections (top) =="; arm-none-eabi-objdump -h "$ELF" | head -n 40
echo "== ISR vector peek =="; arm-none-eabi-objdump -s -j .isr_vector "$ELF" | head || true
echo "== Program headers =="; arm-none-eabi-readelf -l "$ELF" | egrep 'Type|LOAD|R E|RW' -n
echo "== Undefined symbols =="; arm-none-eabi-objdump -t "$ELF" | grep ' UND ' && exit 1 || echo "OK: none"
echo "== Size =="; arm-none-eabi-size -A "$ELF" | egrep 'text|rodata|data|bss'
echo "Preflight done."
