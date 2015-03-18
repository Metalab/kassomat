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

fs = require "fs"
fs.readFile "testimages/internet.jpg", (err, data) ->
	if err
		console.error err
	else
		db.hmset 'projectimages:internet', { type: 'jpg', data: data }, (err) ->
			if err
				console.error err
fs.readFile "testimages/kassomat.png", (err, data) ->
	if err
		console.error err
	else
		db.hmset 'projectimages:kassomat', { type: 'png', data: data }, (err) ->
			if err
				console.error err
