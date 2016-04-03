#include "stdafx.h"
#include "CppUnitTest.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ParseTests
{
	TEST_CLASS(IdentifierTest)
	{
	public:
		TEST_METHOD(NotAndIdentifier)
		{
			assertNotParses("");
			assertNotParses("$$$$$");
			assertNotParses("\"look a string!\"");
			assertNotParses("()");
			assertNotParses("3.14159");
			assertNotParses("[]");
			assertNotParses("-1");
		}

		TEST_METHOD(IsAnIdentifier)
		{
			assertParsesTo("  hunter ", "hunter");
			assertParsesTo("myCity", "myCity");
			assertParsesTo("  snake_case ", "snake_case");
		}
	private:

		void assertParsesTo(std::string source, std::string becomes)
		{
			std::istringstream iss(source);
			GraphLang::Parser parser(iss);
			GraphLang::Tokenizer::Identifier id;

			auto rr = parser.require(id);

			auto errMessage = std::move(std::get<1>(rr));
			Assert::IsTrue(std::get<0>(rr), std::wstring(errMessage.begin(), errMessage.end()).c_str());
			Assert::AreEqual(id.id, becomes);
		}

		void assertNotParses(std::string source)
		{
			std::istringstream iss(source);
			GraphLang::Parser parser(iss);
			GraphLang::Tokenizer::Identifier id;

			auto rr = parser.require(id);

			auto err = "It think's it is: " + id.id;
			Assert::IsFalse(std::get<0>(rr), wstring(err.begin(), err.end()).c_str());
		}
	};
}