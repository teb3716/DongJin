#pragma once

#include "TreeNode.h"
#include <iostream>
#include <cassert>
using namespace std;

class BinarySearchTree
{
	friend class TransferWindowManager;
private:
	TreeNode *m_root;

	// Internal resursive insert
    TreeNode* insert(SoccerPlayerData& data, TreeNode *tNode) {
        // IF: Inserting new node from the top
        if(tNode == NULL)
            tNode = new TreeNode(data, NULL, NULL);

        // ELSE: Routing to the desired place
        else if(data.m_ability < tNode->m_data.m_ability)
            tNode->m_left = insert(data, tNode->m_left);
        else if(data.m_ability > tNode->m_data.m_ability)
            tNode->m_right = insert(data, tNode->m_right);

        // Return pointer to TreeNode
        return tNode;
    };

	// Internal recursive Nth getter
	static const TreeNode* at(int &n, const TreeNode *tNode) {
		// IF: Reached NULL node
		if(tNode == NULL)
			return NULL;

		// Leftmost recursive traverse
		const TreeNode* tempNode = at(n, tNode->m_left);
		if(tempNode != NULL)
			return tempNode;

		// IF: Current node is the desired node
		if(n-- == 0)
			return tNode;

		// Rightmost recursive traverse
		return at(n, tNode->m_right);
	}

	// Internal max value finder
	TreeNode* max(TreeNode *tNode) {
		// IF: Reached NULL Node
		if(tNode == NULL)
			return NULL;

		// ELSE: Route rightmost to find the maximum node
		else if(tNode->m_right == NULL)
			return tNode;
		else
			return max(tNode->m_right);
	}

	// Internal recursive target remover
	TreeNode* deletion(int ability, TreeNode *tNode) {
		TreeNode *tempNode;
		
		// IF: Reached NULL node
		if(tNode == NULL)
			return NULL;

		// ELSE: Routing to the desired place
		else if(ability < tNode->m_data.m_ability)
			tNode->m_left = deletion(ability, tNode->m_left);
		else if(ability > tNode->m_data.m_ability)
			tNode->m_right = deletion(ability, tNode->m_right);

		// ELSE: Target node has two children
		else if(tNode->m_left && tNode->m_right) {
			tempNode = max(tNode->m_left);
			tNode->m_data = tempNode->m_data;
			tNode->m_left = deletion(tNode->m_data.m_ability, tNode->m_left);
		}

		// ELSE: Target node has only one child
		else {
			tempNode = tNode;
			if(tNode->m_left == NULL)
				tNode = tNode->m_right;
			else if(tNode->m_right == NULL)
				tNode = tNode->m_left;
			// Deallocate target node
			delete tempNode;
		}
		return tNode;
	}

	// Internal empty function to deallocate all heap memory
	TreeNode* empty(TreeNode *tNode) {
		// IF: Reached NULL node
		if(tNode == NULL)
			return NULL;

		// Recursive empty
		{
			empty(tNode->m_left);
			empty(tNode->m_right);
			delete tNode;
		}
		return NULL;
	}
		
public:
	BinarySearchTree() { m_root = NULL; }
	~BinarySearchTree() { m_root = empty(m_root); };

	// Wrapper method for recursive insert
	void insert(SoccerPlayerData& data) {
		m_root = insert(data, m_root);
	}

	// Recursive inorder traverse
	void traverse(ostream& os, TreeNode *tNode) const {
		// IF: Node is NULL
		if(tNode == NULL)
			return;
		traverse(os, tNode->m_left);
		os << tNode << '\n'; 
		traverse(os, tNode->m_right);
	}

	// Wrapper method for Nth getter
	SoccerPlayerData at(int n) const {
		const TreeNode *tNode = at(n, m_root);
		return tNode->m_data;
	}

	// Recursive getter for Nth value in ascending order
	void deletion(int ability) {
		m_root = deletion(ability, m_root);
	}

	friend ostream& operator<<(ostream& os, const BinarySearchTree& tree) {
		tree.traverse(os, tree.m_root);
		return os;
	}
};
