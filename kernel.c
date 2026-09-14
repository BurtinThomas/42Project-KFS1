#include <stddef.h>
#include <stdint.h>

#if !defined(__i386__)
#error "This needs to be compiled with an ix86-elf compiler"
#endif

enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_LIGHT_GREY = 7
};

static inline uint8_t compose_vga_color(enum vga_color foreground,
                                        enum vga_color background)
{
    return foreground | (background << 4);
}

static inline uint16_t compose_vga_entry(unsigned char character, uint8_t color)
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

void initialize_terminal(void)
{
    terminal_color = compose_vga_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

    for (size_t row = 0; row < VGA_HEIGHT; row++) {
        for (size_t column = 0; column < VGA_WIDTH; column++) {
            terminal_buffer[row * VGA_WIDTH + column] =
                compose_vga_entry(' ', terminal_color);
        }
    }
}
void write_terminal_character(char c)
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
        compose_vga_entry(c, terminal_color);

    terminal_column++;

    if (terminal_column == VGA_WIDTH) {
        terminal_column = 0;

        if (terminal_row + 1 == VGA_HEIGHT)
            terminal_row = 0;
        else
            terminal_row++;
    }
}

void write_terminal_string(const char* text)
{
    size_t i = 0;

    while (text[i] != '\0') {
        write_terminal_character(text[i]);
        i++;
    }
}

void kernel_entry(void)
{
    initialize_terminal();
    write_terminal_string("42\n");
}
