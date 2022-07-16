import React, { Fragment, useEffect, useState } from "react";


function App() {
 const [socket, setSocket] = useState(null);
  

  const [message, setMessage] = useState("");
  const [serverAdress, setServerAdress] = useState("");
  const [players, setPlayers] = useState([]);
  const [nickname, setNickName] = useState("");
  const [winner, setWinner] = useState(false);
  
  useEffect(
    () => {
      if(socket) {
        socket.addEventListener(
          "open",
          () => {
            // envoi d'un message au serveur
            socket.send(
              JSON.stringify(
                {
                  type: "connect",
                  data: {
                    nickname: nickname
                  }
                }
              )
            );
          }
        );
        
        // réception d'un message envoyé par le serveur
        socket.addEventListener(
          "message",
          ({ data }) => {
            const { type, data: d } = JSON.parse(data);

            if(type === "message") {
              setMessage(d);
            }

            if(type === "players") {
              setPlayers(d);
            }

            if(type === "winner") {
              setWinner(d);
            } 
          
            // switch (packet.type) {
            //   case "bonjour du serveur":
            //     // ...
            //     break;
            //   default:
            //     break;
            // }
          }
        );
      }
    }, [socket]
  )

  function connectToServer() {
    setSocket(
      new WebSocket("ws://" + serverAdress)
    )
  }

  function send(type, data) {
    socket.send(
      JSON.stringify(
        {
          type: type,
          data: data,
        }
      )
    )
  }

  function ready() {
    send("ready");
  }

  function attack(player) {
    send(
      "action",
      {
        action: "attack",
        target: player.nickname
      }
    );
  }

  function defend() {
    send("action", {action: "defence"})
  }

  function ohen() {
    send("action", {action: "ohen"})
  }

  return (
    <div>
      {!socket ? (
        <Fragment>
          <div>
            <label>Server :</label>
            <input value={serverAdress} onChange={ (e) => setServerAdress(e.target.value)}/>
          </div>
          <div>
            <label>Nickname :</label>
            <input value={nickname} onChange={ (e) => setNickName(e.target.value)}/>
          </div>
          <button
            onClick={connectToServer}
          >
            Connect
          </button>
        </Fragment>
      ) : (
        <div>
          Server : {message}

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
            onClick={ready}
          >
            Ready
          </button>

        </div>
      )}
    </div>
  );
}

export default App;
