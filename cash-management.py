import redis
r = redis.StrictRedis(host='localhost', port=6379, db=0)
p = r.pubsub(ignore_subscribe_messages=True)

p.subscribe('action', 'buy')
p.get_message() # hides the subscribe message

for message in p.listen():
	if message["channel"] == "action":
		pass
	elif message["channel"] == "buy":
		pass
