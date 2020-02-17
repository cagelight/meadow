#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace meadow {
	struct buffer;
}

struct meadow::buffer final {
	
	using byte_t = uint8_t;
	using iterator = byte_t *;
	using const_iterator = byte_t const *;
	
	buffer() = default;
	buffer(size_t initial_capacity);
	buffer(const_iterator begin, const_iterator end);
	buffer(buffer const &);
	buffer(buffer &&);
	
	~buffer();
	
	// ================================================================
	// GENERAL
	// ================================================================
	
	[[nodiscard]] inline byte_t * data() { return m_offs; }
	[[nodiscard]] inline byte_t const * data() const { return m_offs; }
	[[nodiscard]] inline size_t size() const { return m_size; }
	[[nodiscard]] inline size_t capacity() const { return m_resv; }
	
	[[nodiscard]] std::string hex(bool lowercase = false) const;
	
	// ================================================================
	// MANIPULATIONS
	// ================================================================
	
	void clear();
	void reserve(size_t);
	void resize(size_t);
	void shrink_to_fit();
	
	// ================================================================
	// IO
	// ================================================================
	
	void write(byte_t const * src, size_t cnt);
	template <typename T, std::enable_if_t<std::is_standard_layout_v<T>, int> = 0>
	void write(T const & v) { write(reinterpret_cast<byte_t const *>(&v), sizeof(T)); }
	
	size_t transfer(buffer & dest, size_t cnt = std::numeric_limits<size_t>::max());
	
	// ================================================================
	// OPERATORS
	// ================================================================
	
	inline buffer & operator = (buffer const & other) { this->~buffer(); new (this) buffer(other); return *this; }
	inline buffer & operator = (buffer && other) { this->~buffer(); new (this) buffer(std::move(other)); return *this; }
	
	[[nodiscard]] bool operator == (buffer const & other) const;
	
	template <typename T, std::enable_if_t<std::is_standard_layout_v<T>, int> = 0>
	inline buffer & operator << (T const & v) { write<T>(v); return *this; }
	
	[[nodiscard]] inline byte_t & operator [] (size_t i) { return m_offs[i]; }
	[[nodiscard]] inline byte_t const & operator [] (size_t i) const { return m_offs[i]; }
	
private:
	byte_t * m_data = nullptr;
	byte_t * m_offs = nullptr;
	size_t m_size = 0;
	size_t m_resv = 0;
	
	void align();
	void alloc(size_t);
	void autoreserve(size_t);
	void check_reserve(size_t);
};
