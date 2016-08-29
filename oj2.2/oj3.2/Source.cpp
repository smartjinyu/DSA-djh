/*
真二叉树重构(Proper Rebuild)
Description
In general, given the preorder traversal sequence and postorder traversal sequence of a binary tree, we cannot determine the binary tree.



Figure 1

In Figure 1 for example, although they are two different binary tree, their preorder traversal sequence and postorder traversal sequence are both of the same.

But for one proper binary tree, in which each internal node has two sons, we can uniquely determine it through its given preorder traversal sequence and postorder traversal sequence.

Label n nodes in one binary tree using the integers in [1, n], we would like to output the inorder traversal sequence of a binary tree through its preorder and postorder traversal sequence.

Input
The 1st line is an integer n, i.e., the number of nodes in one given binary tree,

The 2nd and 3rd lines are the given preorder and postorder traversal sequence respectively.

Output
The inorder traversal sequence of the given binary tree in one line.

Example
Input

5
1 2 4 5 3
4 5 2 3 1
Output

4 2 5 1 3
Restrictions
For 95% of the estimation, 1 <= n <= 1,000,00

For 100% of the estimation, 1 <= n <= 4,000,000

The input sequence is a permutation of {1,2...n}, corresponding to a legal binary tree.

Time: 2 sec

Memory: 256 MB

*/
//95/100 Test20 TLE
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#define maxN 4000010
int pre[maxN];
int post[maxN];
int N;
void getIn(int prel, int prer, int postl, int postr) {
	//output the inorder sequence determined by pre[prel,prer] and post[postl,postr]
	//we can assume that pre[prel,prer] and post[postl,postr] represent the same tree
	if (prer - prel == 0) {
		printf("%d ", pre[prel]);
		return;
	}
	if (prer - prel == 1) {//the question assume that such situation will not occur
		printf("error");
		return;
	}
	if (prer - prel == 2) {
		printf("%d %d %d ", pre[prel + 1], pre[prel], pre[prel + 2]);
		return;
	}
	int root = pre[prel];
	int prels, prele, prers, prere, postls, postle, postrs, postre;//we should determine this four values
	//left subtree's start and end in preorder and postorder,right subtree's start and end in preorder and postorder,
	prels = prel + 1;
	postls = postl;
	prere = prer;
	postre = postr - 1;
	int rroot = post[postre];
	for (prele = prels; pre[prele] != rroot; prele++);
	prele--;
	//now we know the length of left subtree(equals to prele - prels + 1)
	prers = prele + 1;
	postle = postls + prele - prels;
	postrs = postle + 1;
	getIn(prels, prele, postls, postle);
	printf("%d ", root);
	getIn(prers, prere, postrs, postre);
}
int main() {
	scanf("%d", &N);
	for (int i = 0; i < N; i++) {
		scanf("%d", &pre[i]);
	}
	for (int i = 0; i < N; i++) {
		scanf("%d", &post[i]);
	}
	getIn(0, N - 1, 0, N - 1);
	printf("\n");
	return 0;
}