#pragma once

/*
██╗  ██╗███████╗ █████╗ ██████╗ ███████╗██████╗      ██████╗ ███╗   ██╗██╗  ██╗   ██╗
██║  ██║██╔════╝██╔══██╗██╔══██╗██╔════╝██╔══██╗    ██╔═══██╗████╗  ██║██║  ╚██╗ ██╔╝
███████║█████╗  ███████║██║  ██║█████╗  ██████╔╝    ██║   ██║██╔██╗ ██║██║   ╚████╔╝ 
██╔══██║██╔══╝  ██╔══██║██║  ██║██╔══╝  ██╔══██╗    ██║   ██║██║╚██╗██║██║    ╚██╔╝  
██║  ██║███████╗██║  ██║██████╔╝███████╗██║  ██║    ╚██████╔╝██║ ╚████║███████╗██║   
╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚═╝  ╚═╝     ╚═════╝ ╚═╝  ╚═══╝╚══════╝╚═╝   
*/

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>

namespace meadow {
	
	template <typename COUNTER = std::atomic_uint_fast8_t>
	struct fifo_mutex final {
		
		fifo_mutex() = default;
		
		inline void lock() {
			std::unique_lock<std::mutex> lk {mut};
			uint_fast8_t my_number = ticket_counter++;
			if (my_number != now_serving) cv.wait(lk, [&](){return my_number != now_serving;});
		}
		
		inline void unlock() {
			std::unique_lock<std::mutex> lk {mut};
			now_serving++;
			cv.notify_all();
		}
		
	private:
		std::mutex mut;
		std::condition_variable cv;
		
		COUNTER ticket_counter;
		COUNTER now_serving;
	};
	
	struct spinlock final {
		
		inline void lock() { while (accessor.test_and_set()) __asm volatile ("pause" ::: "memory"); }
		inline bool try_lock() { return !accessor.test_and_set(); }
		inline void unlock() { accessor.clear(); }
		
	private:
		
		std::atomic_flag accessor {false};
	};
	
	struct rw_mutex final {

		rw_mutex() = default;
		
		inline void read_access() {
			accessor.lock();
			write_sem.lock();
			write_sem.unlock();
			readers.fetch_add(1);
			accessor.unlock();
		}
		
		inline bool read_access_try() {
			if (!accessor.try_lock()) return false;
			if (!write_sem.try_lock()) {
				accessor.unlock();
				return false;
			}
			write_sem.unlock();
			readers.fetch_add(1);
			accessor.unlock();
			return true;
		}
		
		inline void read_done() {
			if (readers.fetch_add(-1) == 1) rnone_cv.notify_all();
		}
		
		inline void write_lock() {
			accessor.lock();
			write_sem.lock();
			
			while (readers) {
				std::unique_lock<std::mutex> lk {rnone_mut};
				rnone_cv.wait_for(lk, std::chrono::milliseconds(5));
			}
			
			accessor.unlock();
		}
		
		inline bool write_lock_try() {
			if (!accessor.try_lock()) return false;
			if (!write_sem.try_lock()) {
				accessor.unlock();
				return false;
			}
			if (readers.load()) {
				write_sem.unlock();
				accessor.unlock();
				return false;
			}
			accessor.unlock();
			return true;
		}
		
		inline void write_unlock() {
			write_sem.unlock();
		}
		
		inline  void write_to_read() {
			readers.fetch_add(1);
			write_sem.unlock();
		}
		
	private:
		std::atomic_uint_fast8_t readers {0};
		std::mutex accessor;
		std::mutex write_sem;
		
		std::mutex rnone_mut;
		std::condition_variable rnone_cv;
	};

	struct rw_spinlock final {
		
		rw_spinlock() = default;
		
		inline void read_access() {
			while (accessor.test_and_set()) __asm volatile ("pause" ::: "memory");
			while (write_sem.test_and_set()) __asm volatile ("pause" ::: "memory");
			write_sem.clear();
			readers.fetch_add(1);
			accessor.clear();
		}
		
		inline bool read_access_try() {
			if (accessor.test_and_set()) return false;
			if (write_sem.test_and_set()) {
				accessor.clear();
				return false;
			}
			write_sem.clear();
			readers.fetch_add(1);
			accessor.clear();
			return true;
		}
		
		inline void read_done() {
			readers.fetch_sub(1);
		}
		
		inline void write_lock() {
			while (accessor.test_and_set()) __asm volatile ("pause" ::: "memory");
			while (write_sem.test_and_set()) __asm volatile ("pause" ::: "memory");
			while (readers.load()) __asm volatile ("pause" ::: "memory");
			accessor.clear();
		}
		
		inline bool write_lock_try() {
			if (accessor.test_and_set()) return false;
			if (write_sem.test_and_set()) {
				accessor.clear();
				return false;
			}
			if (readers.load()) {
				write_sem.clear();
				accessor.clear();
				return false;
			}
			accessor.clear();
			return true;
		}
		
		inline void write_unlock() {
			write_sem.clear();
		}
		
		inline void write_to_read() {
			readers.fetch_add(1);
			write_sem.clear();
		}
		
	private:
		std::atomic_uint_fast8_t readers {0};
		std::atomic_flag accessor {false};
		std::atomic_flag write_sem {false};
	};
}
