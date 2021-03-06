#if !defined(__cplusplus)
#include <stdbool.h>    //c doesnt have bools by default
#endif

#include <stddef.h>
#include <stdint.h>

//check if the compiler thinks were targeting the wrong OS
#if defined(__linux__)
#error "You are not using a cross compiler"
#endif

//will only work for 32-bit ix86 targets
#if !defined(__i386__)
#error "Please compile with a ix86-elf compiler"
#endif

//Hardware text mode color constants
enum vga_color {
    COLOR_BLACK = 0,
    COLOR_BLUE = 1,
    COLOR_GREEN = 2,
    COLOR_CYAN = 3,
    COLOR_RED = 4,
    COLOR_MAGENTA = 5,
    COLOR_BROWN = 6,
    COLOR_LIGHT_GREY = 7,
    COLOR_DARK_GREY = 8,
    COLOR_LIGHT_BLUE = 9,
    COLOR_LIGHT_GREEN = 10,
    COLOR_LIGHT_CYAN = 11,
    COLOR_LIGHT_RED = 12,
    COLOR_LIGHT_MAGENTA = 13,
    COLOR_LIGHT_BROWN = 14,
    COLOR_WHITE = 15,
};

//sets foreground color (text) and background color
uint8_t make_color (enum vga_color fg, enum vga_color bg){
    return fg | bg << 4;
}

//assigns colors to chars
uint16_t make_vgaentry (char c, uint8_t color){
    uint16_t c16 = c;
    uint16_t color16 = color;
    return c16 | color16 << 8;
}

//custom strlen func, std func not define in freestanding C
size_t strlen (const char* str){
    size_t ret = 0;
    while (str[ret] != 0)
        ret++;
    return ret;
}

//defining scree length and height in chars
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

void terminal_initialize(){
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
    terminal_buffer = (uint16_t*) 0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; y++){
        for (size_t x = 0; x < VGA_WIDTH; x++){
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = make_vgaentry(' ', terminal_color);
        }
    }
}

void terminal_setcolor (uint8_t color) {
    terminal_color = color;
}

void terminal_putentryat (char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = make_vgaentry(c, color);
}

void terminal_putchar (char c){
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH){
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_row = 0;
        }
    }
}

void terminal_writestring(const char* data) {
    size_t datalen = strlen(data);
    for (size_t i = 0; i < datalen; i++)
        terminal_putchar(data[i]);
}

#if defined(__cplusplus)
extern "C"  //use C linkage for kernel_main
#endif

void kernel_main() {
    //initialize the terminal interface
    terminal_initialize();

    //because there is no support for newlines in vga
    //the \n char will print a VGA specific char
    terminal_writestring("Hello, kernel World!\n");
}
