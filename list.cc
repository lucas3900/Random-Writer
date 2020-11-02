k/******************************************
* list.cc
* (Stub by Alistair Campbell)
* rev. 2019.03
*
* NAME:Lucas Barusek
* DATE: 4/4/2019
*** This program is an implementation of a list.
    It allows you to add, remove and index the list.
    You can get the size, the index of a specific
    item, and the item at a specific index in
    the list. Uses nodes to store the items.
***
*
********************************************/

#include "list.h"
#include <cassert>

// List node constructors provided by Prof. Campbell

list::node::node() : prev(nullptr), next(nullptr) {}

list::node::node(const element & d) : prev(nullptr), data(d), next(nullptr) {}

list::node::node(node *p, node *n) : prev(p), next(n) {}

list::node::node(node *p, const element & d, node * n):
  prev(p), data(d), next(n) {}

// List constructors/destructor/assignment also provided.
// No need to change these.

list::list()
{
  _init();
}

list::~list()
{
  _destroy();
}

list::list(const list & orig)
{
  _copy(orig);
}

list & list::operator=(const list & rhs)
{
  if (this != &rhs) {
    _destroy();
    _copy(rhs);
  }
  return *this;
}

// List method stubs provided.  Here's where you come in. You delete this
// comment and fill in the method bodies.  *Delete any existing code in
// these!  That code is only there to make it compile out of the box.*


void list::_init(){
  /* list constructor. Creates an empty list with front and rear
  sentinels and a _current pointer pointing at rear */

  //sets the size and the _current index of the empty list
  _size = 0;
  _current_index = 0;

  //creates the _front and _rear sentinels, with _current pointing
  //at the _rear sentinel
  _front = new node(nullptr, new node(nullptr, nullptr));
  _rear = _current = _front -> next;
  _rear->prev = _front;
}

void list::_copy(const list & orig){
  /* creates a deep copy a list */

  //initializes the empty list
  _init();

  // exits the funtion if original is empty because it's already copied
  if (orig.size() == 0) return;

  //creates a node pointer that will point at the original list
  node * p = orig._front -> next;

  // iterates through the original list and copies all the data into new
  // nodes in the current list
  for(_current_index = 0; _current_index < orig.size(); _current_index++){
    add(p->data, _current_index);
    p = p -> next;
  }
}

void list::_destroy(){
  /* deletes all the nodes in a list including the sentinels */

  // iterates until _front has nothing left to point at
  while(_front){

    // creates a pointer to point at the node after what
    // front is pointing at
    node * death_row = _front -> next;

    // deletes what _front is pointing at and sets front pointing to
    // next node
    delete _front;
    _front = death_row;
  }
  _size = 0;
}

void list::add(const element & item, int index){
  /* adds an item to the list. creates a node containing the items
  and inserts it before the specified index */

  // converts the index to positive if it's negative and points the
  // _current pointer at the node at _current_index
  index = _compute_index(index);
  _set_current_index(index);

  // creates a new node with the item, and inserts it before the node
  // that _current is pointing at
  node * add = new node(_current -> prev, item, _current);

  // has the node _current is pointing at point back at the insterted node,
  // has the node before the inserted node point at inserted node, and
  // sets _current to point at the inserted node
  _current -> prev = add;
  add -> prev -> next = add;
  _current = add;

  // increases the size because an item has been added to the list
  _size++;
}

void list::remove_at(int index){
  /* removes the node from the list at a given index, and connects
  the two nodes that were next and previous of the removed node */

  // converts the index to positive if it's negative and points the
  // _current pointer at the node at _current_index
  index = _compute_index(index);
  _set_current_index(index);

  // creates a pointer to point at the node that will be deleted
  node * death = _current;

  // points _current to the node after the node that's being deleted
  _current = _current -> next;

  // the _current node will point back at the node before the node being
  // deleted, and points that node's next field at current
  _current -> prev = _current -> prev -> prev;
  _current -> prev -> next = _current;

  // deletes the node at the index and decreases the size of the list
  delete death;
  _size--;
}

void list::remove(const element & item){
  /* removes the first node that contains the passed item */

  // finds the first index of the given item. remover will be size() if
  // the item isn't in the list. Then calls remove_at the index if
  // the item was found
  size_t remover = find(item);
  if (remover != size()) remove_at(remover);
}

size_t list::find(const element & item) const{
  /* returns the index of where the passed item is located if
  it's in the list. If it's not in the list, returns size */

  // iterates through the indexes of the list. If the item is found at
  // a given index using the get method, returns that index
  for (size_t i = 0; i < size(); i++){
    if (get(i) == item)
      return i;
  }
  // returns size if the item is not found
  return size();
}

list::element list::get(int index) const{
  /* returns the item in the list at the passed index */

  // computes the non-negative index
  size_t new_index = _compute_index(index);
  // if the index is 0 returns the data in the first non-sentinel node
  if (new_index == 0)
    return _front -> next -> data;

  // if the index is one less than size returns the data in the
  // last non-sentinel node
  if ((new_index == size() - 1))
    return _rear -> prev -> data;

  // checks if the index is in the vicinity of _current_index if
  // _current index isn't poitning at the rear sentinel
  if (_current != _rear){

    // returns the data at _current_index if index = _current_index
    if (new_index == _current_index)
      return _current -> data;

    // returns the data in the node before _current_index if
    // index = _current_index - 1
    else if ((new_index == _current_index - 1))
      return _current -> prev -> data;

    // returns the data in the node after _current_index if
    // index = _current_index + 1
    else if ((new_index == _current_index + 1))
      return _current -> next -> data;
  }

  // if the index isn't in the vicinity of the sentinels or the
  // _current_index then search for it in the list and returns the data
  _set_current_index(new_index);
  return _current -> data;
}

size_t list::size() const{
  /* returns the size of the list */
  return _size;
}

void list::output(std::ostream & ostr) const{
  /* Prints the list in a linear format */

  //prints the openning bracket
  ostr << '<';

  // iterates throught the list, and prints every item in the list,
  // followed by a comma and space unless it's the last item in the list
  for (node * p = _front->next; p != _rear; p = p->next) {
    ostr << p->data;
    if (p->next != _rear)
      ostr << ", ";
  }
  // prints the closing bracket
  ostr << '>';
}

void list::_set_current_index(size_t index) const{
  /* points _current at the node at the passed index in the most
  effiecient manner possible */

  // checks if the index is after _current
  if (index > _current_index){

    // checks if the index is closer to _current or in the middle of
    // _current and _rear. If so, starts moving _current forward from
    // _current until it's at the proper index
    if ((size() - index >= index - _current_index)){
      for (size_t i = 0; i < index - _current_index; i++)
        _current = _current -> next;
      _current_index = index;
      return;
    }

    // checks if the index is closer to the end of the list. If so then
    // start moving _current backwards from _rear until it's at
    // the proper index
    else{
      _current = _rear;
      for (size_t i = 0; i < size() - index; i++)
        _current = _current -> prev;
      _current_index = index;
      return;
    }
  }

  // checks if the index is before _current
  else if (index < _current_index){

    // checks if the index is closer to _front or in the middle of
    // _current and _front. If so, starts moving _current forward from
    // _front -> next until it's at the proper index
    if (_current_index - index >= index){
      _current = _front -> next;
      for(size_t i = 0; i < index; i++)
        _current = _current -> next;
      _current_index = index;
      return;
    }

    // checks if the index is closer to the _current. If so then
    // start moving _current backwards from _current until it's at
    // the proper index
    else{
      for(size_t i = 0; i < _current_index - index; i++)
        _current = _current -> prev;
      _current_index = index;
      return;
    }
  }

  // if index applies to none of the other if statements start from
  // the beginning of the list and move _current forward until it's
  // at the proper index
  else{
    _current = _front -> next;
    for (size_t i = 0; i < index; i++)
      _current = _current -> next;
    _current_index = index;
    return;
  }
}

size_t list::_compute_index(int index) const{
  /* returns the index unchanged if it's positive, and
  returns index + size() if it's negative */
  return index >= 0 ? index: size() + index;
}
