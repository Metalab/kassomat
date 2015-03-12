module.exports = function(app, options) {
    var globSync   = require('glob').sync;
    var mocks      = globSync('./mocks/**/*.js', { cwd: __dirname }).map(require);
    var proxies    = globSync('./proxies/**/*.js', { cwd: __dirname }).map(require);
    var actions    = require('./actions');
    var database   = {};
    var databaseFiles = globSync('./database/*.js', {cwd: __dirname});

    var filepattern = /^.*\/([^/]+)\.js$/;

    for(var i = 0; i < databaseFiles.length; ++i) {
        var name = filepattern.exec(databaseFiles[i]);
        database[name[1]] = require(databaseFiles[i]);
    }

    var userinfo = {username: "anlumo", credits: 380};

    var io = require('socket.io')(options.httpServer);
    io.on('connection', function(socket){
        console.log("Connect!");
        socket.emit('userinfo', userinfo);
        socket.emit('pushData', {
          type: 'DenominationLevel',
          payload: database['denominationLevel']
        });

        socket.on('action', function(data, callback) {
            actions[data.name](data.options, userinfo, callback);
            io.emit('userinfo', userinfo);
        });
        socket.on('disconnect', function(){
            console.log("Disconnect!")
        });

        socket.on('buy', function(data, callback) {
          var value = 0;
          switch(data.name) {
            case 'premium':
              value = 300;
              break;
            case 'mate':
              value = 200;
              break;
            case 'pop':
              value = 150;
              break;
            case 'water':
              value = 100;
              break;
            case 'coffee':
              value = 50;
              break;
          }
          userinfo.credits -= value;
          if(callback) {
            callback({
              status: 1
            });
          }
          io.emit('userinfo', userinfo);
        });

        socket.on('findAll', function(data, callback) {
            console.log("[Adapter] | findall: ", data);
            callback({
                status: 1,
                content: database[data.type]
            });
        });
        socket.on('find', function(data, callback) {
            console.log("[Adapter] | find: ", data);
            var objects = database[data.type];

            for(var i = 0; i < objects.length; ++i) {
                var object = objects[i];
                if(object.id == data.query.id) {
                  if(callback)
                    callback({
                        status: 1,
                        content: object
                    });
                }
            }
        });
    });

    var express = require('express');
    var bodyParser = require('body-parser');
    var backsideRouter = express.Router();
    app.use(bodyParser.json());

    backsideRouter.post('/userinfo', function(req, res) {
      userinfo = req.body;
      io.emit('userinfo', userinfo);
      res.send({
        status: 1
      });
    });

    app.use('/api', backsideRouter);


    // Log proxy requests
    var morgan  = require('morgan');
    app.use(morgan('dev'));

    mocks.forEach(function(route) { route(app); });
    proxies.forEach(function(route) { route(app); });
};
