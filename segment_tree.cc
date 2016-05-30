//////////////////////////////////////////////////
// @fileoverview Defination of segment tree.
// @author ysd
//////////////////////////////////////////////////

#include "segment_tree.h"

using namespace ysd_bes_aoi;

// region public method
// endregion public method

// region static method

// Create non-leaf node recursively with value array and id array.
TreeNode* SegmentTree::CreateSegmentTree (float* values, uint16_t* ids, int i, int j)
{
	assert(j > i);
	TreeNode* root = new TreeNode();
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
		root->id = kNonID;
		if (mid > i)
			root->left = CreateSegmentTree(values, ids, i, mid);
		if (j > mid)
			root->right = CreateSegmentTree(values, ids, mid, j);
		root->height = 1 + std::max(root->left->height, root->right->height);
	}
	return root;
}

// endregion static method

// region private method

// Search the tree recusively to find the position in the range and push the id in result.
void SegmentTree::SearchRange (const TreeNode* root, const float start, const float end, std::vector<uint16_t>& result)
{

	// It is a leaf node.
	if (root->id != kNonID)
	{
		if (root->pos_start <= end && root->pos_start >= start)
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

TreeNode* SegmentTree::InsertNode (TreeNode* root, uint16_t id, float value)
{

	// null tree.
	if (root == nullptr)
	{
		root = new TreeNode();
		root->id = id;
		root->pos_start = value;
		return root;
	}

	// A leaf root.
	if (root->id != kNonID)
	{
		// Two new child nodes.
		TreeNode* left = new TreeNode();
		left->height = 0;

		TreeNode* right = new TreeNode();
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
		root->id = kNonID;
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
			root->left = InsertNode(root->left, id, value);
			root->pos_start = root->left->pos_start;
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
			if (root->left->id != kNonID && root->right->id != kNonID)
			{
				// Do not need to rotate tree.
				root->left = InsertNode(root->left, id, value);
				root->height += 1;
				return root;
			}

			// Left child is a non-leaf node.
			if (root->left->id == kNonID && root->right->id != kNonID)
			{
				if (value > root->left->pos_end)
				{
					root->right = InsertNode(root->right, id, value);
					// Now height of right is equal to height of left.
					return root;
				}
				else
				{
					root->left = InsertNode(root->left, id, value);
					// Rotate! Do not need to adjust height.
					return RotateTreeR(root);
				}
			}

			// Right child is a non-leaf node.
			if (root->left->id != kNonID && root->right->id == kNonID)
			{
				if (value < root->right->pos_start)
				{
					root->left = InsertNode(root->left, id, value);
					return root;
				}
				else
				{
					root->right = InsertNode(root->right, id, value);
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
				root->left = InsertNode(root->left, id, value);
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
				root->right = InsertNode(root->right, id, value);
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
					root->left = InsertNode(root->left, id, value);
					// height = max(rh, lh + 1)
				}
				else if (lh > rh)
				{
					root->right = InsertNode(root->right, id, value);
					// height = max(lh, rh + 1)
				}
				else
				{
					root->left = InsertNode(root->left, id, value);
					root->height = root->left->height + 1;
				}
				return root;
			}
		}
		// Out of range of right node.
		else
		{
			// Insert right.
			root->right = InsertNode(root->right, id, value);
			if (root->right->pos_end != kNonPosition)
				root->pos_end = root->right->pos_end;
			else
				root->pos_end = root->right->pos_start;
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

TreeNode* SegmentTree::RemoveNode (TreeNode* root, uint16_t id, float value)
{
	// Left child is a leaf node.
	if (root->left->id == id)
	{
		TreeNode* pn = root->right;
		delete root->left;
		delete root;
		return pn;
	}
	else if (root->right->id == id)
	{
		TreeNode* pn = root->left;
		delete root->right;
		delete root;
		return pn;
	}

	// In range.
	if (value <= root->pos_end && value >= root->pos_start)
	{
		// The removed node is in the left child.
		if (value <= root->left->pos_end)
		{
			root->left = RemoveNode(root->left, id, value);

			// Unbalance!! Rotate!!
			if (root->right->height - root->left->height > 1)
			{
				if (root->right->left->height > root->right->right->height)
				{
					return RotateTreeRL(root);
				}
				else
				{
					return RotateTreeL(root);
				}
			}
			else
			{
				root->pos_start = root->left->pos_start;
				root->height = 1 + std::max(root->left->height, root->right->height);
				return root;
			}
		}
		// The removed node is in the right child.
		else if (value >= root->right->pos_start)
		{
			root->right = RemoveNode(root->right, id, value);

			// Unbalance!! Rotate!!
			if (root->left->height - root->right->height > 1)
			{
				if (root->left->right->height > root->left->right->height)
				{
					return RotateTreeLR(root);
				}
				else
				{
					return RotateTreeR(root);
				}
			}
			else
			{
				if (root->right->id != kNonID)
				{
					// A non-leaf child.
					root->pos_end = root->right->pos_start;
				}
				else
				{
					root->pos_end = root->right->pos_end;
				}
				root->height = 1 + std::max(root->left->height, root->right->height);
				return root;
			}
		}
	}

	// The given value is out of range.
	return nullptr;

}

TreeNode* SegmentTree::UpdateNode (TreeNode* root, uint16_t id, float cur_val, float new_val)
{
	// Out of range.
	if (cur_val < root->pos_start || cur_val > root->pos_end)
	{
		return nullptr;
	}

	// Find it!
	// Height do not need to change.
	if (root->left->id == id)
	{
		if (new_val <= root->right->pos_start)
		{
			root->left->pos_start = new_val;
			root->pos_start = new_val;
			return root;
		}
		// The new value is big than the current right node.
		else
		{
			// Right child is a leaf node.
			if (root->right->id != kNonID)
			{
				root->left->pos_start = new_val;
				// Exchange two children.
				auto pn = root->left;
				root->left = root->right;
				root->right = pn;
				// Set range of the node.
				root->pos_start = root->left->pos_start;
				root->pos_end = root->right->pos_start;
				return root;
			}
			// Right child is a non-leaf node
			else
			{
				if (new_val <= root->right->pos_end)
				{
					// Exchange node's data.
					uint16_t old_lid = root->left->id;
					root->left->pos_start = root->right->pos_start;
					root->left->id = root->right->left->id;
					root->right->pos_start = new_val;
					root->right->left->pos_start = new_val;
					root->right->left->id = old_lid;
					// Set range of the root.
					root->pos_start = root->left->pos_start;
					root->pos_end = root->right->pos_end;
					return root;
				}
				else
				{
					root->left->pos_start = new_val;
					// Exchange two children.
					auto pn = root->left;
					root->left = root->right;
					root->right = pn;
					// Set range of the root.
					root->pos_start = root->left->pos_start;
					root->pos_end = root->right->pos_start;
					return root;
				}
			}
		}
	}
	else if (root->right->id == id)
	{
		// Left child is a leaf node.
		if (root->left->id != kNonID)
		{
			if (root->left->pos_start <= new_val)
			{
				root->right->pos_start = new_val;
				root->pos_end = new_val;
			}
			else
			{
				root->right->pos_start = new_val;
				// Exchange two children.
				auto pn = root->left;
				root->left = root->right;
				root->right = pn;
				// Set range of the node.
				root->pos_start = root->left->pos_start;
				root->pos_end = root->right->pos_start;
			}
			return root;
		}
		// Right child is a non-leaf node
		else
		{
			if (root->left->pos_end <= new_val)
			{
				root->right->pos_start = new_val;
				root->pos_end = new_val;
				return root;
			}
			else if (root->left->pos_start <= new_val)
			{
				// Exchange node's data.
				uint16_t old_lid = root->right->id;
				root->right->pos_start = root->left->pos_end;
				root->right->id = root->left->right->id;
				root->left->pos_end = new_val;
				root->left->right->pos_start = new_val;
				root->left->right->id = old_lid;
				// Set range of the root.
				root->pos_start = root->left->pos_start;
				root->pos_end = root->right->pos_start;
				return root;
			}
			else
			{
				root->left->pos_start = new_val;
				// Exchange two children.
				auto pn = root->left;
				root->left = root->right;
				root->right = pn;
				// Set range of the root.
				root->pos_start = root->left->pos_start;
				root->pos_end = root->right->pos_start;
				return root;
			}
		}
	}


	// The update node is in left child.
	if (cur_val <= root->left->pos_end)
	{
		// The node after update is still in left child.
		if (new_val <= root->right->pos_start)
		{
			root->left = UpdateNode(root->left, id, cur_val, new_val);
			root->pos_start = root->left->pos_start;
			return root;
		}
		else
		{
			root = RemoveNode(root, id, cur_val);
			return InsertNode(root, id, new_val);
		}
	}
	// The update node is in right child.
	else if (cur_val >= root->right->pos_start)
	{
		// The node after update is still in right node.
		if (new_val >= root->left->pos_start)
		{
			root->right = UpdateNode(root->right, id, cur_val, new_val);
			root->pos_end = root->right->pos_end;
			return root;
		}
		else
		{
			root = RemoveNode(root, id, cur_val);
			return InsertNode(root, id, new_val);
		}
	}

	// Out of range.
	return nullptr;
}

TreeNode* SegmentTree::RotateTreeR (TreeNode* root)
{
	// root is a non-leaf node;
	assert(root->id == kNonID);
	auto pn = root->left;
	assert(pn->id == kNonID);

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
	assert(root->id == kNonID);
	auto pn = root->right;
	assert(pn->id == kNonID);

	// Once assign new pointer to a node's children, the range of the node need to change.
	root->right = pn->left;
	if (root->right->pos_end != kNonPosition)
		root->pos_end = root->right->pos_end;
	else
		root->pos_end = root->right->pos_start;

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