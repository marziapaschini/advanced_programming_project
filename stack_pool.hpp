#include <vector>
#include <iterator>

template <typename stack_pool, typename T, typename N>
class _iterator{
  N current; // indice nodo
  stack_pool* pool_ptr; // puntatore alla stack_pool

  public:
   using value_type = T;
   using reference = value_type&;
   using pointer = value_type*;
   using difference_type = std::ptrdiff_t;
   using iterator_category = std::forward_iterator_tag;

  explicit _iterator(N x, stack_pool* y) : current{x}, pool_ptr{y} {}
  reference operator *() const { return pool_ptr->value(current); }
  pointer operator ->() { return &**this; }
  _iterator& operator++() {
   current = pool_ptr->next(current);
   return *this;
  }
  friend bool operator ==(const _iterator& i1, const _iterator& i2){
    return (i1.current == i2.current) && (i1.pool_ptr == i2.pool_ptr);}
  friend bool operator !=(const _iterator& i1, const _iterator& i2){
    return !(i1==i2);}
};

template <typename T, typename N = std::size_t>
class stack_pool{
  struct node_t{
    T value;
    N next;

  };
  std::vector<node_t> pool;
  using stack_type = N;
  using value_type = T;
  using size_type = typename std::vector<node_t>::size_type;
  stack_type free_nodes; // at the beginning, it is empty
  
  node_t& node(stack_type x) noexcept { return pool[x-1]; }
  const node_t& node(stack_type x) const noexcept { return pool[x-1]; }

	// auxiliary function for _push function
	N next_free(N x) noexcept {
		x = free_nodes;
		free_nodes = next(free_nodes);
		return x;
	}
	
	// auxiliary function for push functions
  template <typename A>
  N _push(A&& val, N head);

  public:
  stack_pool() noexcept = default;
  explicit stack_pool(size_type n){ reserve(n); } // reserve n nodes in the pool
    
  using iterator = _iterator<stack_pool, T, N>;
  using const_iterator = _iterator<stack_pool, const T, N>;

  iterator begin(stack_type x){ return iterator(x, this); }
  iterator end(stack_type ){ return iterator(end(), this); }
    
  const_iterator begin(stack_type x) const { return const_iterator{x, this}; }
  const_iterator end(stack_type ) const { return const_iterator{end(), this}; } 
  
  const_iterator cbegin(stack_type x) const { return const_iterator{x, this}; }
  const_iterator cend(stack_type ) const { return const_iterator{end(), this}; }
    
  stack_type new_stack() noexcept { return end(); } // return an empty stack

  void reserve(size_type n){ // reserve n nodes in the pool
   pool.reserve(n);
  }
  size_type capacity() const noexcept { // the capacity of the pool
   return pool.capacity();
  }

  bool empty(stack_type x) const noexcept { return x==end(); }

  stack_type end() const noexcept { return stack_type(0); }

  T& value(stack_type x) noexcept { return node(x).value; }
  const T& value(stack_type x) const noexcept { return node(x).value; }

  stack_type& next(stack_type x) noexcept { return node(x).next; }
  const stack_type& next(stack_type x) const noexcept { return node(x).next; }

  stack_type push(const T& val, stack_type head){ return _push(val, head);}
  stack_type push(T&& val, stack_type head){ return _push(std::move(val), head);}
  
  stack_type pop(stack_type x) noexcept { // delete first node
   stack_type y = free_nodes;
   free_nodes = x;
   x = next(x);
   next(free_nodes) = y;
   return x;
  }

  stack_type free_stack(stack_type x) noexcept { // free entire stack
   while(!empty(x))
    x = pop(x);
   return x;
  }
};

template <typename T, typename N>
template <typename A>
N stack_pool<T, N>::_push(A&& val, N head){
 if(empty(free_nodes)==true){
		pool.push_back(node_t{std::forward<A>(val), head});
    return stack_type(pool.size());
	}
	N x = next_free(x);
	node(x)=node_t{std::forward<A>(val),head}; // initialization list
	return x;
}
 
