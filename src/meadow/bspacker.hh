#pragma once

#include <memory>

namespace meadow {

	template <typename Q = unsigned int> struct BSPacker {
		
		struct BSPackerCoords {
			Q x, y;
		};
		
		BSPacker(Q init_width = 0, Q init_height = 0) : root { new BSPackerNode(false, init_width, init_height) } {}
		~BSPacker() = default;
		
		BSPackerCoords pack (Q width, Q height) {
			BSPackerCoords c;
			bool ok;
			root->pack_iter(ok, width, height, c);
			
			if (!ok) { // expand
				if (root->width + width > root->height + height) { // expand by height
					std::unique_ptr<BSPackerNode> new_root { new BSPackerNode {false, std::max(root->width, width), root->height + height} };
					new_root->child_a = std::move(root);
					new_root->child_b.reset(new BSPackerNode {false, std::max(new_root->child_a->width, width), height, 0, new_root->child_a->height});
					new_root->pack_iter(ok, width, height, c);
					root = std::move(new_root);
				} else { // expand by width
					std::unique_ptr<BSPackerNode> new_root { new BSPackerNode {false, root->width + width, std::max(root->height, height)} };
					new_root->child_a = std::move(root);
					new_root->child_b.reset(new BSPackerNode {false, width, std::max(new_root->child_a->height, height), new_root->child_a->width, 0});;
					new_root->pack_iter(ok, width, height, c);
					root = std::move(new_root);
				}
			}
			
			return c;
		}
		
		inline Q width() const { return root->width; }
		inline Q height() const { return root->height; }
		
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
		
		std::unique_ptr<BSPackerNode> root;
		
	};

}
