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

	// null tree.
	if (root == nullptr)
	{
		root = new TreeNode;
		root->id = id;
		root->pos_start = value;
		return root;
	}

	// A leaf root.
	if (root->id != NON_ID)
	{
		// Two new child nodes.
		TreeNode* left = new TreeNode;
		left->pos_end = 0.0f;
		left->height = 0;

		TreeNode* right = new TreeNode;
		right->pos_end = 0.0f;
		right->height = 0;
		if (root->pos_start < value)
		{
			// The left node is equal to the root.
			left->id = root->id;
			left->pos_start = root->pos_start;

			// The right node is the new inserted node.
			right->id = id;
			right->pos_start = value;
		}
		else
		{
			// The left node is the new inserted node.
			left->id = id;
			left->pos_start = value;

			// The right node is equal to the root.
			right->id = root->id;
			right->pos_start = root->pos_start;
		}

		// Change the root to non-leaf node.
		root->id = NON_ID;
		// For the leaf node, the pos_start stores the value.
		root->pos_start = left->pos_start;
		root->pos_end = right->pos_start;
		root->left = left;
		root->right = right;
		root->height = 1;
		return root;
	}
	// A non-leaf root
	else
	{

		// Out of range of left node.
		if (value < root->pos_start)
		{
			// Insert left.
			root->left = InsertNode(id, value, root->left);
			uint16_t left_height = root->left->height;
			uint16_t right_height = root->right->height;
			assert(left_height - right_height >= -1);
			if (left_height - right_height > 1)
			{
				return RotateTreeR(root);
			}
			else
			{
				root->height = std::max(left_height, right_height) + 1;
				return root;
			}
		}
		// In the range.
		else if (value < root->pos_end)
		{
			// Two leaf children, insert at left.
			if (root->left->id != NON_ID && root->right->id != NON_ID)
			{
				// Do not need to rotate tree.
				root->left = InsertNode(id, value, root->left);
				root->height += 1;
				return root;
			}

			// Left child is a non-leaf node.
			if (root->left->id == NON_ID && root->right->id != NON_ID)
			{
				if (value > root->left->pos_end)
				{
					root->right = InsertNode(id, value, root->right);
					// Now height of right is equal to height of left.
					return root;
				}
				else
				{
					root->left = InsertNode(id, value, root->left);
					// Rotate! Do not need to adjust height.
					return RotateTreeR(root);
				}
			}

			// Right child is a non-leaf node.
			if (root->left->id != NON_ID && root->right->id == NON_ID)
			{
				if (value < root->right->pos_start)
				{
					root->left = InsertNode(id, value, root->left);
					return root;
				}
				else
				{
					root->right = InsertNode(id, value, root->right);
					// Rotate!!
					return RotateTreeRL(root);
				}
			}

			// Two non-leaf children
			// Insert to the child which range contains the value.
			uint16_t left_end = root->left->pos_end;
			uint16_t right_start = root->right->pos_start;
			if (value < left_end)
			{
				// Insert to left child.
				root->left = InsertNode(id, value, root->left);
				if (root->left->height - root->right->height > 1)
				{
					// Unbalance!! Rotate!!
					if (root->left->left->height > root->left->right->height)
					{
						// Left child is higher, rotate root to right.
						return RotateTreeR(root);
					}
					else
					{
						return RotateTreeLR(root);
					}
				}
				else
				{
					root->height = std::max(root->left->height, root->right->height) + 1;
					return root;
				}
			}
			else if (value > right_start)
			{
				// Insert to right child.
				root->right = InsertNode(id, value, root->right);
				if (root->right->height - root->left->height > 1)
				{
					// Unbalance!! Rotate!!
					if (root->right->right->height > root->right->left->height)
					{
						// Right child is higher, rotate root to left.
						return RotateTreeL(root);
					}
					else
					{
						return RotateTreeRL(root);
					}
				}
				else
				{
					root->height = std::max(root->left->height, root->right->height) + 1;
					return root;
				}
			}
			// Insert to the less high one if the value is out of range.
			else
			{
				uint16_t lh = root->left->height;
				uint16_t rh = root->right->height;
				if (lh < rh)
				{
					root->left = InsertNode(id, value, root->left);
					// height = max(rh, lh + 1)
				}
				else if (lh > rh)
				{
					root->right = InsertNode(id, value, root->right);
					// height = max(lh, rh + 1)
				}
				else
				{
					root->left = InsertNode(id, value, root->left);
					root->height = root->left->height + 1;
				}
				return root;
			}
		}
		// Out of range of right node.
		else
		{
			// Insert right.
			InsertNode(id, value, root->right);
			uint16_t left_height = root->left->height;
			uint16_t right_height = root->right->height;
			assert(right_height - left_height >= -1);
			if (right_height - left_height > 1)
			{
				return RotateTreeL(root);
			}
			else
			{
				root->height = std::max(left_height, right_height) + 1;
				return root;
			}
		}
	}
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