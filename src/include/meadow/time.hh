#pragma once

/*
██╗  ██╗███████╗ █████╗ ██████╗ ███████╗██████╗      ██████╗ ███╗   ██╗██╗  ██╗   ██╗
██║  ██║██╔════╝██╔══██╗██╔══██╗██╔════╝██╔══██╗    ██╔═══██╗████╗  ██║██║  ╚██╗ ██╔╝
███████║█████╗  ███████║██║  ██║█████╗  ██████╔╝    ██║   ██║██╔██╗ ██║██║   ╚████╔╝ 
██╔══██║██╔══╝  ██╔══██║██║  ██║██╔══╝  ██╔══██╗    ██║   ██║██║╚██╗██║██║    ╚██╔╝  
██║  ██║███████╗██║  ██║██████╔╝███████╗██║  ██║    ╚██████╔╝██║ ╚████║███████╗██║   
╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚═╝  ╚═╝     ╚═════╝ ╚═╝  ╚═══╝╚══════╝╚═╝   
*/

#include <cmath>
#include <cstdint>
#include <ctime>

namespace meadow {
	
	template <clockid_t CLOCK = CLOCK_MONOTONIC>
	struct time {
		
		static inline constexpr int64_t NANO = 1000000000;
		static inline constexpr int64_t SIG_THRESH = 50000;
		
		struct unit : public timespec {
			
			constexpr unit() : timespec { 0, 0 } {}
			unit( timespec const & t ) : timespec { t } {}
			unit( double sec ) {
				double i;
				tv_nsec = std::modf( sec, &i ) * NANO;
				tv_sec = i;
				settle();
			}
			
			inline void settle() {
				tv_sec += tv_nsec / NANO;
				tv_nsec = tv_nsec % NANO;
				if ( tv_nsec < 0 ) {
					tv_sec --;
					tv_nsec = NANO + tv_nsec;
				}
			}
			
			inline bool is_significant() const {
				if ( tv_sec < 0 ) return false;
				if ( tv_sec > 0 ) return true;
				return tv_nsec > SIG_THRESH;
			}
			
			inline double seconds() const {
				return static_cast<double>(tv_sec) + static_cast<double>(tv_nsec) / NANO;
			}
			
			inline unit operator + ( unit const & other ) const {
				unit u;
				u.tv_sec = tv_sec + other.tv_sec;
				u.tv_nsec = tv_nsec + other.tv_nsec;
				u.settle();
				return u;
			}
			
			inline unit operator - ( unit const & other ) const {
				unit u;
				u.tv_sec = tv_sec - other.tv_sec;
				u.tv_nsec = tv_nsec - other.tv_nsec;
				u.settle();
				return u;
			}
			
			inline bool operator > ( unit const & other ) const {
				if ( tv_sec == other.tv_sec ) return tv_nsec > other.tv_nsec;
				return tv_sec > other.tv_sec;
			}
			
			inline bool operator < ( unit const & other ) const {
				if ( tv_sec == other.tv_sec ) return tv_nsec < other.tv_nsec;
				return tv_sec > other.tv_sec;
			}
			
		};
		
		static inline unit now() {
			unit u;
			clock_gettime(CLOCK, &u);
			return u;
		}
		
		struct keeper {
			
			unit m_target;
			
			inline keeper( unit target = 1.0 / 60.0 ) : m_target { target } {
				reset();
			}
			
			inline void reset() {
				m_mark = now();
			}
			
			inline unit mark() {
				unit last = m_mark;
				m_mark = now();
				return m_mark - last;
			}
			
			inline unit check() const {
				return now() - m_mark;
			}
			
			inline void buffer() {
				nanosleep( &m_target, nullptr );
			}
			
			inline unit mark_and_sleep() {
				unit n = now();
				unit span = n - m_mark;
				unit diff = m_target - span;
				if (!diff.is_significant()) {
					m_mark = n;
					return span;
				}
				nanosleep( &diff, nullptr );
				return mark();
			}
			
		private:
			
			unit m_mark = now();
			
		};
		
	};
}

/*
#include <chrono>
#include <thread>

namespace meadow {
	
	template <typename CLOCK = std::chrono::steady_clock>
	struct timekeeper {
		
		using time_point = CLOCK::time_point;
		
		inline timekeeper() {
			reset();
		}
		
		inline static time_point now() {
			return CLOCK::now();
		}
		
		inline static double seconds(CLOCK::duration const & dur) {
			return std::chrono::duration_cast<std::chrono::duration<double, std::ratio<1, 1>>>(dur).count();
		}
		
		inline void reset() {
			m_start = m_mark = now();
		}
		
		inline double mark() {
			time_point last = m_mark;
			m_mark = now();
			return seconds(m_mark - last);
		}
		
		inline double check() {
			return seconds(now() - m_mark);
		}
		
		inline double mark_and_sleep(double target_inv = 60) {
			std::this_thread::sleep_until(m_mark + from_seconds(1 / target_inv));
			return mark();
		}
		
	private:
		
		inline static CLOCK::duration from_seconds(double sec) {
			return std::chrono::duration_cast<typename CLOCK::duration>( std::chrono::duration<double, std::ratio<1, 1>>(sec) );
		}
		
		time_point m_mark;
		time_point m_start;
		
	};

}
*/
