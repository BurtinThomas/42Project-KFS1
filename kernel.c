#include <stddef.h>
#include <stdint.h>

#if !defined(__i386__)
#error "This needs to be compiled with an ix86-elf compiler"
#endif

static inline uint8_t compose_vga_color(uint8_t foreground,
                                        uint8_t background)
{
    return foreground | (background << 4);
}

static inline uint16_t compose_vga_entry(unsigned char character, uint8_t color)
{
    return (uint16_t)character | ((uint16_t)color << 8);
}


size_t terminal_row = 0;
size_t terminal_column = 0;
uint8_t terminal_color;
uint16_t* terminal_buffer = (uint16_t*)0xB8000;

void initialize_terminal(void)
{
    terminal_color = compose_vga_color(7, 0);

    for (size_t row = 0; row < 25; row++) {
        for (size_t column = 0; column < 80; column++) {
            terminal_buffer[row * 80 + column] =
                compose_vga_entry(' ', terminal_color);
        }
    }
}
void write_terminal_character(char c)
{
    if (c == '\n') {
        terminal_column = 0;

        if (terminal_row + 1 == 25)
            terminal_row = 0;
        else
            terminal_row++;

        return;
    }

    terminal_buffer[terminal_row * 80 + terminal_column] =
        compose_vga_entry(c, terminal_color);

    terminal_column++;

    if (terminal_column == 80) {
        terminal_column = 0;

        if (terminal_row + 1 == 25)
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

void main(void)
{
    initialize_terminal();
    write_terminal_string("42\n");
}
