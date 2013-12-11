/*
 * UiThread.h
 *
 *  Created on: 22 nov. 2013
 *      Author: blam
 */

#ifndef UITHREAD_H_
#define UITHREAD_H_

#include <iostream>
#include <OpenThreads/Thread>
#include <OpenThreads/ScopedLock>

class UiThread : public OpenThreads::Thread
{
public:
  static UiThread* instance()
  {
   static UiThread s_thread;
   return &s_thread;
  }
  virtual int cancel();
  virtual void run();
  void setNewCommand(std::string intput);
  bool copyNewCommandTo( std::string& output );
protected:
  OpenThreads::Mutex _mutex;
  std::string command;
  bool threadDone;
  bool dirty;
};

#endif /* UITHREAD_H_ */


