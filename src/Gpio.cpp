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

#include <stdio.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <poll.h>

#include <algorithm>
#include <iostream>
#include <cstring>

#include "libgpio/Gpio.h"

using namespace ::std;
using namespace ::com::github::cfriedt;

#ifndef ARRAY_SIZE
#define ARRAY_SIZE( x ) ( sizeof( x ) / sizeof( (x)[ 0 ] ))
#endif

Gpio::Gpio( unsigned num, gpio_value_t value )
:
	gpio_num( num ),
	gpio_edge( GPIO_EDGE_NONE ),
	sys_class_gpio_gpio_n_value_fd( -1 ),
	interruptee_fd( -1 ),
	interruptor_fd( -1 )
{
	int r;
	gpio_direction_t direction;

	export_();

	direction = GPIO_DIR_OUT;
	r = gpio_direction_set( num, & direction );
	if ( -1 == r ) {
		throw std::system_error( errno, std::system_category() );
	}

	r = gpio_value_set( num, &value );
	if ( -1 == r ) {
		throw std::system_error( errno, std::system_category() );
	}
}

Gpio::Gpio( unsigned num, gpio_edge_t edge )
:
	gpio_num( num ),
	gpio_edge( edge ),
	sys_class_gpio_gpio_n_value_fd( -1 ),
	interruptee_fd( -1 ),
	interruptor_fd( -1 )
{
	int r;
	gpio_direction_t direction;

	if ( ! gpio_is_exported( num ) ) {
		r = gpio_export( num );
		if ( -1 == r ) {
			throw std::system_error( errno, std::system_category() );
		}
	}

	direction = GPIO_DIR_IN;
	r = gpio_direction_set( num, & direction );
	if ( -1 == r ) {
		throw std::system_error( errno, std::system_category() );
	}

	r = gpio_edge_set( num, & edge );
	if ( -1 == r ) {
		throw std::system_error( errno, std::system_category() );
	}
}

Gpio::Gpio( unsigned num )
:
	gpio_num( num ),
	gpio_edge( GPIO_EDGE_NONE ),
	sys_class_gpio_gpio_n_value_fd( -1 ),
	interruptee_fd( -1 ),
	interruptor_fd( -1 )
{
	int r;

	gpio_direction_t direction;

	export_();

	direction = GPIO_DIR_IN;
	r = gpio_direction_set( num, & direction );
	if ( -1 == r ) {
		throw std::system_error( errno, std::system_category() );
	}

	r = gpio_edge_set( num, & gpio_edge );
	if ( -1 == r ) {
		throw std::system_error( errno, std::system_category() );
	}
}

Gpio::Gpio()
:
	gpio_num( -1 ),
	gpio_edge( GPIO_EDGE_NONE ),
	sys_class_gpio_gpio_n_value_fd( -1 ),
	interruptee_fd( -1 ),
	interruptor_fd( -1 )
{
}

Gpio::~Gpio() {

	interrupt();

	// TODO: @CF: Add flag => should_unexport_in_dtor ?

	close_fds();
}

uint16_t Gpio::num() {
	return gpio_num;
}

gpio_value_t Gpio::value() {
	gpio_value_t r;
	int rr;

	export_();

	rr = gpio_value_get( gpio_num, &r );
	if ( -1 == rr ) {
		throw std::system_error( errno, std::system_category() );
	}

	return r;
}
void Gpio::value( gpio_value_t value ) {
	int r;

	export_();

	r = gpio_value_set( gpio_num, &value );
	if ( -1 == r ) {
		throw std::system_error( errno, std::system_category() );
	}
}

gpio_direction_t Gpio::direction() {
	gpio_direction_t r;
	int rr;

	export_();

	rr = gpio_direction_get( gpio_num, &r );
	if ( -1 == rr ) {
		throw std::system_error( errno, std::system_category() );
	}

	return r;
}

void Gpio::direction( gpio_direction_t direction ) {
	int r;

	export_();

	r = gpio_direction_set( gpio_num, &direction );
	if ( -1 == r ) {
		throw std::system_error( errno, std::system_category() );
	}
}

gpio_edge_t Gpio::edge() {
	gpio_edge_t r;
	int rr;

	export_();

	rr = gpio_edge_get( gpio_num, &r );
	if ( -1 == rr ) {
		throw std::system_error( errno, std::system_category() );
	}

	return r;
}
void Gpio::edge( gpio_edge_t edge ) {
	int r;

	export_();

	gpio_edge = edge;

	r = gpio_edge_set( gpio_num, &edge );
	if ( -1 == r ) {
		throw std::system_error( errno, std::system_category() );
	}
}

void Gpio::wait() {
	wait( -1 );
}
void Gpio::wait( uint16_t ms ) {

	enum {
		INTERRUPTEE,
		INTERRUPTOR,
	};

	int r;
	int sv[2];
	char path[ 64 ];

	struct pollfd pollfd[2];

	export_();

	r = socketpair( AF_UNIX, SOCK_STREAM, 0, sv );
	if ( -1 == r ) {
		close_fds();
		throw std::system_error( errno, std::system_category() );
	}

	interruptee_fd = sv[ INTERRUPTEE ];
	interruptor_fd = sv[ INTERRUPTOR ];

	memset( path, 0, sizeof( path ) );
	snprintf( path, sizeof( path ) - 1, "/sys/class/gpio/gpio%u/value", gpio_num );

	r = open( path, O_RDWR );
	if ( -1 == r ) {
		close_fds();
		throw std::system_error( errno, std::system_category() );
	}
	sys_class_gpio_gpio_n_value_fd = r;

	r = read( sys_class_gpio_gpio_n_value_fd, path, sizeof( path ) );
	if ( -1 == r ) {
		close_fds();
		throw std::system_error( errno, std::system_category() );
	}

	pollfd[ 0 ].fd = sys_class_gpio_gpio_n_value_fd;
	pollfd[ 0 ].events = POLLPRI;

	pollfd[ 1 ].fd = interruptee_fd;
	pollfd[ 1 ].events = POLLIN;

	r = poll( pollfd, ARRAY_SIZE( pollfd ), ms );
	if ( -1 == r ) {
		close_fds();
		throw std::system_error( errno, std::system_category() );
	}
	if ( 0 == r ) {
		close_fds();
		errno = ETIMEDOUT;
		throw std::system_error( errno, std::system_category() );
	}
	if ( pollfd[ 1 ].revents & POLLIN ) {
		close_fds();
		errno = EINTR;
		throw std::system_error( errno, std::system_category() );
	}
	if ( pollfd[ 0 ].revents & POLLPRI ) {
		// received gpio interrupt
	}
	close_fds();
}

void Gpio::interrupt() {
	const char *foo = "!";
	if ( -1 != interruptor_fd ) {
		write( interruptor_fd, foo, strlen( foo ) );
	}
}

void Gpio::close_fds() {
	if ( -1 != interruptee_fd ) {
		close( interruptee_fd );
		interruptee_fd = -1;
	}
	if ( -1 != interruptor_fd ) {
		close( interruptor_fd );
		interruptor_fd = -1;
	}
	if ( -1 != sys_class_gpio_gpio_n_value_fd ) {
		close( sys_class_gpio_gpio_n_value_fd );
		sys_class_gpio_gpio_n_value_fd = -1;
	}
}

bool Gpio::is_exported() {
	return gpio_is_exported( gpio_num );
}

void Gpio::export_() {
	int r;
	if ( ! is_exported() ) {
		r = gpio_export( gpio_num );
		if ( -1 == r ) {
			throw std::system_error( errno, std::system_category() );
		}
		if ( GPIO_EDGE_NONE != gpio_edge ) {
			edge( gpio_edge );
		}
	}
}

void Gpio::unexport() {
	int r;
	if ( is_exported() ) {
		if ( GPIO_EDGE_NONE != edge() ) {
			edge( GPIO_EDGE_NONE );
		}
		r = gpio_unexport( gpio_num );
		if ( -1 == r ) {
			throw std::system_error( errno, std::system_category() );
		}
	}
}
