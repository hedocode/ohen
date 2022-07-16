require("dotenv").config();

const {
    FASTIFY_ADDRESS,
    FASTIFY_PORT
} = process.env;

module.exports = {
    // https://fr.wikipedia.org/wiki/Hypertext_Transfer_Protocol/2
    
    port: FASTIFY_PORT,
    host: FASTIFY_ADDRESS,
    
    http2: false, // works only over secure https connexion
    https: null,

    // Configuration securisée
    // https: {
    //     allowHTTP1: true, // fallback support for HTTP1
    //     key: fs.readFileSync(path.join(__dirname, '..', 'https', 'fastify.key')),
    //     cert: fs.readFileSync(path.join(__dirname, '..', 'https', 'fastify.cert'))
    // }

    // Le temps maximal d'execution d'une requète
    connectionTimeout: 0,

    // Durée pendant laquelle on peut réutiliser un socket avant qu'il ne se ferme
    keepAliveTimeout: 5000,

    // /route == /route/
    ignoreTrailingSlash: true,

    // Longueur maximale d'un paramètre d'une requète
    maxParamLength: 100,

    // Taille maximale des données d'une requète
    bodyLimit: 1047576,

    // 'error' 'remove' or 'ignore' 
    onProtoPoisoning: "error",

    // https://medium.com/intrinsic/javascript-prototype-poisoning-vulnerabilities-in-the-wild-7bc15347c96
    onConstructorPoisoning: "ignore",

    // https://github.com/pinojs/pino/blob/master/docs/api.md#options-object
    logger: {
        prettyPrint: true,
    },
    // serializers: {
    //     res(reply) {
    //         // The default
    //         return {
    //             statusCode: reply.statusCode
    //         }
    //     },
    //     req(request) {
    //         return {
    //             route : request.method +
    //             request.url +
    //             (request.path ? request.path : "") +
    //             (request.parameters ? request.parameters : ""),
    //             // headers : request.headers;
    //             // Including the headers in the log could be in violation
    //             // of privacy laws, e.g. GDPR. You should use the "redact" option to
    //             // remove sensitive fields. It could also leak authentication data in
    //             // the logs.
    //         };
    //     }
    // }
    // },
    

    // https://www.fastify.io/docs/latest/Server/#disablerequestlogging
    disableRequestLogging: true,


    caseSensitive: true,
};