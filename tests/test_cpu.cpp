#include <gtest/gtest.h>

extern "C" {
    #include "../ch8.h"
}

TEST(Chip8State, MemoryInit) {
    Chip8State cpu;
    chip8_init(&cpu);

    EXPECT_EQ(cpu.program_counter, 0x200);
}
