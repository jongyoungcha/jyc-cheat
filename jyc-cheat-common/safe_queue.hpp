#ifndef _SAFE_QUEUE_HPP_
#define _SAFE_QUEUE_HPP_

#include <queue>
#include <list>
#include <pthread.h>

using namespace std;

template <typename T>
class safe_queue{
  
private:
  list<T> inner_list;
  pthread_mutex_t mutex;

public:
  safe_queue(){
    pthread_mutex_init(&this->mutex, NULL);
  }

  ~safe_queue(){
    pthread_mutex_destroy(&mutex);
  }

  void push(T t){
    pthread_mutex_lock(&mutex);
    this->inner_list.push_back(t);
    pthread_mutex_unlock(&mutex);
  }


  void pop(){
    pthread_mutex_lock(&mutex);
    this->inner_list.pop_front();
    pthread_mutex_unlock(&mutex);
  }
  

  T front(){
    return this->inner_list.front();
  }


  size_t size(){
    pthread_mutex_lock(&mutex);
    size_t ret = this->inner_list.size();
    pthread_mutex_unlock(&mutex);

    return ret;
  }

  
  bool empty(){
    return this->inner_list.empty();
  }
};


#endif

