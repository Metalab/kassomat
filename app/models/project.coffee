`import DS from 'ember-data'`

Project = DS.Model.extend
    name: DS.attr 'string'
    image: DS.attr 'string'
    teaser: DS.attr 'string'

    active: DS.attr 'boolean',
        defaultValue: false
        transient: true

    imageURL: Ember.computed 'image', ->
        "images/projects/" + @get('image')

`export default Project`
