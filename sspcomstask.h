#ifndef kassomat_SSPComsTask_h
#define kassomat_SSPComsTask_h

#include <QObject>
#include <QByteArray>
#include <functional>

class SSPComsTask : public QObject {
	Q_OBJECT
	QByteArray m_message;
	std::function<void(uint8_t status, const QByteArray&)> responder;
public:
	SSPComsTask(const QByteArray &m, const std::function<void(uint8_t, QByteArray)> &r) : m_message(m), responder(r) {}
	
	const QByteArray &message() const { return m_message; }
public slots:
	void responseAvailable(QByteArray data);
};

#endif
