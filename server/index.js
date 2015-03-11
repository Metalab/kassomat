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
    var denominationLevels = [
      { id: 20,  count: 100 },
      { id: 50,  count: 65 },
      { id: 100, count: 0 },
      { id: 200, count: 12 }
    ];

    var io = require('socket.io')(options.httpServer);
    io.on('connection', function(socket){
        console.log("Connect!");
        socket.emit('userinfo', userinfo);
        socket.emit('pushData', {
          type: 'DenominationLevel',
          payload: denominationLevels
        });

        socket.on('action', function(data, callback) {
            actions[data.name](data.options, userinfo, callback);
            socket.emit('userinfo', userinfo);
        });
        socket.on('disconnect', function(){
            console.log("Disconnect!")
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

    // Log proxy requests
    var morgan  = require('morgan');
    app.use(morgan('dev'));

    mocks.forEach(function(route) { route(app); });
    proxies.forEach(function(route) { route(app); });
};
