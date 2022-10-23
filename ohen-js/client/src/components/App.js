import React, { Fragment, useEffect, useState } from "react";
import send from "../functions/send";
import Connexion from "./Connexion";
import { GameContext } from "./GameContext";
import GameCreator from "./GameCreator";

function App({
  server = ""
}) {
  const [socket, setSocket] = useState(null);

  const [message, setMessage] = useState("");
  const [games, setGames] = useState([]);
  const [users, setUsers] = useState([]);

  const [nickname, setNickName] = useState("");

  // Game Info
  const [players, setPlayers] = useState([]);
  const [winner, setWinner] = useState(false);

  const allData = {
    server,
    socket, setSocket,
    message, setMessage,
    games, setGames,
    players, setPlayers,
    nickname, setNickName,
    winner, setWinner,
    users, setUsers,
  };
  
  useEffect(
    () => {
      function onConnect() {
        send(
          socket,
          "connect",
          {
                nickname: nickname
          }
        );
      }
      
      if(socket) {
        socket.removeEventListener("open", onConnect);
        socket.addEventListener("open", onConnect);
      }


      function onMessage({ data }) {
        const { type, data: d } = JSON.parse(data);

        const messageSetter = {
          "message": setMessage,
          "players": setPlayers,
          "games": setGames,
          "users": setUsers,
          "winner": setWinner,
        };

        messageSetter[type](d);
      }

      if(socket) {
        // réception d'un message envoyé par le serveur
        socket.removeEventListener("message", onMessage);
        socket.addEventListener("message", onMessage);
      }
    }, [socket]
  )

  function joinGame(gameName) {
    send(
      socket,
      "game__join", {
        gameName: gameName,
        nickname: nickname
      }
    );
  }

  function toggleReady() {
    send(socket, "toggleReady");
  }

  function attack(player) {
    send(
      socket,
      "action",
      {
        action: "attack",
        target: player.nickname
      }
    );
  }

  function defend() {
    send(socket, "action", {action: "defence"})
  }

  function ohen() {
    send(socket, "action", {action: "ohen"})
  }


  console.log("games : %o", games);
  console.log("users : %o", users);

  return (
    <GameContext.Provider
      value={allData}
    >
      {!socket ? (
        <Connexion
          server={server}
          setSocket={setSocket}
          nickname={nickname}
          setNickName={setNickName}
        />
      ) : (
        <div>
          <span>Server : {message}</span>

          <span>Open Games :</span>
          {games.map(
            (game) => (
              <article>
                <div>{game.name}</div>
                <button
                  onClick={() => joinGame(game.name)}
                >
                  Join
                </button>
              </article>
            )
          )}
          <GameCreator/>

          <span>Connected Users :</span>
          {users.map(
            (user) => (
              <article>
                <div>{ user }</div>
              </article>
            )
          )}


          <div>
            Players : 
            <ul>
              { players && (
                players.map(
                  (player) => {
                    const attacker = players.find(p => p.target === player.nickname && p.action === "attack");

                    const timeToLose = ~~(attacker && player.life / attacker.attack);

                    const timeToWin = ~~(player.ohen_max - player.ohen) / player.ohen_regen;

                    return (
                      <li>
                        <span>
                          {player.nickname}
                          {player.life <= 0 && "☠️"}
                          {!!winner ? (
                            winner.find(w => w.nickname === player.nickname) && "🏆"
                          ) : (
                            <Fragment>
                              {players.find(p => !p.ready) && (player.ready ? "✅" : "⌚")}
                              {player.nickname === nickname ?
                                <div>
                                  (You)
                                  <button onClick={defend}>Defend</button>
                                  <button onClick={ohen}>Ohen</button>
                                </div>
                              :
                                <button
                                  onClick={() => attack(player)}
                                >
                                  Attack
                                </button>
                              }
                            </Fragment>
                          )}
                          {player.action === "attack" ? (
                            "⚔️"
                          ) : player.action === "defence" ?(
                            "🛡️"
                          ) : (
                            "⚡"
                          )}
                        </span>
                        <div>
                          <label>Ohen</label>
                          <progress min={0} value={player.ohen} max={player.ohen_max}/>
                        </div>
                        {player.action === "ohen" && (
                          <div>
                            <label>Time to win : </label>
                            <span>
                              {timeToWin}s
                            </span>
                          </div>
                        )}
                        <div>
                          <label>Life</label>
                          <progress min={0} value={player.life} max={player.life_max}/>
                        </div>
                        {attacker && (
                          <div>
                            <label>Time to lose : </label>
                            <span>
                              {timeToLose}s
                            </span>
                          </div>
                        )}
                        <div>
                          <label>Attack : </label>
                          <span>{player.attack}</span>
                        </div>
                        <div>
                          <label>Defence : </label>
                          <span>{player.defence}</span>
                        </div>
                      </li>
                    )
                  }
                )
              )}
            </ul>
          </div>

          <button
            onClick={toggleReady}
          >
            Ready
          </button>

        </div>
      )}
    </GameContext.Provider>
  );
}

export default App;
