`import DS from 'ember-data'`

Project = DS.Model.extend
  name: DS.attr 'string'
  owner: DS.attr 'string'
  goal: DS.attr 'number'
  image: DS.attr 'string'
  teaser: DS.attr 'string'
  backercount: DS.attr 'number'
  pledged: DS.attr 'number'
  details: DS.attr 'string'
  
  imageURL: Ember.computed 'image', ->
    "images/projects/" + @get('image')

`export default Project`
