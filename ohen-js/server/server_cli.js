
const applyPlugins = require("./applyPlugins");
const serverOptions = require("./serverOptions");

module.exports = async function (fastify, option) {
    await applyPlugins(fastify);
};


module.exports.options = serverOptions;