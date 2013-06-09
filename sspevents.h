#ifndef kassomat_sspevents_h
#define kassomat_sspevents_h

class QString;

class SSPEvent {
public:
	virtual uint8_t eventCode() = 0;
	virtual const QString &eventDescription() = 0;
};

#endif
