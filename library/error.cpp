#include "acsetup.hpp"
#include "uatraits/error.hpp"

#include <cstdio>
#include <cstdarg>

namespace uatraits {

error::error(char const *format, ...) :
	std::exception()
{
	va_list args;
	char buffer[256];

	va_start(args, format);
	int size = vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);
	
	if (size > 0) {
		message_.assign(buffer, buffer + size);
	}
}

} // namespace
