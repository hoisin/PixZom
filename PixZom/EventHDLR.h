//---------------------------------------------------------------------------
//
//	Event handler and listener base class.
//	Event handler accepts sent messages and sends them out to registered
//	 listeners.
//
//---------------------------------------------------------------------------
#pragma once

#include <vector>
#include <memory>
#include <string>
#include <list>
#include <map>
#include <SDL.h>
#include "Events.h"

class CListener
{
public:
	CListener();
	virtual ~CListener();

	virtual void HandleMessage(const CEventMessage& msg) = 0;

protected:
};


class CEventHDLR
{
private:
	typedef std::list<CEventType> EventTypeList;							// List of event types
	typedef std::list<CListener*> EventListenerTable;						// List of listeners, one per registered event
	typedef std::map<unsigned int, EventListenerTable> EventListenerMap;	// Mapping of an event ot list of listeners, the 'unsigned int' is the hash value in CEventType (m_ident)
	typedef std::pair<unsigned int, EventListenerTable> EventListenerEntry;	// Shorthand for declaring new entries into the EventListenerMap
	typedef std::pair<EventListenerMap::iterator, bool> EventListenerIRes;	// Used to determine if insertion was successful

public:
	CEventHDLR();
	~CEventHDLR();

	bool AddListener(CListener* listener, const CEventType& evtType);
	bool QueueMessage(CEventMessage* evt);
	bool AbortEvent(CEventType& evt, bool allType = false);
	bool ProcessEvents(Uint32 maxTime);
	void ActionEvent(CEventMessage* evt);

	bool RegisterEvent(const CEventType& eventType);
	bool ValidateType(const CEventType& type);

	static CEventHDLR* GetInstance();
	void Release();

private:
	bool IgnoreMessage(CEventMessage* evt);

protected:
	enum ENumQueues
	{
		kSize = 2
	};

	EventTypeList m_eventRegistry;			// List of registered events
	EventListenerMap m_listenerEventMap;	// Table for mapping events to listeners
	std::list<CEventMessage*> m_queue[kSize];
	int m_activeQueue;

	static CEventHDLR* __instance;
};

#define EVENTSYS CEventHDLR::GetInstance()