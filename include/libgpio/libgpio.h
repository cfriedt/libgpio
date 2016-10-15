/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016, Christopher Friedt <chrisfriedt@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LIBGPIO_LIBGPIO_H_
#define LIBGPIO_LIBGPIO_H_

#include <sys/cdefs.h>

#include <stdint.h>
#include <stdbool.h>

__BEGIN_DECLS

typedef enum {
	GPIO_PROP_VALUE,
	GPIO_PROP_DIRECTION,
	GPIO_PROP_EDGE,
} gpio_prop_t;

typedef enum {
	GPIO_VALUE_LOW,
	GPIO_VALUE_HIGH,
} gpio_value_t;

typedef enum {
	GPIO_DIR_IN,
	GPIO_DIR_OUT,
} gpio_direction_t;

typedef enum {
	GPIO_EDGE_NONE,
	GPIO_EDGE_RISING,
	GPIO_EDGE_FALLING,
	GPIO_EDGE_BOTH,
} gpio_edge_t;

int gpio_direction_set( uint16_t gpio, gpio_direction_t *output );
int gpio_direction_get( uint16_t gpio, gpio_direction_t *output );

int gpio_value_set( uint16_t gpio, gpio_value_t *high );
int gpio_value_get( uint16_t gpio, gpio_value_t *high );

int gpio_edge_set( uint16_t gpio, gpio_edge_t *edge );
int gpio_edge_get( uint16_t gpio, gpio_edge_t *edge );

bool gpio_is_exported( uint16_t gpio );
int gpio_export( uint16_t gpio );
int gpio_unexport( uint16_t gpio );

__END_DECLS

#endif // LIBGPIO_LIBGPIO_H_
