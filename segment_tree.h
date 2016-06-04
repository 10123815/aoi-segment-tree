//////////////////////////////////////////////////
// @fileoverview Defination of segment tree.
// @author ysd
/////////////////////////////////////////////////

#ifndef _SEGMENT_TREE_H_
#define _SEGMENT_TREE_H_

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <assert.h>

namespace ysd_bes_aoi
{

	const uint16_t kNonID 		= 10000;
	const float kNonPosition 	= -99999;

	struct TreeNode
	{

		TreeNode ( ) :
			id (kNonID), pos_start (kNonPosition), pos_end (kNonPosition) , height (0)
		{

		}

		TreeNode* left;
		TreeNode* right;

		// If this is a leaf node, this property will be 10000.
		uint16_t id;

		// If this is a leaf node, this property will be the value of X/Y coordinate.
		float pos_start;

		// If this is a leaf node, this property will be 0.0f.
		float pos_end;

		// 0 if leaf node
		uint16_t height;
	};

	///////////////////////////////////////////////////
	// Segment tree to manage a 2d game scene.
	// The non-leaf node represent a range of its child
	// nodes; the leaf node represent the X/Y coordinates
	// of a position of a player.
	///////////////////////////////////////////////////
	class SegmentTree final
	{
	public:

		// Create segment tree with given coordinates and IDs.
		// @param[in]	i 	Index of the start position in the input data.
		// @param[in]	j 	Index after the start position in the input data.
		static TreeNode* CreateSegmentTree (float* values, uint16_t* ids, int i, int j);

		// Print the tree by layer.
		void Print ( )
		{
			PrintLayer(root_);
		}

		// For a given range [start, end], get ids of
		// those position that which X/Y coordinate in.
		// @param[in]	start 	Search range.
		// @param[in]	end 	Search range.
		// @param[out]	result	Search result set.
		void Search (const float start, const float end, std::vector<uint16_t>& result)
		{
			if (root_ == nullptr)
			{
				return;
			}

			SearchRange(root_, start, end, result);
		}

		// Insert a node with given id and value.
		// @param[in]	id 		New node's player id.
		// @param[in]	value	New node's player X/Y coordinate.
		void Insert (uint16_t id, float value)
		{
			root_ = InsertNode(root_, id, value);
		}

		// Remove a node with given id.
		// @param[in]	id 		Removed node's player id.
		// @param[in]	value 	X/Y coordinate to search the node.
		bool Remove (uint16_t id, float value)
		{
			if (id == root_->id)
			{
				// The last node.
				delete root_;
				root_ = nullptr;
				return true;
			}
			TreeNode* new_root = RemoveNode(root_, id, value);
			if (new_root != nullptr)
			{
				root_ = new_root;
				return true;
			}
			return false;
		}

		// Change a node's value with the given id.
		// @param[in]	id 		Changed node's player id.
		// @param[in]	cur_val	Changed node's current value that used to find it in the tree.
		// @param[in]	new_val The new value after update.
		bool Update (uint16_t id, float cur_val, float new_val)
		{
			if (cur_val > root_->pos_end || cur_val < root_->pos_start)
			{
				return false;
			}

			TreeNode* new_root = UpdateNode(root_, id, cur_val, new_val);
			if (new_root != nullptr)
			{
				root_ = new_root;
				return true;
			}
			return false;
		}

		bool Range (float* start, float* end) 
		{
			if (root_ == nullptr)
			{
				return false;
			}
			*start = root_->pos_start;
			*end = root_->pos_end;
			return true;
		}

	private:

		// For a given range [start, end], get ids of
		// those position that which X/Y coordinate in.
		// @param[in]		root 	The tree we search.
		// @param[in]		start 	Search range.
		// @param[in]		end 	Search range.
		// @param[in, out]	result	Search result set.
		void SearchRange (const TreeNode* root, const float start, const float end, std::vector<uint16_t>& result);

		// Update the value of a node with given id
		TreeNode* UpdateNode (TreeNode* root, uint16_t id, float cur_val, float new_val);

		// Insert a node with given id and value.
		// @return 	Pointer to the inserted tree.
		TreeNode* InsertNode (TreeNode* root, uint16_t id, float value);

		// Remove a node with given id and value.
		// @return 	Pointer to the handled node.
		TreeNode* RemoveNode (TreeNode* root, uint16_t id, float value);

		// Rotate the tree right.
		// @param[in] 	root 	The pointer to the unbalance node
		// @return		New root of the rotated tree.
		TreeNode* RotateTreeR (TreeNode* root);

		// Rotate the tree left.
		// @param[in] 	root 	The pointer to the unbalance node
		// @return		New root of the rotated tree.
		TreeNode* RotateTreeL (TreeNode* root);

		// Rotate the tree right than rotate left.
		// @param[in] 	root 	The pointer to the unbalance node
		// @return		New root of the rotated tree.
		TreeNode* RotateTreeRL (TreeNode* root);

		// Rotate the tree left than rotate right.
		// @param[in] 	root 	The pointer to the unbalance node
		// @return		New root of the rotated tree.
		TreeNode* RotateTreeLR (TreeNode* root);

		// Print by layer.
		void PrintLayer (TreeNode *root)
		{
			std::queue<TreeNode*> q;
			q.push(root);
			int count = 1;
			while (!q.empty())
			{
				TreeNode* p = q.front();
				q.pop();
				if (p->id == kNonID)
				{
					std::cout << "(" << (p->pos_start) << "," << (p->pos_end) << "), ";
					q.push(p->left);
					q.push(p->right);
				}
				else
				{
					std::cout << "v:" << (p->pos_start)
					          << "i:"  << (p->id)
					          << ", ";
				}

				if (--count == 0)
				{
					count = q.size();
					std::cout << std::endl;
				}

			}
		}

		TreeNode* root_;

	};
}

#endif
