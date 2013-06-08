#ifndef kassomat_SSPComsTask_h
#define kassomat_SSPComsTask_h

#include <QObject>
#include <QByteArray>
#include <functional>

class SSPComsTask : public QObject {
	Q_OBJECT
	QByteArray message;
	std::function<void(const QByteArray&)> responder;
public:
	SSPComsTask(const QByteArray &m, const std::function<void(QByteArray)> &r) : message(m), responder(r) {}
public slots:
	void responseAvailable(QByteArray data);
};

#endif
