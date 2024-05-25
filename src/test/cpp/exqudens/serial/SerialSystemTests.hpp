#pragma once

#include <optional>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "TestLogging.hpp"
#include "TestUtils.hpp"
#include "exqudens/serial/Serial.hpp"

namespace exqudens {

  class SerialSystemTests : public testing::Test {

    protected:

      inline static const char* LOGGER_ID = "exqudens.SerialSystemTests";

  };

  TEST_F(SerialSystemTests, test1) {
    try {
      std::string testGroup = testing::UnitTest::GetInstance()->current_test_info()->test_suite_name();
      std::string testCase = testing::UnitTest::GetInstance()->current_test_info()->name();
      TEST_LOG_I(LOGGER_ID) << "'" << testGroup << "." << testCase << "' start";

      std::shared_ptr<ISerial> serial = std::make_shared<Serial>();

      std::optional<std::string> port = {};

      std::vector<std::map<std::string, std::string>> ports = serial->listPorts();
      for (const std::map<std::string, std::string>& entry : ports) {
        TEST_LOG_I(LOGGER_ID) << TestUtils::toString(entry);
        if (entry.contains("port") && entry.contains("hardware-id") && entry.at("hardware-id").starts_with("USB\\VID_0483&PID_5740")) {
          port.emplace(entry.at("port"));
        }
      }

      ASSERT_TRUE(port.has_value());
      ASSERT_FALSE(port.value().empty());

      serial->open(port.value(), 500);

      std::vector<unsigned char> bytes = serial->readBytes(5);
      size_t length = bytes.size();
      TEST_LOG_I(LOGGER_ID) << "received length: " << length;

      ASSERT_EQ(0, length);

      std::string data = "hello";
      bytes = std::vector<unsigned char>(data.begin(), data.end());
      length = serial->writeBytes(bytes);
      TEST_LOG_I(LOGGER_ID) << "sent length: " << length;

      ASSERT_EQ(5, length);

      bytes = serial->readBytes(length);
      data = std::string(bytes.begin(), bytes.end());
      TEST_LOG_I(LOGGER_ID) << "received: '" << data << "'";

      ASSERT_EQ(std::string("HELLO"), data);

      serial->close();

      TEST_LOG_I(LOGGER_ID) << "'" << testGroup << "." << testCase << "' end";
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }
}
