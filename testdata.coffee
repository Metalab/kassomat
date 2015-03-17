redis = require "redis"
db = redis.createClient
	detect_buffers: true

db.hmset 'userinfo',
	username: ""
	credits: 0

db.hmset 'entity:DenominationLevel:20', { count: 100 }, (err) ->
	if err
		console.error err
db.hmset 'entity:DenominationLevel:50', { count: 65 }, (err) ->
	if err
		console.error err
db.hmset 'entity:DenominationLevel:100', { count: 0 }, (err) ->
	if err
		console.error err
db.hmset 'entity:DenominationLevel:200', { count: 12 }, (err) ->
	if err
		console.error err
db.hmset 'entity:DenominationLevel:1000', { count: 2 }, (err) ->
	if err
		console.error err
