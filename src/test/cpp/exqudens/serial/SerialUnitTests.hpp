#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "TestLogging.hpp"
#include "TestUtils.hpp"
#include "exqudens/serial/Serial.hpp"

namespace exqudens {

  class SerialUnitTests : public testing::Test {

    protected:

      inline static const char* LOGGER_ID = "exqudens.SerialUnitTests";

  };

  TEST_F(SerialUnitTests, test1) {
    try {
      std::string testGroup = testing::UnitTest::GetInstance()->current_test_info()->test_suite_name();
      std::string testCase = testing::UnitTest::GetInstance()->current_test_info()->name();
      TEST_LOG_I(LOGGER_ID) << "'" << testGroup << "." << testCase << "' start";

      std::shared_ptr<ISerial> serial = std::make_shared<Serial>();

      std::vector<std::map<std::string, std::string>> ports = serial->listPorts();
      for (const std::map<std::string, std::string>& port : ports) {
        TEST_LOG_I(LOGGER_ID) << TestUtils::toString(port);
      }

      TEST_LOG_I(LOGGER_ID) << "'" << testGroup << "." << testCase << "' end";
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

}
