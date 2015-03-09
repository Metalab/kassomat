`import DS from 'ember-data'`

Project = DS.Model.extend
    name: DS.attr 'string'
    image: DS.attr 'string'
    teaser: DS.attr 'string'

    active: DS.attr 'boolean',
        defaultValue: false
        transient: true

    imageURL: (->
        "images/projects/" + @get('image')
    ).property('image')

`export default Project`
