class Player {
    constructor(nickname, ip) {
        this.nickname = nickname;

        this.ohen_max = 10;
        this.ohen = 0;
        this.ohen_regen = 1;

        this.attack = 2;
        this.defence = 1;

        this.life_max = 10;
        this.life = 10;
        this.life_regen = 1;

        this.dead = () => this.life <= 0;
        this.ready = false;

        this.action = "ohen";
        this.target = "self";
        this.game_status = "waiting";// playing
    }
}

module.exports = Player;