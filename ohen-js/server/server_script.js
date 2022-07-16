const fastify = require("fastify")();
const applyPlugins = require("./applyPlugins");
const serverOptions = require("./serverOptions");

applyPlugins(fastify);

fastify.listen(
    serverOptions,
).then(
    (res) => {
        console.log("Listening on : %o", res);
    }
);