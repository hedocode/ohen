const ohen = require('./plugins/ohen');
const websockets = require('./plugins/websockets');
const path = require('path');

function applyPlugins(fastify) {
    fastify.register(
        require('@fastify/static'), {
            root: path.join(__dirname, '../client/build'),
            // prefix: '/public/', // optional: default '/'
        }
    );
    fastify.register(require('@fastify/websocket'));
    fastify.register(websockets);
    fastify.after(
        () => {
            fastify.register(ohen);
        }
    );
    fastify.get(
        '/',
        function (req, reply) {
            reply.sendFile('index.html');
        }
    )
}

module.exports = applyPlugins;