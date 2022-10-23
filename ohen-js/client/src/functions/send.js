function send(socket, type, data) {
    socket.send(
      JSON.stringify(
        {
          type: type,
          data: data,
        }
      )
    )
}

export default send;