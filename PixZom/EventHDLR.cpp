#include "EventHDLR.h"

/*
------------------------------------------------------------------------------------------------
	CListener
------------------------------------------------------------------------------------------------
*/


CListener::CListener()
{
}

CListener::~CListener()
{
}


/*
------------------------------------------------------------------------------------------------
	CEventHDLR
------------------------------------------------------------------------------------------------
*/

CEventHDLR* CEventHDLR::__instance(nullptr);

CEventHDLR::CEventHDLR()
{
	m_activeQueue = 0;
}

CEventHDLR::~CEventHDLR()
{
	// Clear queues, otherwise gonna leak memory
	std::list<CEventMessage*>::iterator it;
	for (int i = 0; i < kSize; i++) {
		for (it = m_queue[i].begin(); it != m_queue[i].end(); it++)
			delete (*it);

		m_queue[i].clear();
	}
}

//---------------------------------------------------------------------------
//
//	Maps specified listener to specified event type.
//	So when the event is processed, its passed to the listener 
//
//---------------------------------------------------------------------------
bool CEventHDLR::AddListener(CListener* listener, const CEventType & evtType)
{
	// Check if we have event registered, if not return false and don't add
	if (!ValidateType(evtType))
		return false;

	// Attempt to find event in table
	EventListenerMap::iterator evtListMapIt = m_listenerEventMap.find(evtType.GetIdent());

	// If cannot find an entry then create one
	if (evtListMapIt == m_listenerEventMap.end()) {
		EventListenerIRes evtListMapIRes = m_listenerEventMap.insert(
			EventListenerEntry(evtType.GetIdent(), EventListenerTable()));

		// Cannot insert into map?
		if (evtListMapIRes.second == false)
			return false;

		// Insert and created an empty table?
		if (evtListMapIRes.first == m_listenerEventMap.end())
			return false;

		// Store so we can update the list
		evtListMapIt = evtListMapIRes.first;
	}

	EventListenerTable& evtListTable = (*evtListMapIt).second;
	// Check that event/listener is not already present in the map
	for (EventListenerTable::iterator it = evtListTable.begin(); it != evtListTable.end(); it++) {
		if (*it == listener)
			return false;
	}

	// Add new listener
	evtListTable.push_back(listener);

	return true;
}

//---------------------------------------------------------------------------
//
//	Queues event for later processing
//
//---------------------------------------------------------------------------
bool CEventHDLR::QueueMessage(CEventMessage * evt)
{
	// Valid event type?
	if (!ValidateType(evt->VGetEventType()))
		return false;

	// Should ignore?
	if (IgnoreMessage(evt))
	{
		delete evt;
		return false;
	}
	
	// Find if event in table
	EventListenerMap::const_iterator it =
		m_listenerEventMap.find(evt->VGetEventType().GetIdent());

	// If not in table
	if (it == m_listenerEventMap.end())
		return false;

	// Add event to the queue
	m_queue[m_activeQueue].push_back(evt);

	return true;
}

//---------------------------------------------------------------------------
//
//	Aborts first instance of event type found in queue.
//  Aborts all if allType is true
//
//---------------------------------------------------------------------------
bool CEventHDLR::AbortEvent(CEventType & evt, bool allType)
{
	// Event validity check
	if (!ValidateType(evt))
		return false;

	// Check for listeners for matching event
	EventListenerMap::iterator it = m_listenerEventMap.find(evt.GetIdent());
	if (it == m_listenerEventMap.end())
		return false;

	bool aborted = false;

	std::list<CEventMessage*>& queue = m_queue[m_activeQueue];

	// Go through the active queue
	for (std::list<CEventMessage*>::iterator it = queue.begin(); it != queue.end();) {
		// If found match
		if ((*it)->VGetEventType() == evt) {
			// Remove the event
			delete (*it);
			it = queue.erase(it);
			aborted = true;
			// If all type is not specified, break
			if (!allType)
				break;
		}
		else
			++it;
	}

	return aborted;
}

//---------------------------------------------------------------------------
//
//	Processes all events in the queue.
//  If time taken to process exceeds maxTime, remaining events are
//  transfered to other queue and processed in next frame.
//
//---------------------------------------------------------------------------
bool CEventHDLR::ProcessEvents(Uint32 maxTime)
{
	Uint32 currentTime = SDL_GetTicks();
	Uint32 timeLimit = currentTime + maxTime;

	int queueProcess = m_activeQueue;

	// Swap and clear the active queue
	m_activeQueue = (m_activeQueue + 1) % kSize;
	m_queue[m_activeQueue].clear();

	while (m_queue[queueProcess].size() > 0)
	{
		// Get first message from queue and remove it
		CEventMessage* evt = m_queue[queueProcess].front();
		m_queue[queueProcess].pop_front();

		// Find listeners for this event
		const CEventType& eventType = evt->VGetEventType();
		EventListenerMap::const_iterator listenerIt =
			m_listenerEventMap.find(eventType.GetIdent());

		// Continue if no listeners for this event
		if (listenerIt == m_listenerEventMap.end())
			continue;

		const unsigned int eventId = listenerIt->first;
		const EventListenerTable& table = listenerIt->second;

		for (EventListenerTable::const_iterator it = table.begin(), end = table.end();
			it != end; it++)
		{
			(*it)->HandleMessage(*evt);
		}

		delete evt;

		// Ran outta time, stop processing messages
		currentTime = SDL_GetTicks();
		if (currentTime >= timeLimit)
			break;
	}

	// If we have not managed to process all the messages in the queue,
	// stick'em in the other one.
	bool queueFlushed = m_queue[queueProcess].size() == 0;
	if (!queueFlushed)
	{
		while (m_queue[queueProcess].size() > 0)
		{
			CEventMessage* eventMsg = m_queue[queueProcess].back();
			m_queue[queueProcess].pop_back();
			m_queue[m_activeQueue].push_front(eventMsg);
		}
	}

	// Return whether we managed to process all the messages in the queue or not.
	return queueFlushed;
}

//---------------------------------------------------------------------------
//
//	This actions the event immediately rather than sticking it in the queue
//
//---------------------------------------------------------------------------
void CEventHDLR::ActionEvent(CEventMessage * evt)
{
	// Valid event type?
	if (ValidateType(evt->VGetEventType()))
	{
		// Find listeners for this event
		const CEventType& eventType = evt->VGetEventType();
		EventListenerMap::const_iterator listenerIt =
			m_listenerEventMap.find(eventType.GetIdent());

		// If there are listeners for this message
		if (listenerIt != m_listenerEventMap.end())
		{
			const unsigned int eventId = listenerIt->first;
			const EventListenerTable& table = listenerIt->second;

			for (EventListenerTable::const_iterator it = table.begin(), end = table.end();
				it != end; it++)
			{
				(*it)->HandleMessage(*evt);
			}
		}
	}
	delete evt;
}

//---------------------------------------------------------------------------
//
//	Registers event type.
//  Event won't get queued/actioned if type is not registered
//
//---------------------------------------------------------------------------
bool CEventHDLR::RegisterEvent(const CEventType & eventType)
{
	// Check if event already registered
	for (auto it = m_eventRegistry.begin(); it != m_eventRegistry.end(); it++)
	{
		// Don't add
		if ((*it).GetString() == eventType.GetString())
			return false;
	}
	// Add
	m_eventRegistry.push_back(eventType);
	return true;
}

//---------------------------------------------------------------------------
//
//	Checks if event type is known type/registered
//
//---------------------------------------------------------------------------
bool CEventHDLR::ValidateType(const CEventType & type)
{
	// Valid input?
	if (type.GetString().length() == 0)
		return false;

	// Find event type
	for (auto it = m_eventRegistry.begin(); it != m_eventRegistry.end(); it++)
	{
		if ((*it).GetString() == type.GetString())
			return true;
	}

	return false;
}

CEventHDLR * CEventHDLR::GetInstance()
{
	if (__instance == nullptr)
		__instance = new CEventHDLR;

	return __instance;
}

void CEventHDLR::Release()
{
	if (__instance)
	{
		delete __instance;
		__instance = nullptr;
	}
}

//---------------------------------------------------------------------------
//
//	Ignores certain messages
//  Not used yet.
//
//---------------------------------------------------------------------------
bool CEventHDLR::IgnoreMessage(CEventMessage * evt)
{
	const CEventType& evtType = evt->VGetEventType();

	std::list<CEventMessage*>& queue = m_queue[m_activeQueue];

	if (queue.size() <= 0)
		return false;

	// To Do: ignore messages depending on scenario

	return false;
}

