#include <stddef.h>
#include <stdint.h>

#if !defined(__i386__)
#error "This needs to be compiled with an ix86-elf compiler"
#endif

enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_LIGHT_GREY = 7
};

static inline uint8_t make_color(enum vga_color foreground,
                                 enum vga_color background)
{
    return foreground | (background << 4);
}

static inline uint16_t make_entry(unsigned char character, uint8_t color)
{
    return (uint16_t)character | ((uint16_t)color << 8);
}

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

size_t terminal_row = 0;
size_t terminal_column = 0;
uint8_t terminal_color;
uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;

void terminal_initialize(void)
{
    terminal_color = make_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

    for (size_t row = 0; row < VGA_HEIGHT; row++) {
        for (size_t column = 0; column < VGA_WIDTH; column++) {
            terminal_buffer[row * VGA_WIDTH + column] =
                make_entry(' ', terminal_color);
        }
    }
}

void terminal_putchar(char c)
{
    if (c == '\n') {
        terminal_column = 0;

        if (terminal_row + 1 == VGA_HEIGHT)
            terminal_row = 0;
        else
            terminal_row++;

        return;
    }

    terminal_buffer[terminal_row * VGA_WIDTH + terminal_column] =
        make_entry(c, terminal_color);

    terminal_column++;

    if (terminal_column == VGA_WIDTH) {
        terminal_column = 0;

        if (terminal_row + 1 == VGA_HEIGHT)
            terminal_row = 0;
        else
            terminal_row++;
    }
}

void terminal_writestring(const char* text)
{
    size_t i = 0;

    while (text[i] != '\0') {
        terminal_putchar(text[i]);
        i++;
    }
}

void kernel_main(void)
{
    terminal_initialize();
    terminal_writestring("42\n");
}
