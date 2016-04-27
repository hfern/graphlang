#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ParseTests
{
    TEST_CLASS(NumberTest)
    {
    public:
        TEST_METHOD(NotNumbers)
        {
            assertNotParses("");
            assertNotParses("$$$$$");
            assertNotParses("\"look a string!\"");
            assertNotParses("()");
            assertNotParses("test_identifier");
        }

        TEST_METHOD(NormalNumbers)
        {
            assertParsesTo("0", 0);
            assertParsesTo("1", 1);
            assertParsesTo("12345", 12345);
            assertParsesTo("1.2", 1.2);
        }

        TEST_METHOD(TestNegatives)
        {
            assertParsesTo("-1", -1);
            assertParsesTo("-12345", -12345);
            assertParsesTo("-1.2", -1.2);
        }
    private:
        void assertParsesTo(std::string source, double becomes)
        {
            std::istringstream iss(source);
            GraphLang::Parser parser(iss);
            GraphLang::Tokenizer::Number num;

            auto rr = parser.require(num);

            auto errMessage = std::move(std::get<1>(rr));
            Assert::IsTrue(std::get<0>(rr), std::wstring(errMessage.begin(), errMessage.end()).c_str());
            Assert::AreEqual(num.num, becomes);
        }

        void assertNotParses(std::string source)
        {
            std::istringstream iss(source);
            GraphLang::Parser parser(iss);
            GraphLang::Tokenizer::Number num;

            auto rr = parser.require(num);

            auto errMessage = std::move(std::get<1>(rr));
            Assert::IsFalse(std::get<0>(rr));
        }
    };
}