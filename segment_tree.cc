//////////////////////////////////////////////////
// @fileoverview Defination of segment tree.
// @author ysd
/////////////////////////////////////////////////

#include "segment_tree.h"

using namespace ysd_bes_aoi;

// region public method

// Search the tree recusively to find the position in the range and push the id in result.
void SegmentTree::SearchRange (const TreeNode* root, const float start, const float end, std::vector<uint16_t>& result)
{

	// It is a leaf node.
	if (root->id != NON_ID)
	{
		if (root->pos_start < end && root->pos_start > start)
		{
			result.push_back(root->id);
		}
		return;
	}

	if (root->pos_start > end || root->pos_end < start)
	{
		// The two range have no coincident area.
		return;
	}

	SearchRange(root->left, start, end, result);
	SearchRange(root->right, start, end, result);

}

// endregion public method

// region static method

// Create non-leaf node recursively with value array and id array.
TreeNode* SegmentTree::CreateSegmentTree (float* values, uint16_t* ids, int i, int j)
{
	assert(j > i);
	TreeNode* root = new TreeNode;
	if (j - i == 1)
	{
		root->pos_start = values[i];
		root->id = ids[i];
		root->height = 0;
	}
	else
	{
		int mid = ((j - i) >> 1) + i;
		root->pos_start = values[i];
		root->pos_end = values[j - 1];
		// Non-leaf node's id = 10000.
		root->id = NON_ID;
		if (mid > i)
			root->left = CreateSegmentTree(values, ids, i, mid);
		if (j > mid)
			root->right = CreateSegmentTree(values, ids, mid, j);
		root->height = 1 + std::max(root->left->height, root->right->height);
	}
	return root;
}

void SegmentTree::Print (TreeNode *root)
{
	std::queue<TreeNode*> q;
	q.push(root);
	int count = 1;
	while (!q.empty())
	{
		TreeNode* p = q.front();
		q.pop();
		if (p->id == NON_ID)
		{
			std::cout << "(" << (p->pos_start) << ", " << (p->pos_end) << "), ";
			q.push(p->left);
			q.push(p->right);
		}
		else
		{
			std::cout << "v: " << (p->pos_start)
			          << ", i: "  << (p->id)
			          << ", ";
		}

		if (--count == 0)
		{
			count = q.size();
			std::cout << std::endl;
		}

	}
}

// endregion static method

// region private method


TreeNode* SegmentTree::InsertNode (uint16_t id, float value, TreeNode* root)
{

}

void SegmentTree::RemoveNode (uint16_t id, float value, TreeNode* root)
{

}

TreeNode* SegmentTree::RotateTreeR (TreeNode* root)
{
	// root is a non-leaf node;
	assert(root->id == NON_ID);
	auto pn = root->left;
	assert(pn->id == NON_ID);

	// Once assign new value to a node's children, the range of the node need to change.
	root->left = pn->right;
	root->pos_start = root->left->pos_start;

	pn->right = root;
	pn->pos_end = pn->right->pos_end;

	// Reset the height.
	root->height = std::max(root->left->height, root->right->height) + 1;
	pn->height = std::max(pn->left->height, pn->right->height) + 1;

	return pn;

}

TreeNode* SegmentTree::RotateTreeL (TreeNode* root)
{
	// root is a non-leaf node;
	assert(root->id == NON_ID);
	auto pn = root->right;
	assert(pn->id == NON_ID);

	// Once assign new pointer to a node's children, the range of the node need to change.
	root->right = pn->left;
	root->pos_end = root->right->pos_end;

	pn->left = root;
	pn->pos_start = pn->left->pos_start;

	// Reset the height.
	root->height = std::max(root->left->height, root->right->height) + 1;
	pn->height = std::max(pn->left->height, pn->right->height) + 1;

	return pn;

}

TreeNode* SegmentTree::RotateTreeRL (TreeNode* root)
{
	// Rotate right child with right first.
	root->right = RotateTreeR(root->right);

	// Then rotate root with left.
	return RotateTreeL(root);
}

TreeNode* SegmentTree::RotateTreeLR (TreeNode* root)
{
	// Rotate left child with left first.
	root->left = RotateTreeL(root->left);

	// Then rotate root with right.
	return RotateTreeR(root);
}

// endregion private method