#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ParseTests
{		
	TEST_CLASS(StringTest)
	{
	public:
		TEST_METHOD(NotStrings)
		{
			assertNotParses("");
			assertNotParses("1.023");
			assertNotParses("()");
			assertNotParses("test_identifier");
		}

		TEST_METHOD(TestNormals)
		{
			assertParsesTo("\"\"", "");
			assertParsesTo("\"a\"", "a");
			assertParsesTo("\"A\"", "A");
			assertParsesTo("\"Hunter's\"", "Hunter's");
		}

		TEST_METHOD(TestEscapes)
		{
			assertParsesTo("\"A\\\\Backslash\"", "A\\Backslash");
			assertParsesTo("\"A\\nNewline\"", "A\nNewline");
			assertParsesTo("\"A\\\"Quote\"", "A\"Quote");
		}
	private:
		void assertParsesTo(std::string source, std::string becomes)
		{
			std::istringstream iss(source);
			GraphLang::Parser parser(iss);
			GraphLang::Tokenizer::String str;

			auto rr = parser.require(str);

			auto errMessage = std::move(std::get<1>(rr));
			Assert::IsTrue(std::get<0>(rr), std::wstring(errMessage.begin(), errMessage.end()).c_str());
			Assert::AreEqual(str.val, becomes);
		}

		void assertNotParses(std::string source)
		{
			std::istringstream iss(source);
			GraphLang::Parser parser(iss);
			GraphLang::Tokenizer::String str;

			auto rr = parser.require(str);

			auto errMessage = std::move(std::get<1>(rr));
			Assert::IsFalse(std::get<0>(rr), std::wstring(str.val.begin(), str.val.end()).c_str());
		}
	};
}