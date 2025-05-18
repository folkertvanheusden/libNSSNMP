#include <stdexcept>
#include <gtest/gtest.h>

#include "../block.h"
#include "snmp.h"


TEST(Snmp, test) {
	std::atomic_bool stop { false };
	oid_req_t or_;

	snmp_data sd;
        sd.register_oid("1.3.6.1.2.1.1.1.0",      "libnssnmp");  // static string
        sd.register_oid("1.3.6.1.2.1.1.3.0",      new snmp_data_type_running_since());
        sd.register_oid("1.3.6.1.2.1.1.4.0",      "Folkert van Heusden <folkert@vanheusden.com>");
        sd.register_oid("1.3.6.1.2.1.1.5.0",      "libnssnmp");
        sd.register_oid("1.3.6.1.2.1.1.6.0",      "The Netherlands, Europe, Earth");
        sd.register_oid("1.3.6.1.2.1.1.7.0",      snmp_integer::si_integer, 254);
        sd.register_oid("1.3.6.1.2.1.1.8.0",      snmp_integer::si_ticks, 0);
        sd.register_oid("1.3.6.1.4.1.2021.100.3", __DATE__);
	snmp s(&sd, &stop, false, 1610);

	// too short
	const uint8_t data1[] { 123 };
	block b1(data1, sizeof data1);
	EXPECT_THROW(s.process_BER(&b1, &or_, false, 2), std::runtime_error);

	// too short (length field)
	const uint8_t data2[] { 0x30, 0x80, 0x03 };
	block b2(data2, sizeof data2);
	EXPECT_THROW(s.process_BER(&b2, &or_, false, 2), std::runtime_error);

	// invalid start
	const uint8_t data3[] { 0x29, 0x01, 0x03 };
	block b3(data3, sizeof data3);
	EXPECT_EQ(s.process_BER(&b3, &or_, false, 2), false);

	// integer too long
	const uint8_t data4[9] { };
	block b4(data4, sizeof data4);
	EXPECT_THROW(s.get_INTEGER(&b4), std::runtime_error);

	const uint8_t data5[] { 0xea, 0x55, 0xaa };
	block b5(data5, sizeof data5);
	EXPECT_EQ(s.get_INTEGER(&b5), 0xea55aa);

	const uint8_t data6[] { 0xea, 0x55, 0xaa, 0x78 };
	block b6(data6, sizeof data6);
	b6.get_byte();
	uint8_t type6 = 0, length6 = 0;
	s.get_type_length(&b6, &type6, &length6);
	EXPECT_EQ(type6, 0x55);
	EXPECT_EQ(length6, 0xaa);

	// OID test
	const uint8_t data7[] { 0x2b, 0x6, 0x1, 0x4, 0x1, 0x8f, 0x65, 0x64, 0x03 };
	block b7(data7, sizeof data7);
	std::string oid;
	EXPECT_EQ(s.get_OID(&b7, &oid), true);
	EXPECT_EQ(oid, "1.3.6.1.4.1.2021.100.3");
}
