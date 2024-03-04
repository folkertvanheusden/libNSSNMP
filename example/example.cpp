#include <atomic>
#include <csignal>
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
		sd->register_oid("1.3.6.1.2.1.1.1.0",            "libnssnmp"  );
		sd->register_oid("1.3.6.1.2.1.1.3.0",            new snmp_data_type_running_since());
		sd->register_oid("1.3.6.1.2.1.1.4.0",            "Folkert van Heusden <mail@vanheusden.com>");
		sd->register_oid("1.3.6.1.2.1.1.5.0",            "libnssnmp");
		sd->register_oid("1.3.6.1.2.1.1.6.0",            "The Netherlands, Europe, Earth");
		sd->register_oid("1.3.6.1.2.1.1.7.0",            snmp_integer::si_integer, 254);
		sd->register_oid("1.3.6.1.2.1.1.8.0",            snmp_integer::si_integer, 0);
		sd->register_oid("1.3.6.1.4.1.2021.100.3",       __DATE__);

		s = new snmp(sd, &stop);
	}
	catch(const char *error) {
		fprintf(stderr, "Error: %s\n", error);
		return 1;
	}

	while(!stop)
		sleep(1);

	return 0;
}
