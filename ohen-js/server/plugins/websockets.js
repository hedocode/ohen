const fp = require("fastify-plugin");

async function websockets(fastify) {
    fastify.decorate(
        "sendData", 
        function (connection, type, data) {
            (connection.socket || connection).send(
                JSON.stringify({
                    "type": type,
                    "data": data,
                })
            );
        }
    )
}
module.exports = fp(websockets);