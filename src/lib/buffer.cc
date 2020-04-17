#include "meadow/buffer.hh"

#include <cstdlib>
#include <cstring>

using namespace meadow;

buffer::buffer(size_t initial_capacity) :
	m_data { reinterpret_cast<byte_t *>(std::malloc(initial_capacity)) },
	m_offs { m_data },
	m_size { 0 },
	m_resv { initial_capacity }
{}

buffer::buffer(const_iterator begin, const_iterator end) :
	m_data { begin < end ? reinterpret_cast<byte_t *>(std::malloc(end - begin)) : nullptr },
	m_offs { m_data },
	m_size { begin < end ? static_cast<size_t>(end - begin) : 0 },
	m_resv { m_size }
{
	if (m_size) std::memcpy(m_data, begin, m_size);
}

buffer::buffer(buffer const & other) : 
	m_data { other.m_size ? reinterpret_cast<byte_t *>(std::malloc(other.m_size)) : nullptr },
	m_offs { m_data },
	m_size { other.m_size },
	m_resv { m_size }
{
	if (m_size) std::memcpy(m_data, other.m_offs, m_size);
}

buffer::buffer(buffer && other) : 
	m_data { other.m_data },
	m_offs { other.m_offs },
	m_size { other.m_size },
	m_resv { other.m_resv }
{
	new(&other) buffer;
}

buffer::~buffer() {
	if (m_data) free(m_data);
}

// ================================================================
// GENERAL
// ================================================================

std::string buffer::hex(bool lowercase) const {
	static constexpr uint8_t hmask = 0b11110000;
	static constexpr uint8_t lmask = 0b00001111;
	char const abase = lowercase ? 87 : 55;
	
	std::string str;
	str.resize(m_size * 2);
	for (size_t i = 0; i < m_size; i++) {
		char ch = m_offs[i];
		char vh = (ch & hmask) >> 4;
		char vl = ch & lmask;
		str[i*2+0] = static_cast<char>(vh > 9 ? abase + vh : 48 + vh);
		str[i*2+1] = static_cast<char>(vl > 9 ? abase + vl : 48 + vl);
	}
	return str;;
}

// ================================================================
// MANIPULATIONS
// ================================================================

void buffer::clear() {
	m_size = 0;
	m_offs = m_data;
}

void buffer::reserve(size_t s) {
	if (m_data && s <= m_resv) return;
	alloc(s);
}

void buffer::resize(size_t s) {
	if (m_data) {
		if (m_size == s) return;
		if (!s) { clear(); return; }
		if (s < m_size) { m_size = s; return; }
		align();
		if (s <= m_resv) { m_size = s; return; }
	}
	m_size = s;
	alloc(s);
}

void buffer::shrink_to_fit() {
	if (!m_data) return;
	alloc(m_size);
}

// ----------------
// PRIVATE
// ----------------

void buffer::align() {
	if (m_data == m_offs) return;
	std::memmove(m_data, m_offs, m_size);
	m_offs = m_data;
}

void buffer::alloc(size_t s) {
	if (s == m_resv) return;
	if (!s) {
		this->~buffer();
		new (this) buffer;
		return;
	}
	align();
	m_resv = s;
	if (m_data)
		m_offs = m_data = reinterpret_cast<byte_t *>(std::realloc(m_data, s));
	else
		m_offs = m_data = reinterpret_cast<byte_t *>(std::malloc(s));
}

void buffer::autoreserve(size_t min) {
	size_t s = static_cast<size_t>(m_resv * 1.5);
	alloc(s < min ? min : s);
}

void buffer::check_reserve(size_t req) {
	size_t ts = m_size + req;
	if (ts <= m_resv) {
		if ((m_offs - m_data) + ts <= m_resv) return;
		else align();
	} else autoreserve(ts);
}

// ================================================================
// IO
// ================================================================

void buffer::write(byte_t const * src, size_t cnt) {
	check_reserve(cnt);
	memcpy(m_offs + m_size, src, cnt);
	m_size += cnt;
}

size_t buffer::transfer(buffer & dest, size_t cnt) {
	return 0; // FIXME
}

// ================================================================
// OPERATORS
// ================================================================

bool buffer::operator == (buffer const & other) const {
	if (!m_data && !other.m_data) return true;
	if (m_size != other.m_size) return false;
	return !std::memcmp(m_offs, other.m_offs, m_size);
}
