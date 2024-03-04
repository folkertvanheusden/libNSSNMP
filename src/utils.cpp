// (C) 2022-2024 by folkert van heusden <mail@vanheusden.com>, released under Apache License v2.0
#include <cerrno>
#include <cstdarg>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <string>
#include <vector>


std::vector<std::string> split(std::string in, const std::string & splitter)
{
	std::vector<std::string> out;
	size_t splitter_size = splitter.size();

	for(;;)
	{
		size_t pos = in.find(splitter);
		if (pos == std::string::npos)
			break;

		std::string before = in.substr(0, pos);
		out.push_back(before);

		size_t bytes_left = in.size() - (pos + splitter_size);
		if (bytes_left == 0)
		{
			out.push_back("");
			return out;
		}

		in = in.substr(pos + splitter_size);
	}

	if (in.size() > 0)
		out.push_back(in);

	return out;
}

std::string myformat(const char *const fmt, ...)
{
        char *buffer = nullptr;
        va_list ap;
        va_start(ap, fmt);
        if (vasprintf(&buffer, fmt, ap) == -1) {
                va_end(ap);
                printf("myformat: failed to convert string with format \"%s\"\n", fmt);
                return fmt;
        }
        va_end(ap);

        std::string result = buffer;
        free(buffer);

        return result;
}

uint64_t get_micros()
{
        struct timespec tv { };
        if (clock_gettime(CLOCK_REALTIME, &tv) == -1) {
		printf("get_micros: clock_gettime failed (%s)\n", strerror(errno));
		return 0;
	}

        return uint64_t(tv.tv_sec) * uint64_t(1000 * 1000) + uint64_t(tv.tv_nsec / 1000);
}
