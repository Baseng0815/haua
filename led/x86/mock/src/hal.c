#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <led/mock_strip.h>
#include <led/types.h>
#include <led/hal.h>

led_error_t led_hal_init(const void *config, struct strip *strip)
{
        const struct mock_strip_config *mock_config = config;

        printf("Initializing mock strip %s\n", mock_config->name);

        strip->drv = NULL;

        strip->state.on_off_state = LED_ON;
        strip->state.brightness = 1;
        strip->state.colors =
                malloc(sizeof(struct color_rgb) * mock_config->num_leds);

        strip->info.num_leds = mock_config->num_leds;
        strip->info.position.group = 0;
        strip->info.position.index = 0;

        strip->info.name = malloc(strlen(mock_config->name) + 1);
        strcpy(strip->info.name, mock_config->name);

        return LED_ERROR_OK;
}

led_error_t led_hal_show(struct strip *strip)
{
        const struct strip_info *info = &strip->info;
        unsigned int row = (unsigned int)info->position.group + 1;
        unsigned int col = (unsigned int)info->position.index * 2 + 1;
        size_t i;

        printf("\x1b[%u;%uH", row, col);
        for (i = 0; i < info->num_leds; i++) {
                const struct color_rgb *c = &strip->state.colors[i];

                printf("\x1b[38;2;%u;%u;%um██", (unsigned)c->r, (unsigned)c->g,
                       (unsigned)c->b);
        }
        fputs("\x1b[0m", stdout);
        fflush(stdout);

        return LED_ERROR_OK;
}

led_error_t led_hal_deinit(struct strip *strip)
{
        const struct mock_strip_config *mock_config = strip->drv;

        printf("Deinitializing mock strip %s\n", mock_config->name);

        free(strip->state.colors);
        strip->state.colors = NULL;

        return LED_ERROR_OK;
}
