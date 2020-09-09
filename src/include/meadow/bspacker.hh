#pragma once

/*
██╗  ██╗███████╗ █████╗ ██████╗ ███████╗██████╗      ██████╗ ███╗   ██╗██╗  ██╗   ██╗
██║  ██║██╔════╝██╔══██╗██╔══██╗██╔════╝██╔══██╗    ██╔═══██╗████╗  ██║██║  ╚██╗ ██╔╝
███████║█████╗  ███████║██║  ██║█████╗  ██████╔╝    ██║   ██║██╔██╗ ██║██║   ╚████╔╝ 
██╔══██║██╔══╝  ██╔══██║██║  ██║██╔══╝  ██╔══██╗    ██║   ██║██║╚██╗██║██║    ╚██╔╝  
██║  ██║███████╗██║  ██║██████╔╝███████╗██║  ██║    ╚██████╔╝██║ ╚████║███████╗██║   
╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚═╝  ╚═╝     ╚═════╝ ╚═╝  ╚═══╝╚══════╝╚═╝   
*/

#include "brassica/common.hh"

#include <cassert>
#include <memory>
#include <variant>

namespace meadow {

	template <typename T, typename VALUE = std::monostate>
	struct BSPacker {
		
		struct Coords { T x, y; };
		
		BSPacker() {}
		
		inline T width() const { return m_root ? m_root->width() : 0; }
		inline T height() const { return m_root ? m_root->height() : 0; }
		
		inline void set_power2_mode(bool set = true) { m_power2_mode = set; }
		
		Coords pack(T width, T height, VALUE v = {}) {
			Coords c;
			if (!m_root)
				m_root = std::make_unique<BSPNode>(width, height, 0, 0);
			
			if (m_root->try_fit(width, height, v, c))
				return c;
			
			T min_width = brassica::max(m_root->width(), width);
			T min_height = brassica::max(m_root->height(), height);
			T max_width = m_root->width() + width;
			T max_height = m_root->height() + height;
			
			if (m_power2_mode) {
				min_width = brassica::square_up<T>(min_width);
				min_height = brassica::square_up<T>(min_height);
				max_width = brassica::square_up<T>(max_width);
				max_height = brassica::square_up<T>(max_height);
			}
			
			if (width > m_root->width() && height > m_root->height()) {
				if (max_width <= max_height) {
					expand_to_width(max_width);
					expand_to_height(min_height);
				} else {
					expand_to_height(max_height);
					expand_to_width(min_width);
				}
				assert(m_root->try_fit(width, height, v, c));
			}
			
			else if (width > m_root->width())
				expand_to_width(max_width);
			
			else if (height > m_root->height())
				expand_to_height(max_height);
			
			else if (max_width > max_height)
				expand_to_height(max_height);
			
			else
				expand_to_width(max_width);
			
			assert(m_root->try_fit(width, height, v, c));
			return c;
		}
		
		void expand_to_width(T new_width) {
			m_root = BSPNode::reparent_by_width(std::move(m_root), new_width);
		}
		
		void expand_to_height(T new_height) {
			m_root = BSPNode::reparent_by_height(std::move(m_root), new_height);
		}
		
	private:
		
		struct BSPNode;
		using BSPNodePtr = std::unique_ptr<BSPNode>;
		using BSPNodePair = std::pair<BSPNodePtr, BSPNodePtr>;
		
		struct BSPNode {
			
			BSPNode(T width, T height, T x, T y) : m_width(width), m_height(height), m_x(x), m_y(y) {}
			
			static BSPNodePtr reparent_by_width(BSPNodePtr && orphan, T new_width) {
				BSPNodePtr node = std::make_unique<BSPNode>(new_width, orphan->height(), orphan->x(), orphan->y());
				T width = new_width - orphan->width();
				T height = orphan->height();
				T x = orphan->x() + orphan->width();
				T y = orphan->y();
				node->m_var.template emplace<2>(
					std::move(orphan),
					std::make_unique<BSPNode>(width, height, x, y)
				);
				return node;
			}
			
			static BSPNodePtr reparent_by_height(BSPNodePtr && orphan, T new_height) {
				BSPNodePtr node = std::make_unique<BSPNode>(orphan->width(), new_height, orphan->x(), orphan->y());
				T width = orphan->width();
				T height = new_height - orphan->height();
				T x = orphan->x();
				T y = orphan->y() + orphan->height();
				node->m_var.template emplace<2>(
					std::move(orphan),
					std::make_unique<BSPNode>(width, height, x, y)
				);
				return node;
			}
			
			inline T width() const { return m_width; }
			inline T height() const { return m_height; }
			inline T x() const { return m_x; }
			inline T y() const { return m_y; }
			
			bool try_fit(T width, T height, VALUE const & v, Coords & c) {
				if (has_value()) return false;
				if (width > m_width || height > m_height) return false;
				
				if (has_children()) {
					auto & children = get_children();
					if (children.first->try_fit(width, height, v, c)) return true;
					else return children.second->try_fit(width, height, v, c);
				}
				
				if (width == m_width && height == m_height)
					c = create_value(v);
				else if (width == m_width)
					c = split_v(height).first->create_value(v);
				else if (height == m_height)
					c = split_h(width).first->create_value(v);
				else
					c = split_h(width).first->split_v(height).first->create_value(v);
				
				return true;
			}
			
			BSPNodePair & split_h(T dist) {
				return create_children(dist, m_width - dist, m_height, m_height, m_x, m_x + dist, m_y, m_y);
			}
			
			BSPNodePair & split_v(T dist) {
				return create_children(m_width, m_width, dist, m_height - dist, m_x, m_x, m_y, m_y + dist);
			}
			
		private:
			
			using BSNodeVariant = std::variant<std::monostate, VALUE, BSPNodePair>;
			
			T m_width, m_height, m_x, m_y;
			BSNodeVariant m_var;
			
			inline bool has_children() { return m_var.index() == 2; }
			inline bool has_value() { return m_var.index() == 1; }
			inline BSPNodePair & get_children() { return std::get<2>(m_var); }
			inline VALUE & get_value() { return std::get<1>(m_var); }
			
			inline BSPNodePair & create_children(T width1, T width2, T height1, T height2, T x1, T x2, T y1, T y2) {
				m_var.template emplace<2>(
					std::make_unique<BSPNode>(width1, height1, x1, y1),
					std::make_unique<BSPNode>(width2, height2, x2, y2)
				);
				return get_children();
			}
			
			inline Coords create_value(VALUE const & v) {
				m_var.template emplace<1>(v);
				return { m_x, m_y };
			}
			
		};
		
		BSPNodePtr m_root;
		
		bool m_power2_mode = false;
	};

}

// OLD CODE:

/*
namespace meadow {

	struct BSPackerSettings {
		bool gpu_optimize = false; // always increase by powers of 2
	};

	template <typename Q = unsigned int> struct BSPacker {
		
		struct BSPackerCoords {
			Q x, y;
		};
		
		BSPacker(Q init_width = 0, Q init_height = 0, BSPackerSettings settings = {}) : m_settings { settings }, m_root { new BSPackerNode(false, init_width, init_height) } {}
		~BSPacker() = default;
		
		BSPackerCoords pack (Q width, Q height) {
			BSPackerCoords c;
			bool ok;
			m_root->pack_iter(ok, width, height, c);
			
			if (!ok) { // expand
				if (m_root->width + width > m_root->height + height) { // expand by height
					
					Q new_width = brassica::max(m_root->width, width);
					Q new_height = m_root->height + height;
					
					std::unique_ptr<BSPackerNode> new_root { new BSPackerNode {false, new_width, new_height} };
					new_root->child_a = std::move(m_root);
					new_root->child_b.reset(new BSPackerNode {false, new_width, height, 0, new_root->child_a->height});
					new_root->pack_iter(ok, width, height, c);
					m_root = std::move(new_root);
				} else { // expand by width
					
					Q new_width = m_root->width + width;
					Q new_height = brassica::max(m_root->height, height);
					
					std::unique_ptr<BSPackerNode> new_root { new BSPackerNode {false, new_width, new_height} };
					new_root->child_a = std::move(m_root);
					new_root->child_b.reset(new BSPackerNode {false, width, new_height, new_root->child_a->width, 0});
					new_root->pack_iter(ok, width, height, c);
					m_root = std::move(new_root);
				}
			}
			
			return c;
		}
		
		inline Q width() const { return m_root->width; }
		inline Q height() const { return m_root->height; }
		
	private:

		struct BSPackerNode {
			
			BSPackerNode(bool is_endpoint, Q width, Q height, Q x, Q y) : width(width), height(height), coords {x, y}, is_endpoint(is_endpoint) {}
			BSPackerNode(bool is_endpoint, Q width, Q height) : BSPackerNode(is_endpoint, width, height, 0, 0) {}
			
			Q const width;
			Q const height;
			BSPackerCoords coords;
			
			bool is_endpoint = false;
			std::unique_ptr<BSPackerNode> child_a;
			std::unique_ptr<BSPackerNode> child_b;
			
			void pack_iter (bool & ok, Q const & width, Q const & height, BSPackerCoords & pos) {
				if (is_endpoint || (width > this->width || height > this->height)) { //node already has value or new node is too big to fit
					ok = false;
					return;
				}
				
				if (child_a && child_b) { // if have children, delegate to them
					child_a->pack_iter(ok, width, height, pos);
					if (ok) return;
					child_b->pack_iter(ok, width, height, pos);
					return;
				}
				
				// have space but no children...
					
				if (width == this->width && height == this->height) { // don't need to make children, we fit the new node perfectly, assume the value and good to go
					is_endpoint = true;
					ok = true;
					pos = coords;
					return;
				}
				
				if (width != this->width && height != this->height) { // children won't be enough, one of the children needs to have further children
					child_a.reset( new BSPackerNode { false, width, this->height, coords.x, coords.y } );
					child_b.reset( new BSPackerNode { false, this->width - width, this->height, coords.x + width, coords.y } );
					child_a->pack_iter(ok, width, height, pos);
					// assert(ok);
					return;
				}
				
				if (width == this->width) { // width matches, single split
					child_a.reset( new BSPackerNode { true, width, height, coords.x, coords.y } );
					child_b.reset( new BSPackerNode { false, width, this->height - height, coords.x, coords.y + height } );
					ok = true;
					pos = child_a->coords;
					return;
				}
				
				if (height == this->height) { // height matches, single split
					child_a.reset( new BSPackerNode { true, width, height, coords.x, coords.y } );
					child_b.reset( new BSPackerNode { false, this->width - width, this->height, coords.x + width, coords.y } );
					ok = true;
					pos = child_a->coords;
					return;
				}
				
				// should never get here
			}
			
		};
		
		BSPackerSettings m_settings;
		std::unique_ptr<BSPackerNode> m_root;
		
	};

}

*/ 
