#include <inttypes.h> // strtol(3)
#include <stdlib.h>   // strtol(3)
#include <limits.h>   // USHRT_MAX

#include <iostream>

#include "libgpio/Gpio.h"

using namespace ::std;
using namespace ::com::github::cfriedt;

int main( int argc, char *argv[] ) {
	int r;

	long l;
	Gpio gpio;
	int i;

	if ( 2 != argc ) {
		errno = EINVAL;
		throw std::system_error( errno, std::system_category() );
	}

	l = strtol( argv[ 1 ], NULL, 10 );
	if ( 0 == l && EXIT_SUCCESS != errno ) {
		throw std::system_error( errno, std::system_category() );
	}
	if ( l < 0 || l > USHRT_MAX ) {
		errno = ERANGE;
		throw std::system_error( errno, std::system_category() );
	}
	gpio = Gpio( (uint16_t) l, GPIO_EDGE_FALLING );

	for( ;; ) {
		std::cout << "waiting for an interrupt on GPIO " << gpio.num() << std::endl;
		gpio.wait();
		std::cout << "received for an interrupt on GPIO " << gpio.num() << std::endl;
	}

	return EXIT_SUCCESS;
}
