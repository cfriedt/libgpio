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

#ifndef com_github_cfriedt_Gpio_h_
#define com_github_cfriedt_Gpio_h_

#include "libgpio/libgpio.h"

namespace com {
namespace github {
namespace cfriedt {

class Gpio {

public:
	/**
	 * @brief Initialize a GPIO for output and set its value.
	 *
	 * @param num    the GPIO number
	 * @param value  the value to be set
	 */
	Gpio( unsigned num, gpio_value_t value );
	/**
	 * @brief Initialize an GPIO for input with a specific kind of interrupt
	 *
	 * @param num    the GPIO number
	 * @param value  the interrupt type
	 */
	Gpio( unsigned num, gpio_edge_t edge );
	/**
	 * @brief Initialize a GPIO for input
	 *
	 * @param num    the GPIO number
	 */
	Gpio( unsigned num );
	/**
	 * @brief Allocate a GPIO object
	 */
	Gpio();
	virtual ~Gpio();

	/**
	 * @brief The GPIO number
	 * @return the GPIO number
	 */
	uint16_t num();

	/**
	 * @brief Get the value of the GPIO
	 * @return the value of the GPIO
	 */
	gpio_value_t value();
	/**
	 * @brief Set the value of the GPIO
	 * @param value the value to use
	 */
	void value( gpio_value_t value );

	/**
	 * @brief Get the direction of the GPIO
	 * @return the direction of the GPIO
	 */
	gpio_direction_t direction();
	/**
	 * @brief Set the direction of the GPIO
	 * @param direction the direction to use
	 */
	void direction( gpio_direction_t direction );

	/**
	 * @brief Get the edge of the GPIO
	 * @return the edge of the GPIO
	 */
	gpio_edge_t edge();
	/**
	 * @brief Set the edge of the GPIO
	 * @param edge the edge to use
	 */
	void edge( gpio_edge_t edge );

	/**
	 * @brief Wait for an interrupt indefinitely
	 */
	void wait();

	/**
	 * @brief Wait for an interrupt
	 *
	 * @param ms max milliseconds to wait
	 */
	void wait( uint16_t ms );

	/**
	 * @brief Stop waiting for an interrupt
	 */
	void interrupt();

protected:

	uint16_t gpio_num;
	gpio_edge_t gpio_edge;

	int sys_class_gpio_gpio_n_value_fd;
	int interruptee_fd;
	int interruptor_fd;

	void close_fds();

	bool is_exported();
	void export_();
	void unexport();
};

}
}
} // com.github.cfriedt

#endif // com_github_cfriedt_Gpio_h_
