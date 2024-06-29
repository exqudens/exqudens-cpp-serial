/*!
* @file Serial.cpp
*/

#include <limits>
#include <filesystem>
#include <memory>
#include <stdexcept>

#include "exqudens/serial/Serial.hpp"
#include "exqudens/serial/versions.hpp"

#define CALL_INFO std::string(__FUNCTION__) + "(" + std::filesystem::path(__FILE__).filename().string() + ":" + std::to_string(__LINE__) + ")"
#define LOGGER_ID "exqudens.Serial"
#define LOGGER_LEVEL_ERROR 2
#define LOGGER_LEVEL_DEBUG 5

namespace exqudens {

    Serial::Serial(const bool& autoClose): autoClose(autoClose) {}

    Serial::Serial(): Serial(true) {}

    void Serial::setLogFunction(
        const std::function<void(
            const std::string& file,
            const size_t& line,
            const std::string& function,
            const std::string& id,
            const unsigned short& level,
            const std::string& message
        )>& value
    ) {
        logFunction = value;
    }

    bool Serial::isSetLogFunction() {
        return (bool) logFunction;
    }

    std::string Serial::getVersion() {
        return std::to_string(PROJECT_VERSION_MAJOR) + "." + std::to_string(PROJECT_VERSION_MINOR) + "." + std::to_string(PROJECT_VERSION_PATCH);
    }

    std::vector<std::map<std::string, std::string>> Serial::listPorts() {
        try {
            std::vector<std::map<std::string, std::string>> results;
            std::vector<serial::PortInfo> portInfos = serial::list_ports();

            for (const serial::PortInfo& portInfo : portInfos) {
                std::map<std::string, std::string> result;
                result.insert({"port", portInfo.port});
                result.insert({"description", portInfo.description});
                result.insert({"hardware-id", portInfo.hardware_id});

                if (logFunction) {
                    std::string message = "{";
                    message += "'port': '" + result.at("port") + "', ";
                    message += "'description': '" + result.at("description") + "', ";
                    message += "'hardware-id': '" + result.at("hardware_id") + "'";
                    message += "}";
                }

                results.emplace_back(result);
            }

            return results;
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

    void Serial::open(
        const std::string& port,
        const unsigned int& baudRate,
        const unsigned int& timeoutInterByte,
        const unsigned int& timeoutReadConstant,
        const unsigned int& timeoutReadMultiplier,
        const unsigned int& timeoutWriteConstant,
        const unsigned int& timeoutWriteMultiplier,
        const unsigned int& biteSize,
        const unsigned int& parity,
        const unsigned int& stopBits,
        const unsigned int& flowControl
    ) {
        try {
            log(__FILE__, __LINE__, __FUNCTION__, LOGGER_ID, LOGGER_LEVEL_DEBUG, "port: '" + port + "'");

            serial::bytesize_t internalBiteSize = serial::eightbits;

            if (biteSize == 8) {
                internalBiteSize = serial::eightbits;
            } else if (biteSize == 7) {
                internalBiteSize = serial::sevenbits;
            } else if (biteSize == 6) {
                internalBiteSize = serial::sixbits;
            } else if (biteSize == 5) {
                internalBiteSize = serial::fivebits;
            } else {
                throw std::invalid_argument("biteSize");
            }

            serial::parity_t internalParity = serial::parity_none;

            if (parity == 0) {
                internalParity = serial::parity_none;
            } else if (parity == 1) {
                internalParity = serial::parity_odd;
            } else if (parity == 2) {
                internalParity = serial::parity_even;
            } else {
                throw std::invalid_argument("parity");
            }

            serial::stopbits_t internalStopBits = serial::stopbits_one;

            if (stopBits == 0) {
                internalStopBits = serial::stopbits_one;
            } else if (stopBits == 1) {
                internalStopBits = serial::stopbits_one_point_five;
            } else if (stopBits == 2) {
                internalStopBits = serial::stopbits_two;
            } else {
                throw std::invalid_argument("stopBits");
            }

            serial::flowcontrol_t internalFlowControl = serial::flowcontrol_none;

            if (flowControl == 0) {
                internalFlowControl = serial::flowcontrol_none;
            } else if (flowControl == 1) {
                internalFlowControl = serial::flowcontrol_software;
            } else if (flowControl == 2) {
                internalFlowControl = serial::flowcontrol_hardware;
            } else {
                throw std::invalid_argument("flowControl");
            }

            object = std::make_unique<serial::Serial>(
                port,
                baudRate,
                serial::Timeout(timeoutInterByte, timeoutReadConstant, timeoutReadMultiplier, timeoutWriteConstant, timeoutWriteMultiplier),
                internalBiteSize,
                internalParity,
                internalStopBits,
                internalFlowControl
            );
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

    void Serial::open(const std::string& port, const unsigned int& timeoutSimple) {
        try {
            open(
                port,
                9600,
                serial::Timeout::max(),
                timeoutSimple,
                0,
                timeoutSimple,
                0,
                8,
                0,
                0,
                0
            );
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

    void Serial::open(const std::string& port) {
        try {
            open(
                port,
                9600,
                0,
                0,
                0,
                0,
                0,
                8,
                0,
                0,
                0
            );
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

    bool Serial::isOpen() {
        try {
            if (object) {
                return object->isOpen();
            }
            return false;
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

    void Serial::close() {
        try {
            if (object) {
                if (object->isOpen()) {
                    object->close();
                }
            }
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

    size_t Serial::writeBytes(const std::vector<unsigned char>& bytes) {
        try {
            if (!isOpen()) {
                throw std::runtime_error("device is not open");
            }
            size_t result = 0;
            result = object->write(bytes);
            return result;
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

    std::vector<unsigned char> Serial::readBytes(const size_t& size) {
        try {
            if (!isOpen()) {
                throw std::runtime_error("device is not open");
            }
            std::vector<unsigned char> result;
            object->read(result, size);
            return result;
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

    Serial::~Serial() noexcept {
        if (autoClose) {
            try {
                close();
            } catch (const std::exception& e) {
                log(__FILE__, __LINE__, __FUNCTION__, LOGGER_ID, LOGGER_LEVEL_ERROR, "Error in destructor on call function: 'close': '" + std::string(e.what()) + "'");
            } catch (...) {
                log(__FILE__, __LINE__, __FUNCTION__, LOGGER_ID, LOGGER_LEVEL_ERROR, "Unknown error in destructor on call function: 'close'");
            }
        }
    }

    void Serial::log(
        const std::string& file,
        const size_t& line,
        const std::string& function,
        const std::string& id,
        const unsigned short& level,
        const std::string& message
    ) {
        try {
            if (logFunction) {
                std::string internalFile = std::filesystem::path(file).filename().string();
                logFunction(internalFile, line, function, id, level, message);
            }
        } catch (...) {
            std::throw_with_nested(std::runtime_error(CALL_INFO));
        }
    }

}

#undef CALL_INFO
#undef LOGGER_ID
#undef LOGGER_LEVEL_ERROR
#undef LOGGER_LEVEL_DEBUG
