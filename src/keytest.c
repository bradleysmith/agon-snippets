#include "agon/vdp_vdu.h"
#include <agon/vdp_key.h>
#include <mos_api.h>
#include <stdint.h>
#include <stdio.h>

#define LOG_ERR(E) fprintf(stderr, "Error in %s, line %d : %s\n", __FILE__, __LINE__, E)

char ascii_value_pressed = ' ';
static volatile uint8_t *keymap;

typedef enum _KeyName
{
    START_OF_KEY_CODE_COLLECTION = 0,
    K_LEFT = START_OF_KEY_CODE_COLLECTION,
    K_RIGHT,
    K_UP,
    K_DOWN,
    END_OF_KEY_CODE_COLLECTION
    
}KeyName;

typedef struct _KeyCode
{
    uint8_t m_offset;
    uint8_t m_bit;
} KeyCode;

KeyCode keycodes[4] = {
    {3, 2},
    {15, 2},
    {7, 2},
    {5, 2}
};

#define KEY_DOWN(KEY) *(keymap + keycodes[KEY].m_offset) & keycodes[KEY].m_bit ? 1: 0

/* Called whenever there's a keyboard event */
void key_ev_handler(KEY_EVENT key_event)
{
    /* Cursor to top left */
    putch(31); putch(0); putch(0);
    printf("Keyboard Tester\n");
    printf("---------------\n");
    /* Print key statuses */
    printf("LEFT  : %02x\n", KEY_DOWN(K_LEFT));
    printf("RIGHT : %02x\n", KEY_DOWN(K_RIGHT));
    printf("UP    : %02x\n", KEY_DOWN(K_UP));
    printf("DOWN  : %02x", KEY_DOWN(K_DOWN));

    // Stop when 'q' pressed
    ascii_value_pressed = key_event.ascii;
}

int main(int argc, char *argv[])
{
    keymap = mos_getkbmap();
    if(0 == keymap)
    {
        LOG_ERR("Failed to getkbmap\n");
        return -1;
    }

    if (-1 == vdp_key_init())
    {
        LOG_ERR("Failed to initialise keyboard!\n");
        return -1;
    }
    vdp_set_key_event_handler(&key_ev_handler);
    vdp_clear_screen();
    do
    {
        vdp_update_key_state();
        waitvblank();
    } while (ascii_value_pressed != 'q');
    /* Clean exit */
    vdp_key_reset_interrupt();
    vdp_clear_screen();
}
