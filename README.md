# libnssnmp
libnssnmp is a simple SNMP agent - it allows your (C++) application to respond to SNMP requests.


# how to use it

## steps

1. instantiate an snmp_data object
2. configure each variable you want to return in the snmp_data object
3. instantiate an snmp object
4. start the snmp object


### 1. instantiate an snmp_data object

    snmp_data sd;


### 2. configure each variable you want to return in the snmp_data object

In this step you will add objects that are published. The objects will be deleted when the snmp_data object is deleted.


#### static data

This is a string, it will be returned as-is:

    sd.register_oid("1.3.6.1.2.1.1.4.0", "Folkert van Heusden <folkert@vanheusden.com>");

snmp_data_type_running_since is a special case, it lets the library return the uptime of itelf.

    sd.register_oid("1.3.6.1.2.1.1.3.0", new snmp_data_type_running_since());

Publish integer values. The second parameter is the type. Can be:
* si_counter32  a 32 bit integer value that only increments
* si_counter64  a 64 bit integer value that only increments
* si_integer    an integer value that can be anything during its existance
* si_ticks      a time-ticks variable, 1/100th of a second

    sd.register_oid("1.3.6.1.2.3.4", snmp_integer::si_integer, 123);

The values above are all static.


#### dynamic data

As the library publishes the data from a thread, changing data must use atomic variables.

For the meaning of the snmp_integer::si_... types, see above in the static section.

    std::atomic_uint64_t a_thread_safe_value_that_changes = 123;

    sd.register_oid("1.3.6.1.2.3.51.0", new snmp_data_type_stats_atomic(snmp_integer::si_integer, &a_thread_safe_value_that_changes));

There is also:

    snmp_data_type_stats_atomic_int() for std::atomic_int and
    snmp_data_type_stats_atomic_uint32_t for std::atomic_uint32_t.


### 3. instantiate an snmp object

    std::atomic_bool stop_flag { false };
...

    snmp s(&sd, &stop_flag, false);

You can also add a 4th parameter which sets the port-number to listen on. Usually this is port 161 (requires root) which is the default.


### 4. start the snmp object

    if (s.begin() == false) {
        // error handling
    }

This will start a thread that processes incoming snmp requests. The stop_flag can be used to terminate the thread, just deleting the snmp object does this as well.


See also example/example.cpp in this repository for an implementation example.



(C) 2022-2025 by Folkert van Heusden <mail@vanheusden.com>
