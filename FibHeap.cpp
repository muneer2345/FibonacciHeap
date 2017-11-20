#include <stdio.h>
#include <vector> 
#include <queue>
#include <map> 
#include <set>
#include <sstream> 
#include <iostream> 
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <stack>
#include <cmath>
using namespace std;
 
//Node Structure 
struct FibonacciHeapNode{
	string hashTag;              
	FibonacciHeapNode *ParentNode;
	FibonacciHeapNode *right;    // attributes of node . i.e. Pointers and data fields
	FibonacciHeapNode *left;
	FibonacciHeapNode *ChildNode;
	int count;
	int degree;
	bool childCut;
	
	FibonacciHeapNode(int hashCount, string hshTag){         //constructor to initialize the node
		ParentNode = NULL;
		right = this;
        count = hashCount;
		degree =0;
		childCut = false;
		hashTag = hshTag;
		left = this;
		ChildNode = NULL;
		}
};

class FibonacciHeapClass {
	private:
		FibonacciHeapNode * maxPointer;                                       
		 int nNodes;   // Defining  Heap Max Pointer , no of nodes and Hash Table
		map<string, FibonacciHeapNode*> hashTable;
				
	public:
	FibonacciHeapClass() : maxPointer (NULL) , nNodes() {};

	void insertNode(int hshCount, string hshTag){
		
		if(hashTable.find(hshTag) == hashTable.end()){ // Check if the hashtag already exists in the table.. if it is not present..then create the node and add it to the top level list.. else increase key operation
	
			FibonacciHeapNode *newFibNode = new FibonacciHeapNode(hshCount, hshTag);
			if(!maxPointer){
				maxPointer = newFibNode;
			}
			else{
				if(!maxPointer && !newFibNode){
					maxPointer= NULL;
				}
				else if(!maxPointer){
					maxPointer= newFibNode;
				}
				FibonacciHeapNode *swap;
				swap = maxPointer->right;
				maxPointer->right = newFibNode->right;
				newFibNode->right = swap;                        // insert into top level circular list
				maxPointer->right->left = maxPointer;
				newFibNode->right->left = newFibNode;
				maxPointer= (maxPointer->count > newFibNode->count)? maxPointer: newFibNode;   // update max ptr based on count of newly inserted node...
			}
			hashTable[hshTag]= newFibNode; //update hash table
			nNodes++;   // update number of nodes
		}
		else{
			increaseKey(hshCount, hshTag); // increase key if hash tag is already present in the table
		}
	}
	
	int removeMax(){
		if(!maxPointer){
			cout<<"Empty heap"<<endl;
			return 0;
		}
		FibonacciHeapNode *deleteNode = maxPointer;
		nNodes--;    // decrease number of nodes
		hashTable.erase(maxPointer->hashTag); // remove entry from hash table
		
		maxPointer = removeNode(maxPointer); // remove node from circular list 

		if(deleteNode -> ChildNode){                                   //if deleted node has children.. update parent pinters
			FibonacciHeapNode *cnode  = deleteNode->ChildNode;       //Child Node
			FibonacciHeapNode *start = cnode;
		
				cnode->ParentNode = NULL;
				cnode = cnode->right;
	
			while(cnode != start){
				cnode->ParentNode = NULL;
				cnode = cnode->right;
			}
			if(!deleteNode->ChildNode && !maxPointer){
				maxPointer= NULL;
			}
			else if(!maxPointer){
				maxPointer= deleteNode->ChildNode;
			}
			FibonacciHeapNode *swap;
			swap = deleteNode->ChildNode->right;
			deleteNode->ChildNode->right = maxPointer->right;                        // Update Max node and child node pointers
			maxPointer->right = swap;
			deleteNode->ChildNode->right->left = deleteNode->ChildNode;       
			maxPointer->right->left = maxPointer;
			
			maxPointer= (deleteNode->ChildNode->count > maxPointer->count)? deleteNode->ChildNode: maxPointer;
		}
		if (!maxPointer)
			 return deleteNode->count;
		Combine();		                      // Pair wise combine -- Combining Equal Degree Nodes
		return deleteNode->count;
	}
		FibonacciHeapNode * removeNode(FibonacciHeapNode * node){
			FibonacciHeapNode * ret = node->right;
			if (ret == node){
				return NULL;
			}
			node->left->right = node->right;  // removing node from circular list
			node->right->left = node->left;
			node->left = node;
			node->right = node;
			return ret;
	}	
	
	void Combine(){
		
		if (maxPointer->right == maxPointer) {
			return;
		}
		vector<FibonacciHeapNode *> a;
		FibonacciHeapNode *curr = maxPointer;
		FibonacciHeapNode *start = curr;
	
			a.push_back(curr);
			curr = curr->right;
	
		while(curr != start){
			a.push_back(curr);             // Initialize the vector with the root level nodes..
			curr = curr->right;
		}

		vector<FibonacciHeapNode *> degrees (int(log2(nNodes)) + 1);     //degree vector
		for (int i = 0; i < a.size(); i++){
			curr = a[i];
			if (curr->degree >= degrees.size()){
				degrees.resize(curr->degree+1);	              // Adjust degree vector
			} 
			if (degrees[curr->degree]){
				while (degrees[curr->degree]){					
					
					FibonacciHeapNode *min, *max;
					if (degrees[curr->degree]->count > curr->count){
						min = degrees[curr->degree];                     // min max degree nodes
						max = curr;
					} else {
						min = curr;
						max = degrees[curr->degree];
					}
					FibonacciHeapNode * ret = max->right;
					max->right->left = max->left;              // Update Pointer of Max Node
					max->left->right = max->right;
					max->right = max;
					max->left = max;

					if(!min->ChildNode && !max){
						min->ChildNode= NULL;
					}
					else if(!min->ChildNode){
						min->ChildNode= max;
					}
					FibonacciHeapNode *swap;

					swap = min->ChildNode->right;
					min->ChildNode->right = max->right;
					max->right = swap;

					min->ChildNode->right->left = min->ChildNode;           // Update Pointers of Min Node
					max->right->left = max;

					min->ChildNode= (min->ChildNode->count > max->count)? min->ChildNode: max;
					max->ParentNode = min;
					min->degree++;   // Increase degree of Min Node
					
					curr= min;
					degrees[curr->degree - 1] = NULL;
					if(curr->count >= maxPointer->count){
						 maxPointer = curr;
					}
					if (curr->degree >= degrees.size()){
						degrees.resize(curr->degree+1);	
					} 
				}
				
				degrees[curr->degree] = curr;

			}
			else {
				degrees[curr->degree] = curr;
				if (curr->count >= maxPointer->count){
					maxPointer = curr;	
				}
			}
		}
	}

	void increaseKey(int count, string hsTag){

		FibonacciHeapNode *curr = hashTable[hsTag];
		
		curr->count = curr->count + count;   // update count
		hashTable.erase(hsTag); // remove entry from hashtable
		hashTable[hsTag] = curr; // update entry with new value into hashtable
		
		if (curr->ParentNode && (curr->ParentNode->count < curr->count)){
				cascadingCut(curr);     // Cascading Cut if the count is more than the parent
		}
		if (curr->count > maxPointer->count){
			maxPointer=curr;	
		} 
	}
	void cascadingCut(FibonacciHeapNode *node){

		if (!node->ParentNode) {
			return;
		}
		node->childCut =false;
		FibonacciHeapNode *swap;

		if (node->ParentNode->ChildNode ==node){
			if (node->right !=node){
				node->ParentNode->ChildNode =node->right;	
			} 
			else{
				node->ParentNode->ChildNode =NULL;	
				
			} 
		}
		node->right->left = node->left;
		node->left->right = node->right;               // Cut it from the sibling list
		node->right = node;
		node->left = node;

		if(!maxPointer && !node){
			maxPointer= NULL;
		}
		else if(!maxPointer){
			maxPointer= node;
		}


		swap = maxPointer->right;
		maxPointer->right = node->right;             // Add it to the topmost list
		node->right = swap;

		maxPointer->right->left = maxPointer;
		node->right->left = node;

		maxPointer=(maxPointer->count > node->count)? maxPointer: node;          // Update Max Pointer

		node->ParentNode->degree--;                           // Decrease Parent Degree
		
		if (node->ParentNode->childCut) {                // Call Child cut for parent if it is set to True
		
			cascadingCut(node->ParentNode);	
		}
		else{
			
			node->ParentNode->childCut=true;         // Else mark parent as true
		} 
		node->ParentNode=NULL;
	}
	
	string getMax(){
		if (maxPointer) {

			return maxPointer->hashTag;   // returns the hashtag of the max node
		}
		
	}
	
	int getMaxCount(){
		if (maxPointer) {

			return maxPointer->count;    // returns the count of the max node
		}
		
	}

};
int main(int argc, char* argv[]) {
	
	FibonacciHeapClass heap;
	string line;
	string Tag;
	int count;
	int query_count;
	int flag;
	stack <string> StringStack;
    stack <int> CountStack;
	ifstream infile(argv[1]);
	ofstream outputfile;
	
	outputfile.open("output.txt", ios::app);
	
	while (std::getline(infile, line))
	{
	    std::istringstream iss(line);   // read line by line
	    iss>>Tag;
	    if(Tag[0]=='#'){

	    	iss>>count;    // set flag 1 if beginning with #
	    	flag=1;
	    }
	    else if(Tag[0]!='#' && Tag!="STOP"){
	    	 stringstream convert(Tag);         // set flag 2 if it is a number i.e. not #
	    	 convert>>query_count;
	    	 flag=2;
	    	
	    }
	    else if(Tag=="STOP" || Tag=="stop"){        // break if u encounter stop            
	    	break;
	    }
	  
	 	if(flag==2){              //if flag 2 remove max = no of queries and re insert
	 		
	 		while(query_count--){
	 		
	 			if(query_count!=0){
	 				outputfile<<heap.getMax().substr(1,heap.getMax().length())<<",";   // write to output
	 			}
	 			else{
	 				outputfile<<heap.getMax().substr(1,heap.getMax().length());        // write to output 
	 			}
	 		
	 			StringStack.push(heap.getMax());
	 			CountStack.push(heap.getMaxCount());    // Store in stack to reinsert the nodes ( after remove max)
	 			heap.removeMax();
	 		}
	 		outputfile<<endl;
	 		while(StringStack.size()>0){
	 			heap.insertNode(CountStack.top(), StringStack.top());          // Re insert the nodes 
	 			StringStack.pop();
	 			CountStack.pop();
	 		}
	 	}
		  else if(flag==1){                                       // if flag 1 create node

	     	heap.insertNode(count, Tag);
	 	}
	}
}