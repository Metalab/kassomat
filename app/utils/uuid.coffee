uuid = ->
    d = new Date().getTime()
    'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace /[xy]/g, (c) ->
        r = (d + Math.random()*16)%16 | 0
        d = Math.floor(d/16)
        (if c=='x' then r else (r&0x3|0x8)).toString(16)

`export default uuid`
