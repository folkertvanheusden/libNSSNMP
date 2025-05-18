#include <gtest/gtest.h>

#include "../block.h"


TEST(Block, tests) {
	const uint8_t testdata1[] { 0x01, 0x2, 0x03, 0x04, 0x05, 0xde, 0xad, 0xbe, 0xef };  // random data
	block b1(testdata1, sizeof testdata1);
	EXPECT_EQ(memcmp(b1.get_data(), testdata1, sizeof testdata1), 0);
	EXPECT_EQ(b1.get_size(), sizeof testdata1);
	EXPECT_EQ(b1.empty(), false);
	EXPECT_EQ(b1.dump(), "01 02 03 04 05 de ad be ef");

	std::vector<uint8_t> testdata2(testdata1, testdata1 + sizeof(testdata1));
	block b2(testdata2);
	EXPECT_EQ(memcmp(b2.get_data(), testdata1, sizeof testdata1), 0);
	EXPECT_EQ(b2, b1);

	block b3(b1);
	EXPECT_EQ(memcmp(b3.get_data(), testdata1, sizeof testdata1), 0);
	EXPECT_EQ(b3, b1);
	EXPECT_EQ(b3, b2);

	std::vector<uint8_t> testdata3(testdata1, testdata1 + sizeof(testdata1));
	testdata3[2] ^= 255;
	block b4(testdata3);
	EXPECT_NE(b4, b1);

	std::vector<uint8_t> testdata4(testdata1, testdata1 + sizeof(testdata1) - 1);
	block b5(testdata4);
	EXPECT_NE(b5, b1);

	std::vector<uint8_t> to_v1;
	b1.push_to_vector(to_v1);
	EXPECT_EQ(to_v1.size(), b1.get_size());
	EXPECT_EQ(memcmp(b1.get_data(), to_v1.data(), sizeof testdata1), 0);

	std::vector<uint8_t> to_v2;
	to_v2.push_back(0xaa);
	b1.push_to_vector(to_v2);
	EXPECT_EQ(to_v2.size(), b2.get_size() + 1);
	EXPECT_EQ(memcmp(b1.get_data(), to_v2.data() + 1, sizeof testdata1), 0);
	EXPECT_EQ(to_v2.at(0), 0xaa);

	block *s1 = b1.substr(5);
	EXPECT_EQ(s1->dump(), "de ad be ef");
	delete s1;

	block *s2 = b1.substr(6, 2);
	EXPECT_EQ(s2->dump(), "ad be");
	delete s2;

	block *b6 = b1.duplicate();
	EXPECT_EQ(*b6, b1);
	delete b6;
}
