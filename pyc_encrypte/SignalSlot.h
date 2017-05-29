#pragma once
#include<set>
#include<list>

class MySlot;

template<typename Tparam>
class Connection_Base
{
public:
	virtual MySlot* getdest() const = 0;
	virtual void emitSignal(Tparam) = 0;
	virtual ~Connection_Base() {}
};


template<typename Tobject, typename Tparam>
class Connection : public Connection_Base<Tparam>
{
	typedef void (Tobject::*Tfunc)(Tparam);
public:
	Connection(Tobject* pobject, Tfunc p) {
		m_pobject = pobject;
		m_pf = p;
	}
	void emitSignal(Tparam parm) {
		(m_pobject->*m_pf)(parm);
	}
	MySlot* getdest() const {
		return m_pobject;
	}
private:
	Tobject* m_pobject;
	Tfunc m_pf;
};





class _Signal_Base
{
public:
	virtual void slot_disconnect(MySlot* pslot) = 0;
};


class MySlot
{
private:
	typedef std::set<_Signal_Base*> sender_set;
	typedef sender_set::const_iterator const_iterator;

public:
	void signal_connect(_Signal_Base* sender) {
		m_senders.insert(sender);
	}
	void signal_disconnect(_Signal_Base* sender) {
		m_senders.erase(sender);
	}

	virtual ~MySlot() {
		disconnect_all();
	}

	void disconnect_all()
	{
		const_iterator it = m_senders.begin();
		const_iterator itEnd = m_senders.end();
		while (it != itEnd)
		{
			(*it)->slot_disconnect(this);
			++it;
		}
		m_senders.erase(m_senders.begin(), m_senders.end());
	}

private:
	sender_set m_senders;
};


template<typename Tparam>
class Signal_Base : public _Signal_Base
{
public:
	typedef std::list<Connection_Base<Tparam>*>  connections_list;
	~Signal_Base() {
		disconnect_all();
	}

	void disconnect_all()
	{
		typename connections_list::const_iterator it = m_connected_slots.begin();
		typename connections_list::const_iterator itEnd = m_connected_slots.end();

		while (it != itEnd)
		{
			(*it)->getdest()->signal_disconnect(this);
			delete *it;
			++it;
		}
		m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
	}

	void disconnect(MySlot* pclass)
	{
		typename connections_list::iterator it = m_connected_slots.begin();
		typename connections_list::iterator itEnd = m_connected_slots.end();

		while (it != itEnd)
		{
			if ((*it)->getdest() == pclass)
			{
				delete *it;
				m_connected_slots.erase(it);
				pclass->signal_disconnect(this);
				return;
			}
			++it;
		}
	}

	void slot_disconnect(MySlot* pslot)
	{
		typename connections_list::iterator it = m_connected_slots.begin();
		typename connections_list::iterator itEnd = m_connected_slots.end();

		while (it != itEnd)
		{
			typename connections_list::iterator itNext = it;
			++itNext;

			if ((*it)->getdest() == pslot)
			{
				m_connected_slots.erase(it);
				//          delete *it;  
			}
			it = itNext;
		}
	}
protected:
	connections_list m_connected_slots;
};


template<typename Tparam>
class MySignal :public Signal_Base<Tparam>
{
public:
	template<typename Tobject>
	void connect(Tobject* pclass, void (Tobject::*pmemfun)(Tparam))
	{
		Connection<Tobject, Tparam>* conn = new Connection<Tobject, Tparam>(pclass, pmemfun);
		Signal_Base<Tparam>::m_connected_slots.push_back(conn);
		pclass->signal_connect(this);
	}

	void operator()(Tparam p)
	{
		typename std::list<Connection_Base<Tparam>*>::const_iterator itNext, it, itBegin = Signal_Base<Tparam>::m_connected_slots.begin();
		typename std::list<Connection_Base<Tparam>*>::const_iterator itEnd = Signal_Base<Tparam>::m_connected_slots.end();
		it = itBegin;
		while (it != itEnd)
		{
			itNext = it;
			++itNext;
			(*it)->emitSignal(p);
			it = itNext;
		}
	}
};
