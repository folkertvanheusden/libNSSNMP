#include <atomic>
#include <csignal>
#include <cstdint>
#include <cstdio>
#include <unistd.h>
#include <libnssnmp/snmp.h>


snmp      *s  { nullptr };
snmp_data *sd { nullptr };

std::atomic_bool stop { false };

void signal_handler(int s)
{
	stop = true;
}

int main(int argc, char *argv[])
{
	signal(SIGINT, signal_handler);

	try {
		sd = new snmp_data();
		sd->register_oid("1.3.6.1.2.1.1.1.0",            "libnssnmp");  // static string
		sd->register_oid("1.3.6.1.2.1.1.3.0",            new snmp_data_type_running_since());
		sd->register_oid("1.3.6.1.2.1.1.4.0",            "Folkert van Heusden <mail@vanheusden.com>");
		sd->register_oid("1.3.6.1.2.1.1.5.0",            "libnssnmp");
		sd->register_oid("1.3.6.1.2.1.1.6.0",            "The Netherlands, Europe, Earth");
		sd->register_oid("1.3.6.1.2.1.1.7.0",            snmp_integer::si_integer, 254);
		sd->register_oid("1.3.6.1.2.1.1.8.0",            snmp_integer::si_integer, 0);
		sd->register_oid("1.3.6.1.4.1.2021.100.3",       __DATE__);

		sd->register_oid("1.3.6.1.2.3.4", snmp_integer::si_integer, 123);  // 123 is a static integer value

		uint64_t a_value_that_changes = 456;
		sd->register_oid("1.3.6.1.2.3.5.0", new snmp_data_type_stats(snmp_integer::si_integer, &a_value_that_changes));

		int some_int = 4;
		sd->register_oid("1.3.6.1.2.3.6.0", new snmp_data_type_stats_int(&some_int));

		uint32_t an_uint32 = 19;
		sd->register_oid("1.3.6.1.2.3.8.0", new snmp_data_type_stats_uint32_t(&an_uint32));

		s = new snmp(sd, &stop, true);  // set to 'false' to suppress any errors/warnings
	}
	catch(const char *error) {
		fprintf(stderr, "Error: %s\n", error);
		return 1;
	}

	while(!stop)
		sleep(1);

	delete s;
	delete sd;

	return 0;
}
