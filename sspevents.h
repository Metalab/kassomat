#ifndef kassomat_sspevents_h
#define kassomat_sspevents_h

#include <qobject.h>
#include <qstring.h>

class SSPEvent : public QObject {
public:
	virtual uint8_t eventCode() = 0;
    virtual const QString eventDescription() = 0;
    virtual uint8_t length() { return 1; }
    virtual bool pollWithAckEvent() { return false; }
};

class SSPResetEvent : public SSPEvent {
public:
    SSPResetEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xf1;
    }
    const QString eventDescription() {
        return "Slave Reset";
    }
};

class SSPReadNoteEvent : public SSPEvent {
public:
    SSPReadNoteEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xef;
    }
    const QString eventDescription() {
        return "Read Note";
    }
};

class SSPCreditNoteEvent : public SSPEvent {
public:
    SSPCreditNoteEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xee;
    }
    const QString eventDescription() {
        return "Credit Note";
    }
};

class SSPNoteRejectingEvent : public SSPEvent {
public:
    SSPNoteRejectingEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xed;
    }
    const QString eventDescription() {
        return "Note Rejecting";
    }
};

class SSPNoteRejectedEvent : public SSPEvent {
public:
    SSPNoteRejectedEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xec;
    }
    const QString eventDescription() {
        return "Note Rejected";
    }
};

class SSPNoteStackingEvent : public SSPEvent {
public:
    SSPNoteStackingEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xcc;
    }
    const QString eventDescription() {
        return "Note Stacking";
    }
};

class SSPNoteStackedEvent : public SSPEvent {
public:
    SSPNoteStackedEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xeb;
    }
    const QString eventDescription() {
        return "Note Stacked";
    }
};

class SSPSafeNoteJamEvent : public SSPEvent {
public:
    SSPSafeNoteJamEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xea;
    }
    const QString eventDescription() {
        return "Safe Note Jam";
    }
};

class SSPUnsafeNoteJamEvent : public SSPEvent {
public:
    SSPUnsafeNoteJamEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xe9;
    }
    const QString eventDescription() {
        return "Unsafe Note Jam";
    }
};

class SSPDisabledEvent : public SSPEvent {
public:
    SSPDisabledEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xe8;
    }
    const QString eventDescription() {
        return "Disabled";
    }
};

class SSPFraudAttemptEvent : public SSPEvent {
public:
    SSPFraudAttemptEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xe6;
    }
    const QString eventDescription() {
        return "Fraud Attempt";
    }
};

class SSPStackerFullEvent : public SSPEvent {
public:
    SSPStackerFullEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xe7;
    }
    const QString eventDescription() {
        return "Stacker Full";
    }
};

class SSPNoteClearedFromFrontEvent : public SSPEvent {
public:
    SSPNoteClearedFromFrontEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xe1;
    }
    const QString eventDescription() {
        return "Note Cleared From Front";
    }
};

class SSPNoteClearedToCashboxEvent : public SSPEvent {
public:
    SSPNoteClearedToCashboxEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xe2;
    }
    const QString eventDescription() {
        return "Note Cleared To Cashbox";
    }
};

class SSPCashboxRemovedEvent : public SSPEvent {
public:
    SSPCashboxRemovedEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xe3;
    }
    const QString eventDescription() {
        return "Cashbox Removed Event";
    }
};

class SSPCashboxReplacedEvent : public SSPEvent {
public:
    SSPCashboxReplacedEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xe4;
    }
    const QString eventDescription() {
        return "Cashbox replaced";
    }
};

class SSPBarCodeTicketValidatedEvent : public SSPEvent {
public:
    SSPBarCodeTicketValidatedEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xe5;
    }
    const QString eventDescription() {
        return "Car Code Ticket Validated";
    }
};

class SSPBarCodeTicketAcknowledgeEvent : public SSPEvent {
public:
    SSPBarCodeTicketAcknowledgeEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xd1;
    }
    const QString eventDescription() {
        return "Bar Code Ticket Acknowledge";
    }
};

class SSPNotePathOpenEvent : public SSPEvent {
public:
    SSPNotePathOpenEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xe0;
    }
    const QString eventDescription() {
        return "Note Path Open";
    }
};

class SSPChannelDisableEvent : public SSPEvent {
public:
    SSPChannelDisableEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xb5;
    }
    const QString eventDescription() {
        return "Channel Disable";
    }
};

class SSPInitiatingEvent : public SSPEvent {
public:
    SSPInitiatingEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xb6;
    }
    const QString eventDescription() {
        return "Initiating";
    }
};

class SSPDispensingEvent : public SSPEvent {
public:
    SSPDispensingEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xda;
    }
    const QString eventDescription() {
        return "Dispensing";
    }
};

class SSPDispensedEvent : public SSPEvent {
public:
    SSPDispensedEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xd2;
    }
    const QString eventDescription() {
        return "Dispensed";
    }
};

class SSPJammedEvent : public SSPEvent {
public:
    SSPJammedEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xd5;
    }
    const QString eventDescription() {
        return "Jammed";
    }
};

class SSPHaltedEvent : public SSPEvent {
public:
    SSPHaltedEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xd6;
    }
    const QString eventDescription() {
        return "Halted";
    }
};

class SSPFloatingEvent : public SSPEvent {
public:
    SSPFloatingEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xd7;
    }
    const QString eventDescription() {
        return "Floating";
    }
};

class SSPFloatedEvent : public SSPEvent {
public:
    SSPFloatedEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xd8;
    }
    const QString eventDescription() {
        return "Floated";
    }
};

class SSPTimeoutEvent : public SSPEvent {
public:
    SSPTimeoutEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xd9;
    }
    const QString eventDescription() {
        return "Timeout";
    }
};

class SSPIncompletePayoutEvent : public SSPEvent {
public:
    SSPIncompletePayoutEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xdc;
    }
    const QString eventDescription() {
        return "Incomplete Payout";
    }
};

class SSPIncompleteFloatEvent : public SSPEvent {
public:
    SSPIncompleteFloatEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xdd;
    }
    const QString eventDescription() {
        return "Incomplete Float";
    }
};

class SSPCashboxPaidEvent : public SSPEvent {
public:
    SSPCashboxPaidEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xde;
    }
    const QString eventDescription() {
        return "Cashbox Paid";
    }
};

class SSPCoinCreditEvent : public SSPEvent {
public:
    SSPCoinCreditEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xdf;
    }
    const QString eventDescription() {
        return "Coin Credit";
    }
};

class SSPCoinMechJammedEvent : public SSPEvent {
public:
    SSPCoinMechJammedEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xc4;
    }
    const QString eventDescription() {
        return "Coin Mech Jammed";
    }
};

class SSPCoinMechReturnPressedEvent : public SSPEvent {
public:
    SSPCoinMechReturnPressedEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xc5;
    }
    const QString eventDescription() {
        return "Coin Mech Return Pressed";
    }
};

class SSPEmptyingEvent : public SSPEvent {
public:
    SSPEmptyingEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xc2;
    }
    const QString eventDescription() {
        return "Emptying";
    }
};

class SSPEmptiedEvent : public SSPEvent {
public:
    SSPEmptiedEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xc3;
    }
    const QString eventDescription() {
        return "Emptied";
    }
};

class SSPSmartEmptyingEvent : public SSPEvent {
public:
    SSPSmartEmptyingEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xb3;
    }
    const QString eventDescription() {
        return "Smart Emptying";
    }
};

class SSPSmartEmptiedEvent : public SSPEvent {
public:
    SSPSmartEmptiedEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xb4;
    }
    const QString eventDescription() {
        return "Smart Emptied";
    }
};

class SSPCoinMechErrorEvent : public SSPEvent {
public:
    SSPCoinMechErrorEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xb7;
    }
    const QString eventDescription() {
        return "Coin Mech Error";
    }
};

class SSPNoteStoredInPayoutEvent : public SSPEvent {
public:
    SSPNoteStoredInPayoutEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xdb;
    }
    const QString eventDescription() {
        return "Note Stored in Payout";
    }
};

class SSPPayoutOutOfServiceEvent : public SSPEvent {
public:
    SSPPayoutOutOfServiceEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xc6;
    }
    const QString eventDescription() {
        return "Payout out of Service";
    }
};

class SSPJamRecoveryEvent : public SSPEvent {
public:
    SSPJamRecoveryEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xb0;
    }
    const QString eventDescription() {
        return "Jam Recovery";
    }
};

class SSPErrorDuringPayoutEvent : public SSPEvent {
public:
    SSPErrorDuringPayoutEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xb1;
    }
    const QString eventDescription() {
        return "Error During Payout";
    }
};

class SSPNoteTransferredToStackerEvent : public SSPEvent {
public:
    SSPNoteTransferredToStackerEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xc9;
    }
    const QString eventDescription() {
        return "Note Transferred to Stacker";
    }
};

class SSPNoteHeldInBezelEvent : public SSPEvent {
public:
    SSPNoteHeldInBezelEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xce;
    }
    const QString eventDescription() {
        return "Note Held in Bezel";
    }
};

class SSPNotePaidIntoStoreOnPowerupEvent : public SSPEvent {
public:
    SSPNotePaidIntoStoreOnPowerupEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xcb;
    }
    const QString eventDescription() {
        return "Note Paid into Store on Powerup";
    }
};

class SSPNotePaidIntoStackerOnPowerupEvent : public SSPEvent {
public:
    SSPNotePaidIntoStackerOnPowerupEvent(const QByteArray &data) {}
    uint8_t eventCode() {
        return 0xca;
    }
    const QString eventDescription() {
        return "Note Paid into Stacker on Powerup";
    }
};

#endif
