#!/usr/bin/env python
# -*- encoding: utf-8 -*-
import json
import redis
import sys
import codecs
from datetime import datetime
r = redis.StrictRedis(host='localhost', port=6379, db=0)

logfile = "/tmp/kassomat.log"

if len(sys.argv) > 1:
	if sys.argv[1] == "-h" or sys.argv[1] == "--help":
		sys.stderr.write("Usage: %s [logfile]\n" % (sys.argv[0],))
		sys.exit(0)
	logfile = sys.argv[1]

while True:
	key, line = r.brpop('log')
	record = json.loads(line)

	if record:
		try:
			ts = datetime.utcfromtimestamp(record["timestamp"])

			f = codecs.open(logfile, mode="a", encoding="utf-8")
			f.write(u"[%s]\t%s\t%s\n" % (record["source"], ts.isoformat(), record["message"]))
			f.close()
		except:
			pass
