#include <stddef.h>
#include <stdint.h>

#if !defined(__i386__)
#error "This needs to be compiled with an ix86-elf compiler"
#endif

/**
 * @brief Combine a foreground and background color into a VGA color attribute byte.
 *
 * @param foreground Foreground color (low nibble).
 * @param background Background color (high nibble).
 * @return The combined VGA color attribute byte.
 */
static inline uint8_t compose_vga_color(uint8_t foreground,
                                        uint8_t background)
{
    return foreground | (background << 4);
}

/**
 * @brief Build a VGA text-mode entry (character + color attribute).
 *
 * @param character Character to display.
 * @param color VGA color attribute byte, as produced by compose_vga_color().
 * @return A 16-bit VGA entry ready to be written to video memory.
 */
static inline uint16_t compose_vga_entry(unsigned char character, uint8_t color)
{
    return (uint16_t)character | ((uint16_t)color << 8);
}


/** @brief Current cursor row in the terminal. */
size_t terminal_row = 0;
/** @brief Current cursor column in the terminal. */
size_t terminal_column = 0;
/** @brief Current VGA color attribute used for writing characters. */
uint8_t terminal_color;
/** @brief Pointer to the VGA text-mode video memory buffer. */
uint16_t* terminal_buffer = (uint16_t*)0xB8000;

/**
 * @brief Initialize the terminal state and clear the screen.
 *
 * Sets the default color and fills the entire VGA buffer with
 * blank (space) entries.
 */
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

/**
 * @brief Write a character at the current cursor position and advance the cursor.
 *
 * Handles newline characters by moving to the start of the next row,
 * and wraps both column and row when reaching the edges of the screen
 * (row wraps back to 0, effectively overwriting the top of the screen).
 *
 * @param c Character to write.
 */
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

/**
 * @brief Write a null-terminated string to the terminal.
 *
 * @param text Null-terminated string to write.
 */
void write_terminal_string(const char* text)
{
    size_t i = 0;

    while (text[i] != '\0') {
        write_terminal_character(text[i]);
        i++;
    }
}

/**
 * @brief Kernel entry point.
 *
 * Initializes the terminal and prints a startup message.
 */
void main(void)
{
    initialize_terminal();
    write_terminal_string("42\n");
}
