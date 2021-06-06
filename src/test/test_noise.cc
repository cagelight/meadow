#include "tests.hh"

#include "meadow/noise.hh"

#include <future>
#include <random>
#include <thread>
#include <vector>

void test_noise() {
	
	static constexpr int64_t DIST_LIM = 1 << 20;
	
	static constexpr size_t TEST_COUNT_2D = 2000000;
	static constexpr size_t TEST_COUNT_3D = 20000;
	static constexpr size_t TEST_COUNT_4D = 20000;
	
	static size_t TEST_THREAD_COUNT = std::thread::hardware_concurrency();
	
	meadow::noise::simplex tn1;
	tn1.seed();
	
	std::mt19937_64 rng { std::random_device{}() };
	meadow::noise::simplex tn2;
	tn2.seed(rng);
	
	std::uniform_real_distribution<double> dist { -DIST_LIM, DIST_LIM };
	
	struct ThreadCache {
		double avg = 0;
		double min = std::numeric_limits<double>::max();
		double max = std::numeric_limits<double>::min();
	};
	
	auto run_test = [](std::function<ThreadCache()> thf) -> ThreadCache {
		std::vector<std::future<ThreadCache>> futures;
		std::vector<std::thread> workers;
		
		for (size_t i = 0; i < TEST_THREAD_COUNT; i++) {
			std::packaged_task<ThreadCache()> task {thf};
			futures.emplace_back(task.get_future());
			workers.emplace_back(std::move(task));
		}
		
		ThreadCache master;
		for (auto & f : futures) {
			ThreadCache tc = f.get();
			master.avg += tc.avg;
			if (tc.min < master.min) master.min = tc.min;
			if (tc.max > master.max) master.max = tc.max;
		}
		master.avg /= TEST_THREAD_COUNT;
		
		for (auto & w : workers) {
			w.join();
		}
		
		futures.clear();
		workers.clear();
		
		return master;
	};
	
	tlog << "================================";
	tlog << "Starting 2D Test";
	tlog << "--------------------------------";
	{
		
		ThreadCache result = run_test([&](){
			ThreadCache tc;
			
			for (size_t i = 0; i < TEST_COUNT_2D; i++) {
				double v = tn2.generate(dist(rng), dist(rng));
				tc.avg += v;
				if (v < tc.min) tc.min = v;
				if (v > tc.max) tc.max = v;
			}
			
			tc.avg /= TEST_COUNT_2D;
			
			return tc;
		});
		
		double err = result.max > -result.min ? result.max : -result.min;
		
		tlog << "Average: " << result.avg;
		tlog << "Minimum: " << result.min;
		tlog << "Maximum: " << result.max;
		tlog << "Hueristic Adjustment: " << 1 / err;
	}
	tlog << "================================\n\n";
	
	tlog << "================================";
	tlog << "Starting 3D Test";
	tlog << "--------------------------------";
	{
		
		ThreadCache result = run_test([&](){
			ThreadCache tc;
			
			for (size_t i = 0; i < TEST_COUNT_3D; i++) {
				double v = tn2.generate(dist(rng), dist(rng), dist(rng));
				tc.avg += v;
				if (v < tc.min) tc.min = v;
				if (v > tc.max) tc.max = v;
			}
			
			tc.avg /= TEST_COUNT_3D;
			
			return tc;
		});
		
		double err = result.max > -result.min ? result.max : -result.min;
		
		tlog << "Average: " << result.avg;
		tlog << "Minimum: " << result.min;
		tlog << "Maximum: " << result.max;
		tlog << "Hueristic Adjustment: " << 1 / err;
	}
	tlog << "================================\n\n";
	
	tlog << "================================";
	tlog << "Starting 4D Test";
	tlog << "--------------------------------";
	{
		
		ThreadCache result = run_test([&](){
			ThreadCache tc;
			
			for (size_t i = 0; i < TEST_COUNT_4D; i++) {
				double v = tn2.generate(dist(rng), dist(rng), dist(rng), dist(rng));
				tc.avg += v;
				if (v < tc.min) tc.min = v;
				if (v > tc.max) tc.max = v;
			}
			
			tc.avg /= TEST_COUNT_4D;
			
			return tc;
		});
		
		double err = result.max > -result.min ? result.max : -result.min;
		
		tlog << "Average: " << result.avg;
		tlog << "Minimum: " << result.min;
		tlog << "Maximum: " << result.max;
		tlog << "Hueristic Adjustment: " << 1 / err;
	}
	tlog << "================================\n\n";
}
