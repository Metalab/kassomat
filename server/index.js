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

    var express = require('express');
    var bodyParser = require('body-parser');
    var backsideRouter = express.Router();
    app.use(bodyParser.json());

    // Log proxy requests
    var morgan  = require('morgan');
    app.use(morgan('dev'));

    mocks.forEach(function(route) { route(app); });
    proxies.forEach(function(route) { route(app); });
};
