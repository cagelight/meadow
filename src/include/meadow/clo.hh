#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace meadow {

	struct CLO {
		
		using CLOValue = std::variant<std::string, int64_t, double, bool>;
		
		inline std::string const & add_opt_string(std::string_view sw_long, char sw_short = 0, std::string_view desc = "", std::string_view init_val = "") {
			CLOValuePtr ptr = std::make_shared<CLOValue>();
			return ptr->emplace<std::string>(init_val);
		}
		
	private:
		
		using CLOValuePtr = std::shared_ptr<CLOValue>;
		
		struct Option {
			std::vector<char> sw_short;
			std::vector<std::string> sw_long;
			std::string desc;
		};
		
		void finalize_option(std::initializer_list<char> short_opts, std::initializer_list<std::string_view> long_opts) {
			Option & opt = m_opts.emplace_back();
			opt.sw_short = short_opts;
			opt.sw_long = { long_opts.begin(), long_opts.end() };
		}
		
		std::vector<Option> m_opts {};
	};

}
