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

#include <stdlib.h> // EXIT_SUCCESS, getenv(3), atoi(3)
#include <sys/utsname.h> // uname(3)

#include <gtest/gtest.h>

#include "libgpio/Gpio.h"

using namespace ::std;
using namespace ::com::github::cfriedt;

#define OSPASS_() \
	if ( ospass ) { \
		std::cout << "skipping test because this platform is not 'Linux'" << std::endl; \
		return; \
	} \

class GpioTest : public testing::Test
{

public:

	int gpio_num;
	bool ospass;
	Gpio gpio;

	GpioTest();
	~GpioTest();

	void SetUp();
	void TearDown();
};

GpioTest::GpioTest()
: gpio_num( 0 ), ospass( false )
{
}

GpioTest::~GpioTest() {
}

void GpioTest::SetUp() {
	struct utsname utsname;
	int rr;
	char *gpionum_env;

	gpionum_env = getenv( "LIBGPIO_TEST_GPIO_NUM" );
	if ( NULL != gpionum_env ) {
		gpio_num = atoi( gpionum_env );
	}

	rr = uname( & utsname );
	ASSERT_EQ( EXIT_SUCCESS, rr );
	ospass = 0 != strcmp( "Linux", utsname.sysname );
	if ( ! ospass ) {
		gpio = Gpio( gpio_num );
	}
}

void GpioTest::TearDown() {
}

TEST_F( GpioTest, TestGpioIsExported ) {
	OSPASS_();

	bool expected_bool;
	bool actual_bool;

	expected_bool = true;
	actual_bool = gpio_is_exported( gpio.num() );
	EXPECT_EQ( expected_bool, actual_bool );

	expected_bool = false;
	actual_bool = gpio_is_exported( gpio.num() + 2 );
	EXPECT_EQ( expected_bool, actual_bool );
}

TEST_F( GpioTest, TestValueGet ) {
	OSPASS_();
	gpio_value_t v = gpio.value();
	EXPECT_TRUE( GPIO_VALUE_LOW == v || GPIO_VALUE_HIGH == v );
}

TEST_F( GpioTest, TestDirectionGet ) {
	OSPASS_();
	gpio_direction_t v = gpio.direction();
	EXPECT_TRUE( GPIO_DIR_IN == v || GPIO_DIR_OUT == v );
}

TEST_F( GpioTest, TestEdgeGet ) {
	OSPASS_();
	gpio_edge_t v = gpio.edge();
	EXPECT_TRUE( GPIO_EDGE_NONE == v || GPIO_EDGE_RISING == v || GPIO_EDGE_FALLING == v || GPIO_EDGE_BOTH == v );
}

TEST_F( GpioTest, TestDirectionOut ) {
	OSPASS_();
	gpio_direction_t expected_gpio_direction_t = GPIO_DIR_OUT;
	gpio_direction_t actual_gpio_direction_t;
	gpio.direction( expected_gpio_direction_t );
	actual_gpio_direction_t = gpio.direction();
	EXPECT_EQ( expected_gpio_direction_t, actual_gpio_direction_t );
}

TEST_F( GpioTest, TestDirectionOutValueOne ) {
	OSPASS_();
	gpio.direction( GPIO_DIR_OUT );

	gpio_value_t expected_gpio_value_t = GPIO_VALUE_HIGH;
	gpio_value_t actual_gpio_value_t;
	gpio.value( expected_gpio_value_t );
	actual_gpio_value_t = gpio.value();
	EXPECT_EQ( expected_gpio_value_t, actual_gpio_value_t );
}

TEST_F( GpioTest, TestWaitForInterruptFail ) {
	OSPASS_();

/*
	gpio_edge_t expected_gpio_edge_t = GPIO_EDGE_BOTH;
	gpio_edge_t actual_gpio_edge_t;
	gpio.edge( expected_gpio_edge_t );
	actual_gpio_edge_t = gpio.edge();
	EXPECT_EQ( expected_gpio_edge_t, actual_gpio_edge_t );

	gpio.wait( 3000 );
*/

	int expected_errno;
	int actual_errno;

	actual_errno = EXIT_SUCCESS;
//	expected_errno = EINTR;
	expected_errno = ETIMEDOUT;
	try {
		gpio.wait( 1 );
	} catch( std::system_error &e ) {
		actual_errno = e.code().value();
	}
	EXPECT_EQ( expected_errno, actual_errno );
}
