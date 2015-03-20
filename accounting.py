#!/usr/bin/env python
# -*- encoding: utf-8 -*-
import redis
import sys
import os
import errno
import json
import codecs
from datetime import datetime
r = redis.StrictRedis(host='localhost', port=6379, db=0)

if len(sys.argv) < 3:
	sys.stderr.write("Usage: %s path-to-accounts account ...\n" % (sys.argv[0],))
	sys.exit(1)

path = sys.argv[1]

try:
	os.makedirs(path, 0700)
except OSError, err:
	if err.errno != errno.EEXIST:
		sys.stderr.write("%s: %s" % (sys.argv[0], os.strerror(err.errno)))
		sys.exit(errno.EEXIST)

accounts = map(lambda x: "account:" + x, sys.argv[2:])

while True:
	key, line = r.brpop(accounts, timeout=0)
	record = json.loads(line)

	if record:
		ts = datetime.utcfromtimestamp(record["timestamp"])

		f = codecs.open(path + "/" + key[len("account:"):] + ".csv", mode="a", encoding="utf-8")
		f.write(u"\"%s\",\"%s\",%.2f\n" % (ts.isoformat(), record["line"], record["amount"] / 100))
		f.close()
