`import DS from 'ember-data'`

Userinfo = DS.Model.extend
    username: DS.attr 'string'
    credits: DS.attr 'number'

`export default Userinfo`
