#ifndef SKIP_LIST_GUARD_170720181942
#define SKIP_LIST_GUARD_170720181942


#include <algorithm>	// for_each()
#include <cstdlib>		// size_t	aligned_alloc()
#include <iterator>		// begin()/end()
#include <ostream>
#include <random>		// generation of the levels
#include <type_traits>	// conditional
#include <utility>		// std::pair	/ move / unique pointer
#include <vector>		// for head implementation

namespace skip_list {

	template<typename Key,typename T>
	class Skip_list {
	private:
		struct Skip_node;														// forward declaration because iterator class needs to know about the node
		std::vector<Skip_node*> head = std::vector<Skip_node*>(1, nullptr);		// element before first element containg pointers to all the first elements of each level
	public:
		using key_type = Key;
		using mapped_type = T;

		using value_type = std::pair<const key_type, mapped_type>;
		using size_type = std::size_t;

		template<typename Value_type>
		class iterator_base {
		public:
			using value_type = Value_type;
			using node_type = std::conditional_t<std::is_const_v<Value_type>, Skip_node const, Skip_node>;
			
			using difference_type = std::ptrdiff_t;
			using reference = value_type&;
			using pointer = value_type*;
			using iterator_category = std::forward_iterator_tag;

			iterator_base() = default;

			constexpr bool operator==(const iterator_base& b) const { return curr == b.curr; }
			constexpr bool operator!=(const iterator_base& b) const { return curr != b.curr; }

			iterator_base& operator++() noexcept
			{
				curr = curr->next[0];
				return *this;
			}

			iterator_base& operator++(int ) noexcept
			{
				iterator_base result(*this);
				++(*this);
				return *this;
			}

			constexpr reference operator*() noexcept { return curr->value; }
			constexpr pointer operator->() noexcept  { return &curr->value; }

		private:
			constexpr explicit iterator_base(node_type* pos) noexcept
				: curr{ pos }
			{
			}

			node_type* curr = nullptr;

			friend class Skip_list;		// to access curr in skiplist functions
		};

		using iterator = iterator_base<value_type>;
		using const_iterator = iterator_base<value_type const>;

		Skip_list() = default;					// constructor

		~Skip_list() noexcept					// destructor
		{
			free_all_nodes(head[0]);
		}

		Skip_list(const Skip_list& other)				// copy constructor
		{
			try{

				copy_nodes(other);
			}
			catch (...){
				free_all_nodes(head[0]);
				throw;
			}
		}

		Skip_list& operator=(const Skip_list& other)	// copy assignment
		{
			using std::swap;

			auto temp = other;
			swap(temp, *this);
			return *this;
		}

		friend void swap(Skip_list& a, Skip_list& b) noexcept
		{
			using std::swap;
			swap(a.head, b.head);
		}

		Skip_list(Skip_list&& other) noexcept						// move constructor
			:Skip_list{}
		{
			using std::swap;
			swap(*this, other);
		}

		Skip_list& operator=(Skip_list&& other) noexcept			// move assignment
		{
			using std::swap;
			swap(*this, other);
			return *this;
		}

		// ------------Iterators
		iterator begin() noexcept 
		{
			return iterator{ head[0] };
		}

		iterator end() noexcept
		{
			return iterator{ nullptr };
		}

		const_iterator begin() const noexcept
		{
			return const_iterator{ head[0] };
		}

		const_iterator end() const noexcept
		{
			return const_iterator{ nullptr };
		}

		const_iterator cbegin() const noexcept
		{
			return begin();
		}

		const_iterator cend() const noexcept
		{
			return end();
		}

		// ------------capacity
		bool empty() const noexcept
		{
			return (head[0] == nullptr);
		}

		size_type size() const noexcept			
			// Not conform to the STL because the time complexity is =O(n) instead of O(1)
		{
			Skip_list<Key, T>::size_type counter = Skip_list<Key, T>::size_type{};

			for (auto index = head[0]; index != nullptr; index = index->next[0], ++counter);

			return counter;
		}

		size_type max_size() const noexcept
		{
			return std::numeric_limits<size_type>::max();
		}

		// ------------element access
		mapped_type& operator[] (const key_type& key)
		{
			if (find(key) == end()) {		// insert new element with default value if not present
				insert(std::pair<key_type, mapped_type>{key, mapped_type{}});
			}
			return find(key)->second;
		}
		mapped_type& operator[] (key_type&& key)
		{
			if (find(key) == end()) {		// insert new element with default value if not present
				insert(std::pair<key_type, mapped_type>{key, mapped_type{}});
			}
			return find(key)->second;
		}

		// ------------modifiers
		std::pair<iterator, bool> insert(const value_type& value);
//		std::pair<iterator, bool> insert(value_type&& value);

		size_type erase(const key_type& key);	
//		iterator erase(const_iterator position);
//		iterator erase(iterator position);

		void clear() noexcept				// erase all elements 
		{
			free_all_nodes(head[0]);
			head.assign(1, nullptr);
		}

		// ------------Operations
		iterator find(const key_type& key)
		{
			return iterator{ _find(key) };
		}

		const_iterator find(const key_type& key) const
		{
			return const_iterator{ _find(key) };
		}

		size_type count(const key_type& key) const
		{
			return find(key) != end() ? 1 : 0;
		}

		size_type top_level() const { return head.size(); }		// maximum height the Skip_list has reached

		void debug_print(std::ostream& os) const;			// show all the levels for debug only. can this be put into skiplist_unit_tests ?
	private:
		class Skip_node_deleter
		{
		public:
			auto operator()(Skip_node* pointer) const noexcept
			{
				if (pointer) {
					pointer->~Skip_node();

					auto raw_pointer = reinterpret_cast<std::byte*>(pointer);
					delete[] raw_pointer;
				}
			}
		};

		using node_ptr = std::unique_ptr<Skip_node*, Skip_node_deleter>;

		size_type generate_level() const;
		static bool next_level() noexcept;

		struct Skip_node{
			value_type value;		// key / T
			size_type levels;
			Skip_node* next[1];
		};

		Skip_node* _find(const key_type& key) const;

		static auto allocate_node(value_type value, size_type levels)->std::unique_ptr<Skip_node*, Skip_node_deleter>;

		void copy_nodes(const Skip_list& other);					
		static void free_all_nodes(Skip_node* head) noexcept;		
	};

	template<typename Key, typename T>
	std::pair<typename Skip_list<Key,T>::iterator, bool> Skip_list<Key,T>::insert(const value_type& value)
		// if key is already present the position of that key is returned and false for no insert
		//
		// if new key inserted or value of given key was replaced return next pos as iterator and indicate change with true
		// otherwise return iterator end() and false
	{
		enum Insert_state {
			no_action,
			replaced,
			inserted
		};

		Insert_state state = Insert_state::no_action;

		const auto insert_level = generate_level();     // top level of new node
	
		auto level = head.size();
		auto next = head.data();

		Skip_list::iterator insert_pos;

		auto index = level - 1;
		auto node = next[index];

		bool insert_new_node = false;
		bool replace_old_node = false;

		while (level > 0) {
			index = level - 1;
			node = next[index];

			// the horizontal end is reached or we are already past the value on this level
			if ((node == nullptr || node->value.first > value.first)) {

				if (level <= insert_level) {		// level of the new insert node is reached so end function
					insert_new_node = true;
					break;
				}
				else {
					--level;
				}
			}
			else if (node->value.first == value.first) {
				// key already present, keep node with more levels
				//  -> no need to insert new node into list if not needed
				//  -> if insert_node->levels > node->levels, we already modified the list
				//     so continuing and removing the other node seems like the easier option
				//     (compared to retracing where links to insert_node have been made)

				if (node->levels >= insert_level) {
					node->value.second = value.second;

					state = Insert_state::replaced;

					return std::make_pair(Skip_list::iterator{ node }, true);
				}
				else {
					replace_old_node = true;
					break;
				}
			}
			else {
				next = node->next;
			}
		}

		auto insert_node = allocate_node(value, insert_level);

		if (head.size() < insert_level) {
			head.resize(insert_level, nullptr);
		}

		if (insert_new_node) {
			// Pure insertion case:
			insert_node->next[index] = next[index];
			next[index] = insert_node.release(); // note the added ".release()"
			--level;

			state = Insert_state::inserted;
		}
		else if (replace_old_node) {
			// Replacing old node case:
			auto const old_node = std::unique_ptr<Skip_node, Skip_node_deleter>{ node, Skip_node_deleter{ *this } }; // store the old node in a unique ptr to be cleaned up
			insert_node->next[index] = node->next[index];
			next[index] = insert_node.release(); // note the added ".release()"
			--level;

			state = Insert_state::replaced;
		}

		insert_pos = Skip_list::iterator{ insert_node };

		return std::make_pair(insert_pos, added);


		//const auto insert_level = generate_level();     // top level of new node
		//const auto insert_node = allocate_node(value, insert_level);
		//Skip_list::Skip_node* old_node = nullptr;

		//if (head.size() < insert_level) {
		//	head.resize(insert_level,nullptr);
		//}

		//auto level = head.size();
		//auto next = head.data();

		//Skip_list::iterator insert_pos;
		//bool added = false;

		//while (level > 0) {
		//	const auto index = level - 1;
		//	auto node = next[index];

		//	if (node == nullptr || node->value.first > value.first) {   //compare by key

		//		if (level <= insert_level) {

		//			insert_node->next[index] = next[index];
		//			next[index] = insert_node;

		//			if (!added) {
		//				insert_pos = Skip_list::iterator{ next[index] };
		//				added = true;
		//			}
		//		}
		//		--level;
		//	}
		//	else if (node->value.first == value.first) {
		//		// key already present, keep node with more levels
		//		//  -> no need to insert new node into list if not needed
		//		//  -> if insert_node->levels > node->levels, we already modified the list
		//		//     so continuing and removing the other node seems like the easier option
		//		//     (compared to retracing where links to insert_node have been made)

		//		if (node->levels >= insert_level) {
		//			node->value.second = value.second;
		//			free_node(insert_node);

		//			return std::make_pair(Skip_list::iterator{ node }, true);
		//		}

		//		old_node = node;

		//		insert_node->next[index] = node->next[index];
		//		next[index] = insert_node;
		//		--level;
		//	}
		//	else {
		//		next = node->next;
		//	}
		//}

		//if (old_node != nullptr) {
		//	free_node(old_node);
		//}

		//return std::make_pair(insert_pos, added);


	}

	template<typename Key, typename T>
	typename Skip_list<Key,T>::size_type Skip_list<Key,T>::erase(const key_type& key)
		// starts search on the highest lvl of the Skip_list
		// if a node with the erase key is found the algorithm goes 
		// down until the lowest lvl.
		// on the way down all links with the key in the list are removed
		// on the lowest lvl the current node which contains the erase key is deleted
		//
		// the return type indicates how many elements are deleted (like std::map)
		// it can become only 0 or 1
	{
		Skip_node* node = nullptr;

		auto level = head.size();
		auto next = head.data();

		while (level > 0) {

			const auto link_index = level - 1;

			if (!next[link_index] || next[link_index]->value.first > key) {
				--level;
			}
			else if (next[link_index]->value.first == key) {
				node = next[link_index];
				next[link_index] = node->next[link_index];
				--level;
			}
			else {
				next = next[link_index]->next;
			}
		}

		if (node) {		// element to erase was found and taken out of list
			while (head.size() > 1 && head.back() == nullptr) head.pop_back();
			auto const old_node = std::unique_ptr<Skip_node, Skip_node_deleter>{ node, Skip_node_deleter{ *this } }; // store the old node in a unique ptr to be cleaned up
			return 1;
		}
		else {
			return 0;
		}
	}

	template<typename Key, typename T>
	typename Skip_list<Key, T>::Skip_node* Skip_list<Key, T>::_find(const key_type& key) const
	{
		auto level = head.size();
		auto next = head.data();

		while (level > 0) {
			const auto index = level - 1;

			if (!next[index] || next[index]->value.first > key) {
				--level;
			}
			else if (next[index]->value.first == key) {
				return next[index];
			}
			else {
				next = next[index]->next;
			}
		}
		return nullptr;
	}

	template<typename Key, typename T>
	void Skip_list<Key,T>::debug_print(std::ostream& os) const
		//messy debug routine to print with all available layers
	{
		if (head[0] == nullptr) {
			os << "empty" << '\n';
			return;
		}

		auto level = head.size();
		auto next = head.data();

		os << "lvl: " << level << " ";

		while (level > 0) {

			const auto index = level - 1;

			if (!next[index]) {
				os << '\n';
				--level;

				if (level > 0) {
					os << "lvl: " << index << " ";
					next = head.data();		// point back to begining
				}
			}
			else {
				os << next[index]->value.first << '/' << next[index]->value.second << ' ';
				next = next[index]->next;
			}
		}
	}

	template<typename Key, typename T>
	typename Skip_list<Key,T>::size_type Skip_list<Key,T>::generate_level() const
		// generate height of new node
	{
		size_type new_node_level = size_type{};

		do {
			++new_node_level;
		} while (new_node_level <= head.size() && next_level());

		return new_node_level;
	}

	template<typename Key, typename T>
	bool Skip_list<Key,T>::next_level() noexcept
		// arround 50% chance that next level is reached
	{
		static auto engine = std::mt19937{ std::random_device{}() };
		static auto value = std::mt19937::result_type{ 0 };
		static auto bit = std::mt19937::word_size;

		if (bit >= std::mt19937::word_size) {
			value = engine();
			bit = 0;
		}

		const auto mask = std::mt19937::result_type{ 1 } << (bit++);
		return value & mask;
	}

	template<typename Key, typename T>
	auto Skip_list<Key, T>::allocate_node(value_type value, size_type levels) -> std::unique_ptr<Skip_node*, Skip_node_deleter>
		// expects levels > 0
	{
		const auto node_size = sizeof(Skip_node) + (levels - 1) * sizeof(Skip_node*);

		auto node = std::make_unique<std::byte[]>(node_size);

		new(node.get()) Skip_node{ std::move{value}, levels, nullptr };

		return node_ptr{ reinterpret_cast<Skip_node*>(node.release()) };
	}

	template<typename Key, typename T>
	void Skip_list<Key,T>::copy_nodes(const Skip_list& other)
		// precondition: head isn't owner of any nodes
	{
		head.assign(other.head.size(), nullptr);

		auto tail = std::vector<Skip_node**>{};

		tail.reserve(head.size());
		std::for_each(std::begin(head), std::end(head), [&](auto&& link) { tail.push_back(&link); });

		auto nodes = std::vector<node_ptr>{};
		nodes.reserve(other.size());

		for (auto node = other.head[0]; node != nullptr; node = node->next[0]) {

			const auto copy_node = allocate_node(node->value, node->levels);

			for (auto i = 0u; i < copy_node->levels; ++i) {
				*tail[i] = copy_node;
				tail[i] = &copy_node->next[i];
			}
		}

		std::for_each(std::begin(tail), std::end(tail), [](auto link) { *link = nullptr; });

		for (auto&& node : nodes)
			node.release();
	}

	template<typename Key, typename T>
	void Skip_list<Key,T>::free_all_nodes(Skip_node* head) noexcept
	{
		for (auto index = head; index != nullptr;) {
			const auto temp = index;
			index = index->next[0];
			free_node(temp);
		}
	}
}
#endif