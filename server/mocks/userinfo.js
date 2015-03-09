module.exports = function(app) {
  var express = require('express');
  var userinfoRouter = express.Router();

  var userinfo = {
      id: 0,
      username: "anlumo",
      credits: 150
  };

  userinfoRouter.get('/', function(req, res) {
    res.send({
      'userinfos': [ userinfo ]
    });
  });

  userinfoRouter.post('/', function(req, res) {
    res.status(201).end();
  });

  userinfoRouter.get('/:id', function(req, res) {
    res.send({
      'userinfo': userinfo
    });
  });

  app.use('/api/userinfos', userinfoRouter);
};
