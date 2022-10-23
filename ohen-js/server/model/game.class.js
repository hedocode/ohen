const Player = require("./player.class");

class Game {
    constructor (
        name,
        password,
        maxPlayers = 4,
        ticksPerSeconds = 10
    ) {
        this.players = [];
        this.maxPlayers = maxPlayers;
        this.ticksPerSeconds = ticksPerSeconds;
        this.name = name;
        this.password = password;
        this.status = "waiting";
    }

    addPlayer(nickname, ip) {
        this.players.push(
            new Player(nickname, ip)
        );
    }

    toggleReady(connection) {
        const matchingPlayer = this.getPlayerFromConnection(connection);
        matchingPlayer.ready = !matchingPlayer.ready;
        this.sendPlayersDataToEveryone();
        if(
            this.players.filter(
                p => p.ready === true
            ).length === this.players.length
        ) {
            this.start();
        }
    }

    start() {
        this.step();
    }

    playerWon() {
        const playerIsAlive = p => !p.dead()
        const playerReachedMaxOhen = p => p.ohen >= p.ohen_max;

        const alivePlayers = this.players.filter(playerIsAlive);
        const lastAlive = alivePlayers.length === 1;
        const maxOhenPlayers = this.players.filter(playerReachedMaxOhen);

        if(lastAlive) {
            return alivePlayers
        }
        if(maxOhenPlayers.length > 0) {
            return maxOhenPlayers
        }

        return [];
    }
    
    getPlayerFromConnection(connection) {
        return this.players.find(
            p => p.nickname === connection.nickname
        );
    }
    
    getPlayersConnections(fastify) {
        return Array.from(fastify.websocketServer.clients).map(
            (connection) => {
                const found = this.getPlayerFromConnection(connection);
                return found ? connection : null;
            }
        );
    }

    sendToAllPlayers(fastify, type, data) {
        this.getPlayersConnections(fastify).forEach(
            (connection) => {
                fastify.sendData(
                    connection,
                    type,
                    data
                );
            }
        )
    }

    sendPlayers(fastify, connection) {
        fastify.sendData(
            connection,
            "players",
            this.players
        );
    }

    sendPlayersDataToEveryone(fastify) {
        this.getPlayersConnections(fastify).forEach(
            (connection) => {
                this.sendPlayers(fastify, connection);
            }
        )
    }


    step() {
        const winner = playerWon();
        
        if(winner.length == 0){
            players.forEach(
                (player) => {
                    if(player.action === "ohen") {
                        player.ohen += player.ohen_regen / this.ticksPerSeconds;
                    }

                    if(player.action === "attack") {
                        const target = players.find(
                            p => p.nickname == player.target
                        );
                        
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
}

module.exports = Game;