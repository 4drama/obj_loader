#ifndef INDEX_LIST__
#define INDEX_LIST__

#include <stdexcept>

template<class T>
class index_list{
public:
	index_list(){
		head = nullptr;
	}

	std::size_t add(T data_, T** res){
		std::size_t count = 0;
		
		if(this->head == nullptr){
			this->head = new node;
			this->head->index = 0;
			this->head->data = data_;
			this->head->next = nullptr;
			if(res != nullptr)
				*res = &this->head->data;
			return this->head->index;
		}
		
		node *prev = nullptr;
		node *curr = this->head;
		
		while(true){
			if(curr == nullptr) {
				curr = new node;
				curr->index = count;
				curr->data = data_;
				curr->next = nullptr;
				
				prev->next = curr;
				break;
			} else if(curr->index == count) {				
				++count;
				prev = curr;
				curr = curr->next;
			} else if(curr->index > count) {
				node *new_node = new node;
				new_node->index = count;
				new_node->data = data_;
				new_node->next = curr;
				if(curr == this->head)
					this->head = new_node;
				else
					prev->next = new_node;
				curr = new_node;
				break;
			} else if(curr->index < count) {
				throw std::logic_error("curr->index < count");
			}
		}
		
		if(res != nullptr)
			*res = &curr->data;
		return count;
	}
	
	void remove(std::size_t index){
		node *prev = nullptr;
		node *curr = this->head;
		
		while(true){
			if(curr == nullptr){
				break;
			}else if(curr->index == index){
				if(this->head == curr)
					this->head = curr->next;
				else 
					prev->next = curr->next;
				delete curr;
				break;
			}else{
				prev = curr;
				curr = curr->next;
			}
		}
	}
	
	void clean(){
		node *prev = this->head;
		node *curr = prev->next;
		
		while(curr != nullptr){
			prev = curr;
			curr = curr->next;
			delete prev;
		}
		this->head = nullptr;
	}
	
	std::size_t first(T **data){
		current = this->head;
		*data = &current->data;
		return current->index;
	}
	
	std::size_t next(T **data){
		current = current->next;
		if(current == nullptr)
			current = this->head;
		*data = &current->data;
		return current->index;
	}
	
private:
	struct node;
	node *head;
	node *current;
	
	struct node{
		std::size_t index;
		T data;
		index_list::node *next;
	};
};

#endif