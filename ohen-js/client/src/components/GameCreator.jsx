import React, { useState } from "react";
import { useContext } from "react";
import send from "../functions/send";
import { GameContext } from "./GameContext";

function GameCreator() {
    const [gameName, setGameName] = useState("");
    
    const { socket } = useContext(GameContext);

    function createGame() {
        send(socket, "game__create", { name: gameName});
    }

    return(
        <div>
            <label>Game name</label>
            <input
                value={gameName}
                onChange={
                (e) => setGameName(e.target.value) 
                }
            />
            <button
                onClick={createGame}
            >
                Validate
            </button>
        </div>
    )
}

export default GameCreator;