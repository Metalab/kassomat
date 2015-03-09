module.exports = function(app) {
  var express = require('express');
  var actionsRouter = express.Router();

  actionsRouter.post('/return', function(req, res) {
      console.log("Return money!");
      res.send({
          'status': 'ok'
      });
  });
  actionsRouter.post('/redeem', function(req, res) {
      console.log("Redeem money!");
      res.send({
          'status': 'ok'
      });
  });
  actionsRouter.post('/donate', function(req, res) {
      console.log("Donate money!");
      res.send({
          'status': 'ok'
      });
  });

  app.use('/api/actions', actionsRouter);
};
