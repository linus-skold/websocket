
var socket = new WebSocket("ws://localhost:8080")
var app = new Vue({
    el:'#app',
    data:{
        message: 'Hello Vue!',
        socket: socket,
        items:[
            { message : "hello" }, 
            { message : "world" }, 
            { message : "foo" }, 
            { message : "bar" }, 
            { message : "biz" }, 
            { message : "baz" } 
        ]
    }
})
