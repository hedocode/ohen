const fp = require("fastify-plugin");
const Game = require("../model/game.class");

async function ohen(fastify) {

    const games = [];
    const users = [];


    function addPlayer(game, nickname, ip) {
        game.addPlayer(nickname, ip);   
        game.sendPlayersDataToEveryone(fastify);
    }

    function createGame(name, password, maxPlayers, tickPerSeconds) {
        games.push(
            new Game(name, password, maxPlayers, tickPerSeconds)
        );
    }

    function sendGames(connection) {
        fastify.sendData(
            connection,
            "games",
            games
        )
    }

    function sendUsers(connection) {
        fastify.sendData(
            connection,
            "users",
            users
        )
    }

    fastify.get(
        '/game',
        { websocket: true },
        (
            connection /* SocketStream */,
            req /* FastifyRequest */
        ) => {
            connection.socket.on(
                'message',
                message => {
                    const { type, data } = JSON.parse(message);

                    if(type === "connect") {
                        connection.socket.nickname = data.nickname;
                        users.push(data.nickname);
                        fastify.sendData(
                            connection,
                            "message",
                            `Hello ${data.nickname}`
                        );
                        sendUsers(connection);
                        sendGames(connection);
                    }

                    if(type === "game__create") {
                        const { name, password, maxPlayers, tickPerSeconds } = data;
                        createGame(name, password, maxPlayers, tickPerSeconds);
                        sendGames(connection);
                    }

                    if(type === "game__join") {
                        const { nickname, gameName } = data;
                        addPlayer(
                            games.find(
                                (g) => g.name = gameName
                            ),
                            nickname
                        )
                    }

                    if(type === "game__leave") {

                        sendGames(connection);
                    }
                   
                    
                    if(type === "toggleReady") {
                        const targetGame = games.find(g => g.name == data.game_name);
                        targetGame.toggleReady(connection);
                    }

                    if(type === "action") {
                        const user = getPlayerFromConnection(connection);
                        if(data.action === "attack") {
                            user.action = "attack";
                            user.target = data.target;
                        } else {
                            user.action = data.action
                        }
                    }
                }
            )
        }
    )
}

module.exports = fp(ohen);