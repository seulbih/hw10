/*
 * Binary Search Tree #2
 *
 * Data Structures
 *
 * Department of Computer Science
 * at Chungbuk National University
 *
 */

#include <stdio.h>
#include <stdlib.h>



typedef struct node {
	int key;
	struct node *left;
	struct node *right;
} Node;

/* for stack */
#define MAX_STACK_SIZE		20
Node* stack[MAX_STACK_SIZE];
int top = -1;

Node* pop();
void push(Node* aNode);

/* for queue */
#define MAX_QUEUE_SIZE		20
Node* queue[MAX_QUEUE_SIZE];
int front = -1;
int rear = -1;

Node* deQueue();
void enQueue(Node* aNode);


int initializeBST(Node** h);

/* functions that you have to implement */
void recursiveInorder(Node* ptr);	  /* recursive inorder traversal */
void iterativeInorder(Node* ptr);     /* iterative inorder traversal */
void levelOrder(Node* ptr);	          /* level order traversal */
int insert(Node* head, int key);      /* insert a node to the tree */
int deleteNode(Node* head, int key);  /* delete the node for the key */
int freeBST(Node* head); /* free all memories allocated to the tree */

/* you may add your own defined functions if necessary */
void isEmpty(Node* ptr);

/*void printStack();*/


int main()
{
	char command;
	int key;
	Node* head = NULL;

	do{
		printf("\n\n");
		printf("----------------------------------------------------------------\n");
		printf("                   Binary Search Tree #2                        \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize BST       = z                                       \n");
		printf(" Insert Node          = i      Delete Node                  = d \n");
		printf(" Recursive Inorder    = r      Iterative Inorder (Stack)    = t \n");
		printf(" Level Order (Queue)  = l      Quit                         = q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		fflush(stdout);
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z':
			initializeBST(&head);
			break;
		case 'q': case 'Q':
			freeBST(head);
			break;
		case 'i': case 'I':
			printf("Your Key = ");
			fflush(stdout);
			scanf("%d", &key);
			insert(head, key);
			break;
		case 'd': case 'D':
			printf("Your Key = ");
			fflush(stdout);
			scanf("%d", &key);
			deleteNode(head, key);
			break;

		case 'r': case 'R':
			isEmpty(head->left);
			recursiveInorder(head->left);
			break;
		case 't': case 'T':
			isEmpty(head->left);
			iterativeInorder(head->left);
			break;

		case 'l': case 'L':
			isEmpty(head->left);
			levelOrder(head->left);
			break;

		case 'p': case 'P':
			/*printStack();*/
			break;

		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

int initializeBST(Node** h) {

	/* if the tree is not empty, then remove all allocated nodes from the tree*/
	if(*h != NULL)
		freeBST(*h);

	/* create a head node */
	*h = (Node*)malloc(sizeof(Node));
	(*h)->left = NULL;	/* root */
	(*h)->right = *h;
	(*h)->key = -9999;

	top = -1;

	front = rear = -1;

	return 1;
}

void isEmpty(Node* ptr){ //빈 트리인지 구분(순회시 사용)
	if(ptr==NULL){
		printf("tree is empty!\n");
		return ;
	}
}

void recursiveInorder(Node* ptr)
{

	if(ptr!=NULL) {
		recursiveInorder(ptr->left);
		printf(" [ %d ] ", ptr->key);
		recursiveInorder(ptr->right);
	}
}

/**
 * textbook: p 224s
 */
void iterativeInorder(Node* node)
{
		while(1){
			for(; node; node=node->left){
				push(node);
			}
			node=pop();
			if(!node)
				break;
			printf(" [ %d ] ", node->key);
			node=node->right;
		}
}

/**
 * textbook: p 225
 */
void levelOrder(Node* ptr)
{
	enQueue(ptr);
	while(1){
		ptr=deQueue();
		if(ptr){
			printf(" [ %d ] ", ptr->key);
			if(ptr->left)
				enQueue(ptr->left);
			if(ptr->right)
				enQueue(ptr->right);
		}
		else
			break;
	}
}


int insert(Node* head, int key)
{
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->key = key;
	newNode->left = NULL;
	newNode->right = NULL;

	if (head->left == NULL) {
		head->left = newNode;
		return 1;
	}

	/* head->left is the root */
	Node* ptr = head->left;

	Node* parentNode = NULL;
	while(ptr != NULL) {

		/* if there is a node for the key, then just return */
		if(ptr->key == key) return 1;

		/* we have to move onto children nodes,
		 * keep tracking the parent using parentNode */
		parentNode = ptr;

		/* key comparison, if current node's key is greater than input key
		 * then the new node has to be inserted into the right subtree;
		 * otherwise the left subtree.
		 */
		if(ptr->key < key)
			ptr = ptr->right;
		else
			ptr = ptr->left;
	}

	/* linking the new node to the parent */
	if(parentNode->key > key)
		parentNode->left = newNode;
	else
		parentNode->right = newNode;
	return 1;
}


int deleteNode(Node* head, int key)
{
	if(head->left==NULL){ //빈 트리일 경우
				printf("tree is empty!\n");
				return 0;
			}

	Node *parent, *p, *t, *tparent;
	Node *child;

	parent =NULL; //부모노드
	p=head->left;

	while(p!=NULL&&p->key!=key){ //노드탐색
		parent=p;
		if(key<p->key) //key가 root노드보다 작은 경우 왼쪽 서브트리로
			p=p->left;
		else //큰 경우 오른쪽 서브트리로 이동하여 반복
			p=p->right;
	}

	if(p==NULL){ //p가 NULL인 경우, 트리에서 key를 찾지 못한 경우
		printf("%d is not in tree\n", key);
		return 0; //종료
	}

	/*삭제하고자 하는 노드가 단말노드일 때*/
	if(p->left==NULL&&p->right==NULL){ //단말노드인지 판별
		if(parent!=NULL){ //단말노드인 경우, 부모노드가 있는 경우(루트노드가 아닌 경우)
			if(parent->left==p) //key가 부모노드의 왼쪽 자식인 경우
				parent->left=NULL; //부모노드의 left를 NULL로 설정
			else //key가 오른쪽 자식인 경우
				parent->right=NULL; //부모노드의 right를 NULL로 설정
		}
		else
			head->left=NULL; //단말노드이면서 루트노드인경우(노드가 하나뿐인 경우) head의 left를 NULL로
	}

	/*삭제하고자 하는 노드가 하나의 자식만을 가질 때*/
	else if(p->left==NULL || p->right==NULL){ //둘중 하나는 NULL인 경우
		if(p->left != NULL) //왼쪽 자식만을 갖는 ㄴ경우
			child=p->left; //child=왼쪽 자식
		else //오른쪽 자식만을 갖는 경우
			child=p->right;//child=오른쪽 자식

		if(parent!=NULL){ //부모노드가 있는 경우(루트노드가 아닌 경우)
			if(parent->left==p) //key가 왼쪽 자식인 경우
				parent->left=child; //부모노드의 왼쪽 자식을 key의 child로 대체
			else //key가 오른쪽 자식인 경우
				parent->right=child; //부모노드의 오른쪽 자식을 key의 child로 대체
		}
		else
			head->left=child; //루트노드인 경우 child로 대체
	}

	/*삭제하고자 하는 노드가 두개의 자식을 가질 때, 오른쪽 서브트리에서 가장 작은 값으로 대체되도록 함*/
	else{
		tparent=p;
		t=p->right;

		while(t->left != NULL){ //t의 왼쪽이 존재 : key 오른쪽 서브트리의 최솟값이 존재
			tparent=t; //왼쪽노드로 이동하며 서브트리의 최솟값 찾기
			t=t->left;
		}

		if(tparent->right == t){ //최솟값이 부모노드의 오른쪽 자식인 경우
			tparent->right=t->right; //부모노드가 최솟값의 오른쪽을 그대로 가리키도록 함
		}
		else //최솟값이 부모노드의 왼쪽 자식인 경우
			tparent->left=t->right; //부모노드가 최솟값의 왼쪽을 그대로 가리키도록 함

		p->key=t->key; //삭제할 노드의 key값을 최솟값의 key값으로
		p=t; //삭제할 노드는 최솟값노드로 설정 (최솟값을 찾은 자리에 두고 최솟값이 있던 자리를 삭제함)
	}
	free(p); //노드 메모리 해제
	return 0;
}


void freeNode(Node* ptr)
{
	if(ptr) {
		freeNode(ptr->left);
		freeNode(ptr->right);
		free(ptr);
	}
}

int freeBST(Node* head)
{

	if(head->left == head)
	{
		free(head);
		return 1;
	}

	Node* p = head->left;

	freeNode(p);

	free(head);
	return 1;
}



Node* pop()
{
	Node* p=NULL;
		if(top>=0)
			p=stack[top--];
		return p;
}

void push(Node* aNode)
{
	if(top<MAX_STACK_SIZE-1&& aNode !=NULL)
		stack[++top] = aNode;
}



Node* deQueue()
{
	Node *p=NULL;
	if(front!=rear)
		p=queue[++front];
	return p;
}

void enQueue(Node* aNode)
{
	if((rear!= MAX_QUEUE_SIZE-1)&&(aNode != NULL)){
		queue[++rear]=aNode;
	}
}







