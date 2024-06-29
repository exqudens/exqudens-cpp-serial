/*!
* @file ISerial.hpp
*/

#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <map>
#include <functional>

#include "exqudens/serial/export.hpp"

namespace exqudens {

    class EXQUDENS_SERIAL_EXPORT ISerial {

        public:

            /*!
            * Sets log function.
            */
            EXQUDENS_SERIAL_INLINE
            virtual void setLogFunction(
                    const std::function<void(
                        const std::string& file,
                        const size_t& line,
                        const std::string& function,
                        const std::string& id,
                        const unsigned short& level,
                        const std::string& message
                    )>& value //!< A log function.
            ) = 0;

            /*!
            * Gets log function status.
            *
            * @return @b true if log function is set, @b false otherwise.
            */
            EXQUDENS_SERIAL_INLINE
            virtual bool isSetLogFunction() = 0;

            /*!
            * Gets library version.
            *
            * @return A library version.
            */
            EXQUDENS_SERIAL_INLINE
            virtual std::string getVersion() = 0;

            /*!
            * Lists the serial ports available on the system.
            *
            * @return An available serial ports, each represented by a map with keys: ["port", "description", "hardware-id"].
            *
            * @throws std::runtime_error
            */
            EXQUDENS_SERIAL_INLINE
            virtual std::vector<std::map<std::string, std::string>> listPorts() = 0;

            /*!
            * Opens the serial port connection with parameter(s).
            *
            * @throws std::runtime_error.
            */
            EXQUDENS_SERIAL_INLINE
            virtual void open(
                const std::string& port,                                        //!< A string reference containing the address of the serial port, which would be something like 'COM1' on Windows and '/dev/ttyS0' on Linux.
                const unsigned int& baudRate,                             //!< An integer that sets the baud rate for the serial port, default is 9600. Possible baud rates depends on the system but some safe baud rates include: 110, 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 56000, 57600, 115200. Some other baudrates that are supported by some comports: 128000, 153600, 230400, 256000, 460800, 921600.
                const unsigned int& timeoutInterByte,             //!< Number of milliseconds between bytes received to timeout on.
                const unsigned int& timeoutReadConstant,        //!< A constant number of milliseconds to wait after calling read.
                const unsigned int& timeoutReadMultiplier,    //!< A multiplier against the number of requested bytes to wait after calling read.
                const unsigned int& timeoutWriteConstant,     //!< A constant number of milliseconds to wait after calling write.
                const unsigned int& timeoutWriteMultiplier, //!< A multiplier against the number of requested bytes to wait after calling write.
                const unsigned int& biteSize,                             //!< Size of each byte in the serial transmission of data, default is 8, possible values are: 5, 6, 7, 8.
                const unsigned int& parity,                                 //!< Method of parity, default is 0-none, possible values are: 0-none, 1-odd, 2-even.
                const unsigned int& stopBits,                             //!< Number of stop bits used, default is 0-one, possible values are: 0-one, 1-one-point-five, 2-two.
                const unsigned int& flowControl                         //!< Type of flow control used, default is 0-none, possible values are: 0-none, 1-software, 2-hardware.
            ) = 0;

            /*!
            * Opens the serial port connection with parameter(s).
            *
            * @throws std::runtime_error.
            */
            EXQUDENS_SERIAL_INLINE
            virtual void open(
                const std::string& port,                    //!< A string reference containing the address of the serial port, which would be something like 'COM1' on Windows and '/dev/ttyS0' on Linux.
                const unsigned int& timeoutSimple //!< A value that defines the time in milliseconds until a timeout occurs after a call to read or write is made.
            ) = 0;

            /*!
            * Opens the serial port connection with parameter(s).
            *
            * @throws std::runtime_error.
            */
            EXQUDENS_SERIAL_INLINE
            virtual void open(
                const std::string& port //!< A string reference containing the address of the serial port, which would be something like 'COM1' on Windows and '/dev/ttyS0' on Linux.
            ) = 0;

            /*!
            * Gets the open status of the serial port.
            *
            * @return Returns @b true if the port is open, @b false otherwise.
            */
            EXQUDENS_SERIAL_INLINE
            virtual bool isOpen() = 0;

            /*!
            * Closes the serial port.
            */
            EXQUDENS_SERIAL_INLINE
            virtual void close() = 0;

            /*!
            * Write a bytes to the serial port.
            *
            * @return A size representing the number of bytes actually written to the serial port.
            *
            * @throws std::runtime_error.
            */
            EXQUDENS_SERIAL_INLINE
            virtual size_t writeBytes(
                const std::vector<unsigned char>& bytes //!< A const reference containing the data to be written to the serial port.
            ) = 0;

            /*!
            * Read a given amount of bytes from the serial port.
            *
            * @return A bytes buffer.
            *
            * @throws std::runtime_error.
            */
            EXQUDENS_SERIAL_INLINE
            virtual std::vector<unsigned char> readBytes(
                const size_t& size //!< A size defining how many bytes to be read.
            ) = 0;

            /*!
            * Destructor.
            */
            EXQUDENS_SERIAL_INLINE
            virtual ~ISerial() noexcept = default;

    };

}
