// Copyright (c) 2006- Facebook
// Distributed under the Thrift Software License
//
// See accompanying file LICENSE or visit the Thrift site at:
// http://developers.facebook.com/thrift/

#ifndef _THRIFT_CONCURRENCY_POSIXTHREADFACTORY_H_
#define _THRIFT_CONCURRENCY_POSIXTHREADFACTORY_H_ 1

#include "Thread.h"

#include <boost/shared_ptr.hpp>

namespace facebook { namespace thrift { namespace concurrency { 

/**
 * A thread factory to create posix threads 
 *
 * @author marc
 * @version $Id:$
 */
class PosixThreadFactory : public ThreadFactory {

 public:

  /**
   * POSIX Thread scheduler policies
   */
  enum POLICY {
    OTHER,
    FIFO,
    ROUND_ROBIN
  };

  /**
   * POSIX Thread scheduler relative priorities,
   *
   * Absolute priority is determined by scheduler policy and OS. This
   * enumeration specifies relative priorities such that one can specify a
   * priority withing a giving scheduler policy without knowing the absolute
   * value of the priority.
   */
  enum PRIORITY {
    LOWEST = 0,
    LOWER = 1,
    LOW = 2,
    NORMAL = 3,
    HIGH = 4,
    HIGHER = 5,
    HIGHEST = 6,
    INCREMENT = 7,
    DECREMENT = 8
  };

  /** 
   * Posix thread (pthread) factory.  All threads created by a factory are reference-counted 
   * via boost::shared_ptr and boost::weak_ptr.  The factory guarantees that threads and 
   * the Runnable tasks they host will be properly cleaned up once the last strong reference
   * to both is given up.
   *
   * Threads are created with the specified policy, priority, stack-size and detachable-mode
   * detached means the thread is free-running and will release all system resources the 
   * when it completes.  A detachable thread is not joinable.  The join method 
   * of a detachable thread will return immediately with no error.  
   *
   * Joinable threads will detach themselves iff they were not explicitly joined and
   * there are no remaining strong references to the thread.  This guarantees that
   * joinnable threads don't leak resources even when the application neglects to 
   * call join explicitly.
   *
   * By default threads are joinable.
   */

  PosixThreadFactory(POLICY policy=ROUND_ROBIN, PRIORITY priority=NORMAL, int stackSize=1, bool detached=false);

  // From ThreadFactory;
  boost::shared_ptr<Thread> newThread(boost::shared_ptr<Runnable> runnable) const;

  // From ThreadFactory;
  Thread::id_t currentThreadId() const;

  /**
   * Sets stack size for created threads
   *
   * @param value size in megabytes
   */
  virtual void stackSize(int value);

  /**
   * Gets stack size for created threads
   *
   * @return int size in megabytes
   */
  virtual int stackSize() const;

  /**
   * Sets priority relative to current policy
   */
  virtual void priority(PRIORITY priority);

  /**
   * Gets priority relative to current policy
   */
  virtual PRIORITY priority() const;
  
 private:
  class Impl;
  boost::shared_ptr<Impl> impl_;
};

}}} // facebook::thrift::concurrency

#endif // #ifndef _THRIFT_CONCURRENCY_POSIXTHREADFACTORY_H_
