
var socket = new WebSocket("ws://localhost:8091")

Vue.component('connection-status', {
    data:function() {
        return {
            connection_status:'no connection'
        }
    },
    template: '<h2>{{connection_status}}</h2>'
})

var app = new Vue({
    el:'#app',
    data:{
        message: 'Hello Vue!',
        socket: socket,
        connection_status: 'No Connection',
        items:[
            { message : "hello" }, 
            { message : "world" }, 
            { message : "foo" }, 
            { message : "bar" }, 
            { message : "biz" }, 
            { message : "baz" } 
        ]
    },
    template: '<h2>{{connection_status}}</h2>'
})


/* This component stuff is pretty dope */
//data must be a function in a component
Vue.component('button-counter', {
    data: function() {
        return {
            count: 0
        }
    },
    template: '<button v-on:click="count++">You clicked me {{ count }} times.</button>'
})

new Vue({ el : '#demo' })
new Vue({ el : '#connection-status' })

socket.onopen = function() {
    app.connection_status = "Connected"
}

socket.onclose = function(event) {
    app.connection_status = "Closed"
    console.log(event)
}

socket.onerror = function() {
    app.connection_status = "There was an error"
}