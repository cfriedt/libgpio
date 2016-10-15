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

#include <stdint.h>
#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <string.h>
#include <errno.h>

#include "libgpio/libgpio.h"

#ifndef min
#define min( a, b ) ( (a) < (b) ? (a) : (b) )
#endif // min

#define GPIO_PROP_DESC_NVALS_MAX 4

typedef struct {
	gpio_prop_t type;
	const char *type_str;
	unsigned nvals;
	const char *val[ GPIO_PROP_DESC_NVALS_MAX ];
} gpio_prop_desc_t;

static const gpio_prop_desc_t gpio_desc[] = {
	[ GPIO_PROP_VALUE ] = {
		.type = GPIO_PROP_VALUE,
		.type_str = "value",
		.nvals = 2,
		.val = {
			"0",
			"1",
		},
	},
	[ GPIO_PROP_DIRECTION ] = {
		.type = GPIO_PROP_DIRECTION,
		.type_str = "direction",
		.nvals = 2,
		.val = {
			"in",
			"out",
		},
	},
	[ GPIO_PROP_EDGE ] = {
		.type = GPIO_PROP_EDGE,
		.type_str = "edge",
		.nvals = 4,
		.val = {
			"none",
			"rising",
			"falling",
			"both",
		},
	},
};

static int gpio_prop( uint16_t gpio, gpio_prop_t prop, unsigned *eval, bool set ) {
	int r;

	char sys_class_gpio_gpioN_prop_fn[ 64 ];
	char prop_str_buf[ 16 ];
	int fd;
	int i;

	memset( sys_class_gpio_gpioN_prop_fn, 0, sizeof( sys_class_gpio_gpioN_prop_fn ) );
	snprintf( sys_class_gpio_gpioN_prop_fn, sizeof( sys_class_gpio_gpioN_prop_fn ) - 1,
		"/sys/class/gpio/gpio%u/%s",
		gpio,
		gpio_desc[ prop ].type_str
	);

	r = open( sys_class_gpio_gpioN_prop_fn, O_RDWR );
	if ( -1 == r ) {
		goto out;
	}
	fd = r;

	if ( set ) {
		if ( *eval >= gpio_desc[ prop ].nvals ) {
			errno = EINVAL;
			r = -1;
			goto closefd;
		}
		r = write( fd, gpio_desc[ prop ].val[ *eval ], strlen( gpio_desc[ prop ].val[ *eval ] ) );
		if ( -1 == r ) {
			goto closefd;
		}
	} else {
		memset( prop_str_buf, 0, sizeof( prop_str_buf ) );
		r = read( fd, prop_str_buf, sizeof( prop_str_buf ) );
		if ( -1 == r ) {
			goto closefd;
		}
		if ( '\n' == prop_str_buf[ r - 1 ] ) {
			prop_str_buf[ r - 1 ] = '\0';
			r--;
		}
		for( i = 0; i < gpio_desc[ prop ].nvals; i++ ) {
			if ( 0 == strncmp( gpio_desc[ prop ].val[ i ], prop_str_buf, min( strlen( gpio_desc[ prop ].val[ i ] ), r ) ) ) {
				*eval = i;
				break;
			}
		}
		if ( i >= gpio_desc[ prop ].nvals ) {
			r = -1;
			errno = EINVAL;
			goto closefd;
		}
	}

	r = EXIT_SUCCESS;

closefd:
	close( fd );

out:
	return r;
}
int gpio_direction_set( uint16_t gpio, gpio_direction_t *output ) {
	return gpio_prop( gpio, GPIO_PROP_DIRECTION, output, true );
}
int gpio_direction_get( uint16_t gpio, gpio_direction_t *output ) {
	return gpio_prop( gpio, GPIO_PROP_DIRECTION, output, false );
}
int gpio_value_set( uint16_t gpio, gpio_value_t *high ) {
	return gpio_prop( gpio, GPIO_PROP_VALUE, high, true );
}
int gpio_value_get( uint16_t gpio, gpio_value_t *high ) {
	return gpio_prop( gpio, GPIO_PROP_VALUE, high, false );
}
int gpio_edge_set( uint16_t gpio, gpio_edge_t *edge ) {
	return gpio_prop( gpio, GPIO_PROP_EDGE, edge, true );
}
int gpio_edge_get( uint16_t gpio, gpio_edge_t *edge ) {
	return gpio_prop( gpio, GPIO_PROP_EDGE, edge, false );
}

static int gpio_ex_unex_port( uint16_t gpio, bool ex ) {
	int r;
	int fd;

	const char *sys_class_gpio_export = "/sys/class/gpio/export";
	const char *sys_class_gpio_unexport = "/sys/class/gpio/unexport";
	char buf[ 16 ];

	r = open( ex ? sys_class_gpio_export : sys_class_gpio_unexport, O_WRONLY );
	if ( -1 == r ) {
		goto out;
	}
	fd = r;

	memset( buf, 0, sizeof( buf ) );
	snprintf( buf, sizeof( buf ) - 1, "%u", gpio );
	r = write( fd, buf, strlen( buf ) );
	if ( -1 == r ) {
		goto closefd;
	}

	r = EXIT_SUCCESS;

closefd:
	close( fd );
	fd = -1;

out:
	return r;
}
bool gpio_is_exported( uint16_t gpio ) {
	bool r;

	char sys_class_gpio_gpioN[ 32 ];
	int access_r;

	memset( sys_class_gpio_gpioN, 0, sizeof( sys_class_gpio_gpioN ) );
	snprintf( sys_class_gpio_gpioN, sizeof( sys_class_gpio_gpioN ) - 1, "/sys/class/gpio/gpio%u", gpio );

	access_r = access( sys_class_gpio_gpioN, F_OK );
	r = EXIT_SUCCESS == access_r;

	return r;
}
int gpio_export( uint16_t gpio ) {
	return gpio_ex_unex_port( gpio, true );
}
int gpio_unexport( uint16_t gpio ) {
	return gpio_ex_unex_port( gpio, false );
}
