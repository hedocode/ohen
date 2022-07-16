const Player = require("../model/player.class");
const fp = require("fastify-plugin");

async function ohen(fastify) {

    const players = [];
    const tickPerSeconds = 10;

    function addPlayer(nickname, ip) {
        players.push(
            new Player(nickname, ip)
        );
        
        sendPlayersDataToEveryone();
    }

    function sendObject(connection, object) {
        (connection.socket || connection).send(
            JSON.stringify(object)
        );
    }

    function sendToAllPlayers(type, object) {
        fastify.websocketServer.clients.forEach(
            (connection) => {
                sendObject(
                    connection, {
                        "type": type,
                        "data": object
                    }
                );
            }
        )
    }

    function sendPlayersDataToEveryone() {
        fastify.websocketServer.clients.forEach(
            (connection) => {
                sendPlayers(connection);
            }
        )
    }

    function sendPlayers(connection) {
        sendObject(
            connection, {
                "type": "players",
                "data": players
            }
        );
    }

    function getPlayerFromConnection(connection) {
        return players.find(p => p.nickname === connection.socket.nickname);
    }

    function playerWon() {
        const alivePlayers = players.filter(p => !p.dead());
        const lastAlive = alivePlayers.length === 1;
        const maxOhenPlayers = players.filter(p => p.ohen >= p.ohen_max);

        if(lastAlive) {
            return alivePlayers
        }
        if(maxOhenPlayers.length > 0) {
            return maxOhenPlayers
        }

        return [];
    }

    function step() {
        const winner = playerWon();
        
        if(winner.length == 0){
            players.forEach(
                (player) => {
                    if(player.action === "ohen") {
                        player.ohen += player.ohen_regen / tickPerSeconds;
                    }

                    if(player.action === "attack") {
                        const target = players.find(
                            p => p.nickname == player.target
                        );

                        // console.log("target : %o", target);
                        
                        let damage;
                        if(target.action === "defence") {
                            damage = (player.attack - target.defence) / tickPerSeconds;
                        } else {
                            damage = player.attack / tickPerSeconds;
                        }

                        if(target.life - damage < 0) {
                            target.life = 0;
                        } else {
                            target.life -= damage
                        }
                    }
                }
            )
            
            sendPlayersDataToEveryone();
            setTimeout(
                step,
                1000 / tickPerSeconds
            );
        } else {
            sendToAllPlayers("winner", winner);
        }
    }

    function startGame() {
        step()
    }

    fastify.get(
        '/',
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
                        addPlayer(data.nickname, req.socket.remoteAddress);
                        sendObject(
                            connection,
                            {
                                "type": "message",
                                "data": `Hello ${data.nickname}`
                            }
                        );
                        sendPlayers(connection)
                    }
                    
                    if(type === "ready") {
                        getPlayerFromConnection(connection).ready = true;
                        sendPlayersDataToEveryone();
                        if(
                            players.filter(
                                p => p.ready === true
                            ).length === players.length
                        ) {
                            startGame();
                        }
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