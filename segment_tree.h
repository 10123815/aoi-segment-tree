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

	typedef unsigned short uint16_t;

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
			SearchRange(root_, start, end, result);
		}

		// Insert a node with given id and value.
		// @param[in]	id 		New node's player id.
		// @param[in]	pos_x	New node's player x coordinate.
		void Insert (uint16_t id, float x_pos)
		{
			root_ = InsertNode(id, x_pos, root_);
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
		void Update (uint16_t id, float value);

		// Insert a node with given id and value.
		// @return 	Pointer to the inserted tree.
		TreeNode* InsertNode (uint16_t id, float value, TreeNode* root);

		// Remove a node with given id and value.
		void RemoveNode (uint16_t id, float value, TreeNode* root);

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
					std::cout << "(" << (p->pos_start) << ", " << (p->pos_end) << "), ";
					q.push(p->left);
					q.push(p->right);
				}
				else
				{
					std::cout << "v: " << (p->pos_start)
					          // << ", i: "  << (p->id)
					          << ", ";
				}

				if (--count == 0)
				{
					count = q.size();
					std::cout << std::endl;
				}

			}
			std::cout << std::endl;
		}

		TreeNode* root_;

	};
}

#endif
