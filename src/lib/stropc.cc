#include "meadow/stropc.hh"

#include <cstdarg>
#include <cstring>

std::string meadow::strf(char const * fmt, ...) noexcept {
	
	static constexpr uint_fast16_t STRF_INITLEN = 256;
	
	va_list va;
	va_start(va, fmt);
	char * tmp_buf = reinterpret_cast<char *>(malloc(STRF_INITLEN));
	tmp_buf[STRF_INITLEN - 1] = 0;
	size_t req_size = vsnprintf(tmp_buf, STRF_INITLEN, fmt, va);
	va_end(va);
	if (req_size >= STRF_INITLEN) {
		tmp_buf = reinterpret_cast<char *>(realloc(tmp_buf, req_size+1));
		va_start(va, fmt);
		vsprintf(tmp_buf, fmt, va);
		va_end(va);
		tmp_buf[req_size] = 0;
	}
	std::string r = {tmp_buf};
	free(tmp_buf);
	
	return {r};
}

char const * meadow::vas(char const * fmt, ...) noexcept {
	
	static constexpr uint_fast8_t VAS_HISTORY_SIZE = 8;
	static thread_local char * vas_buffers [VAS_HISTORY_SIZE] {0};
	static thread_local uint_fast8_t vas_cur = 0;
	
	va_list va;
	va_start(va, fmt);
	size_t size = vsnprintf(nullptr, 0, fmt, va) + 1;
	va_end(va);
	
	if (++vas_cur >= VAS_HISTORY_SIZE) vas_cur = 0;
	char * & vas_buf = vas_buffers[vas_cur];
	vas_buf = reinterpret_cast<char *>(realloc(vas_buf, size));
	
	va_start(va, fmt);
	vsnprintf(vas_buf, size, fmt, va);
	va_end(va);
	
	return vas_buf;
}

