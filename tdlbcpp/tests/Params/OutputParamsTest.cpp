//
//  OutputParamsTest.cpp
//  OutputParamsTest
//
//  Unit tests for OutputParams
//

#include <cstdio>
#include <cstdlib>
#include <random>

#include "gtest/gtest.h"

#include "Header.h"
#include "Params/OutputParams.hpp"

#include "tdlbcpp/tests/utils.hpp"
#include "tdlbcpp/tests/Params/ParamsCommon.hpp"

class OutputParamsTests : public ::testing::Test
{
protected:
    std::string filename;

public:
    OutputParamsTests()
    {
        filename = TestUtils::getTempFilename();
    }
    ~OutputParamsTests()
    {
        TestUtils::removeTempFile(filename);
    }
};

TEST_F(OutputParamsTests, OutputParamsWriteReadValidTest)
{
    OutputParams outputParams = ParamsCommon::createOutputParamsFixed();

    outputParams.writeParamsToJsonFile(filename);
    std::cerr << filename << std::endl;

    OutputParams outputParamsRead("test2");
    outputParamsRead.getParamsFromJsonFile(filename);

    ParamsCommon::checkAllFields(outputParams, outputParamsRead);
}

TEST_F(OutputParamsTests, OutputParamsRandomWriteReadValidTest)
{
    OutputParams outputParams = ParamsCommon::createOutputParamsRandom();

    outputParams.writeParamsToJsonFile(filename);
    std::cerr << filename << std::endl;

    OutputParams outputParamsRead("test2");
    outputParamsRead.getParamsFromJsonFile(filename);

    ParamsCommon::checkAllFields(outputParams, outputParamsRead);
}

TEST_F(OutputParamsTests, OutputParamsInvalidTest)
{
    std::ofstream out(filename);
    out << "{\"XY_planes\":[}";
    out.close();
    std::cerr << filename << std::endl;

    OutputParams outputParamsRead("test");
    TestUtils::captureStderr();
    outputParamsRead.getParamsFromJsonFile(filename);
    std::string capturedStdErr = TestUtils::getCapturedStderr();

    ASSERT_EQ(capturedStdErr, "Unhandled Exception reached parsing arguments: * Line 1, Column 15\n"
                              "  Syntax error: value, object or array expected.\n"
                              ", application will now exit\n")
        << "cerr should contain error";
}

TEST_F(OutputParamsTests, OutputParamsInvalidTestInvalidType)
{
    std::ofstream out(filename);
    out << "{\"XY_planes\":[{\"repeat\":\"invalidNumber\"}]}";
    out.close();
    std::cerr << filename << std::endl;

    OutputParams outputParamsRead("test");
    TestUtils::captureStderr();
    outputParamsRead.getParamsFromJsonFile(filename);
    std::string capturedStdErr = TestUtils::getCapturedStderr();

    ASSERT_EQ(capturedStdErr, "Unhandled Exception reached parsing arguments: "
                              "Value is not convertible to UInt64."
                              ", application will now exit\n")
        << "cerr should contain error";
}
