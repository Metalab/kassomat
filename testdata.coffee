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

db.hmset 'entity:Project:1', {
	name: 'Kassomat'
	image: 'kassomat'
	teaser: 'Eine selbstgebaute Kassa mit Touchinterface, die hauptsächlich als Ersatz zur offenen Kassa im Einsatz sein soll, die verhindern soll dass nicht einfach so mehrere Geldscheine aus der Kassa verschwinden. Zusätzlich eröffnen sich angenehme Funktionalität wie zB: iButton Credits, automatische Buchhaltung, Lazzzor Jobs erfassen, Barcode Scanner,.. whatnot'
}, (err) ->
	if err
		console.error err

db.hmset 'entity:Project:2', {
	name: 'Internet'
	image: 'internet'
	teaser: 'The Internet is a global system of interconnected computer networks that use the standard Internet protocol suite (TCP/IP) to link several billion devices worldwide. It is a network of networks that consists of millions of private, public, academic, business, and government networks of local to global scope, linked by a broad array of electronic, wireless, and optical networking technologies. The Internet carries an extensive range of information resources and services, such as the inter-linked hypertext documents and applications of the World Wide Web (WWW), the infrastructure to support email, and peer-to-peer networks for file sharing and telephony.'
}, (err) ->
	if err
		console.error err

db.hmset 'entity:Project:3', {
	name: 'UTF-8 Testing'
	image: 'internet'
	teaser: '日本語😩'
}, (err) ->
	if err
		console.error err
