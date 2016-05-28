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

	#define NON_ID 	10000
	#define NON_POS	-99999

	typedef unsigned short uint16_t;

	struct TreeNode
	{
		TreeNode* left = nullptr;
		TreeNode* right = nullptr;

		// 0 if leaf node
		uint16_t height = 0;

		// If this is a leaf node, this property will be the value of X/Y coordinate.
		float pos_start = NON_POS;

		// If this is a leaf node, this property will be 0.0f.
		float pos_end = NON_POS;

		// If this is a leaf node, this property will be 10000.
		uint16_t id = NON_ID; 
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
		SegmentTree();
		~SegmentTree();

		static TreeNode* CreateSegmentTree (float* values, uint16_t* ids, int i, int j);

		static void Print (TreeNode* root);

		// For a given range [start, end], get ids of 
		// those position that which X/Y coordinate in.
		// @param[in]	start 	Search range.
		// @param[in]	end 	Search range.
		// @return		Search result set.
		std::vector<uint16_t>& Search (const float start, const float end)
		{
			std::vector<uint16_t> result;
			SearchRange(root_, start, end, &result);
			return result;
		}

		// Insert a node with given id and value.
		// @param[in]	id 		New node's player id.
		// @param[in]	pos_x	New node's player x coordinate.	
		void Insert (uint16_t id, float pos_x)
		{
			InsertNode(id, pos_x, root_);
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

		TreeNode* root_;

	};
}

#endif
