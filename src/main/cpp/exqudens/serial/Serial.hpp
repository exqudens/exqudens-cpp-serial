/*!
* @file Serial.hpp
*/

#pragma once

#include <memory>

#include <serial/serial.h>

#include "exqudens/serial/ISerial.hpp"

namespace exqudens {

   class EXQUDENS_SERIAL_EXPORT Serial : public virtual ISerial {

      private:

         std::function<void(
            const std::string& file,
            const size_t& line,
            const std::string& function,
            const std::string& id,
            const unsigned short& level,
            const std::string& message
         )> logFunction;
         bool autoClose = false;
         std::unique_ptr<serial::Serial> object = nullptr;

      public:

         Serial(const bool& autoClose);
         Serial();

         void setLogFunction(
               const std::function<void(
                     const std::string&,
                     const size_t&,
                     const std::string&,
                     const std::string&,
                     const unsigned short&,
                     const std::string&
               )> &value
         ) override;

         bool isSetLogFunction() override;

         std::string getVersion() override;

         std::vector<std::map<std::string, std::string>> listPorts() override;

         void open(
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
         ) override;

         void open(const std::string& port, const unsigned int& timeoutSimple) override;

         void open(const std::string& port) override;

         bool isOpen() override;

         void close() override;

         size_t writeBytes(const std::vector<unsigned char>& bytes) override;

         std::vector<unsigned char> readBytes(const size_t& size) override;

         ~Serial() noexcept override;

      private:

         std::map<std::string, std::string> toMap(const serial::PortInfo& value);

         std::string normalize(const std::string& value);

         void log(
            const std::string& file,
            const size_t& line,
            const std::string& function,
            const std::string& id,
            const unsigned short& level,
            const std::string& message
         );

   };

}
