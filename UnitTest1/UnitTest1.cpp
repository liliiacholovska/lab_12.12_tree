#include "pch.h"
#include "CppUnitTest.h"
#include "../Lab12.12(2)/lab12.12(2).cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestDeleteNode)
		{
			TreeNode* root = nullptr;
			Note note1 = { "Doe", "John", "123456789", {1, 4, 1990} };
			Note note2 = { "Smith", "Alice", "987654321", {15, 8, 1985} };

			root = insertNode(root, note1);
			root = insertNode(root, note2);

			root = deleteNode(root, "Doe");
			TreeNode* foundNode = searchByLastName(root, "Doe");

			Assert::IsNull(foundNode);
		}
	};
}