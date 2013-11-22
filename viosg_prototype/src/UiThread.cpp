/*
 * UiThread.cpp
 *
 *  Created on: 22 nov. 2013
 *      Author: blam
 */

#include "UiThread.h"

using namespace std;
int UiThread::cancel()
{
  threadDone = true;
  //FIXME : Uithread stays open beacause of blocking call getLine(cin,command)
  while( isRunning() ) YieldCurrentThread();

  return 0;
}

void UiThread::run()
{
  threadDone = false;
  dirty = true;
  command="";
  do
  {
    YieldCurrentThread();
    string userInput;
    getline(cin,userInput);
    setNewCommand(userInput);
  } while( !threadDone );
}

void UiThread::setNewCommand(string input )
{
  OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
  command = input;
  dirty = true;
}

bool UiThread::copyNewCommandTo( string& output )
{
  OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
  if ( dirty )
  {
    output = command;
    dirty = false;
    return true;
  }
  return false;
}
