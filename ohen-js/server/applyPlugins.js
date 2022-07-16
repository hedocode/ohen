const ohen = require('./plugins/ohen');

function applyPlugins(fastify) {
    fastify.register(require('@fastify/websocket'));
    fastify.register(ohen);
}

module.exports = applyPlugins;