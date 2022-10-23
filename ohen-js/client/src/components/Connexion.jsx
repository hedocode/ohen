import React, { Fragment, useState, useEffect } from "react"
import { useContext } from "react";
import { GameContext } from "./GameContext";

function Connexion() {
    const {
        server,
        nickname,
        setNickName,
        setSocket,
    } = useContext(GameContext);

    const [serverAdress, setServerAdress] = useState(server);

    return (

        <Fragment>
            {!server && (
                <div>
                    <label>Server :</label>
                    <input
                        value={serverAdress}
                        onChange={
                            (e) => setServerAdress(e.target.value)
                        }
                    />
                </div>
            )}
            <div>
                <label>Nickname :</label>
                <input value={nickname} onChange={ (e) => setNickName(e.target.value)}/>
            </div>
            <button
                onClick={
                    () => {
                        setSocket(
                            new WebSocket("wss://" + serverAdress)
                        )
                    }
                }
            >
                Connect
            </button>
        </Fragment>
    );
}

export default Connexion;