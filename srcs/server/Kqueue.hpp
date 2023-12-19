#pragma once

#include <iostream>
#include <string>

#include <sys/event.h>

#define MAX_EVENTS 10

class Kqueue {
	private:
		int _kq;
		struct kevent _change_event;
		struct kevent _events[MAX_EVENTS];
	public:
		Kqueue(void);
		~Kqueue(void);
		void init(void);
		void add(int fd);
		void del(int fd);
		int wait(void);
		struct kevent *getEvents(void);
};