#ifndef FIBOHEAP_HPP
#define FIBOHEAP_HPP

namespace honeta {
  template<typename T>
  class FiboHeap {
  public:
    FiboHeap() : min_(nullptr), size_(0) {}
    ~FiboHeap() {Destroy(min_);}
    bool empty() const {return size_ == 0;}
    size_t size() const {return size_;}
    void push(const T &val) {
      ++size_;
      Insert(new Node(val));    
    }
    void pop() {
      if (!size_) return;
      --size_;
      DeleteMin();
    }
    const T top() const {return min_->val_;}
    void clear() {
      size_ = 0;
      Destroy(min_);
    }
  private:
    struct Node {
      Node() : val_(), deg_(0), child_(nullptr), left_(nullptr), right_(nullptr) {}
      explicit Node(T val) : val_(val), deg_(0), child_(nullptr), left_(nullptr), right_(nullptr) {}
      T val_;
      size_t deg_;
      Node *child_, *left_, *right_;
    };
    Node *min_;
    size_t size_;
    void Consolidate() {
      if (size_ <= 1) return;
      Node *cur = min_, **last = new Node *[size_]();
      do {
        while (last[cur->deg_] && (last[cur->deg_] != cur)) {        
          Node *temp = last[cur->deg_];        
          last[cur->deg_] = nullptr;
          if (temp->val_ < cur->val_) {
            cur->left_->right_ = cur->right_;
            cur->right_->left_ = cur->left_;
            temp->deg_++;
            if (temp->child_) {
              cur->right_ = temp->child_->right_;
              cur->left_ = temp->child_;
              temp->child_->right_->left_ = cur;
              temp->child_->right_ = cur;            
            } else {
              temp->child_ = cur;
              cur->left_ = cur;
              cur->right_ = cur;
            }
            cur = temp;
          } else {
            if ((temp->val_ == cur->val_) && (min_ == temp))
              min_ = cur;
            temp->left_->right_ = temp->right_;
            temp->right_->left_ = temp->left_;
            cur->deg_++;
            if(cur->child_) {
              temp->right_ = cur->child_->right_;
              temp->left_ = cur->child_;
              cur->child_->right_->left_ = temp;
              cur->child_->right_ = temp;
            } else {
              cur->child_ = temp;
              temp->left_ = temp;
              temp->right_ = temp;
            }          
          }
        }
        last[cur->deg_] = cur;
        cur = cur->right_;  
      } while (cur != min_);
      delete[] last;
    }  
    void Insert(Node *temp) {
      if (!min_) {
        min_ = temp;
        min_->left_ = temp;
        min_->right_ = temp;
        return;
      }
      min_->right_->left_ = temp;
      temp->right_ = min_->right_;
      temp->left_ = min_;
      min_->right_ = temp;
      if (temp->val_ < min_->val_)
        min_ = temp;
    }
    void DeleteMin() {
      if (!min_) return;
      if (!size_) {
        delete min_;
        min_ = nullptr;
        return;
      }
      if (min_->child_) {
        Node *pointer = min_->child_;
        do {
          Node *right = pointer->right_;
          Insert(pointer);
          pointer = right;
        } while (pointer != min_->child_);
        min_->child_ = nullptr;
      }    
      Node *temp = min_;
      min_ = temp->right_;
      T val = temp->right_->val_;
      for (Node *cur = temp->right_; cur != temp; cur = cur->right_) {
        if (cur->val_ < val) {
          val = cur->val_;
          min_ = cur;
        }
      }
      temp->left_->right_ = temp->right_;
      temp->right_->left_ = temp->left_;    
      delete temp;
      Consolidate();
    }
    void Destroy(Node *cur) {
      if (!cur) return;
      Node *pointer = cur, *right;
      do {
        if(pointer->child_)
          Destroy(pointer->child_);
        right = pointer->right_;
        delete pointer;
        pointer = right;
      } while (pointer != cur);
    }
    void swap(FiboHeap<T> &other) noexcept {
      Node *min = min_;
      size_t size = size_;
      min_ = other.min_;
      other.min_ = min;
      size_ = other.size_;
      other.size_ = size;
    }
  };
}

#endif