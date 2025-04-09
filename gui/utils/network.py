import re
from dataclasses import dataclass, field


def get_available_port() -> int:
    """
    Get an available port on the local machine.

    Returns:
        int: An available port number.
    """
    import socket

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind(("", 0))
        return s.getsockname()[1]


@dataclass
class Socket:
    """
    ### A class to represent a network socket.

    #### Attributes:
    - `ip (str)`: The IP address of the socket.
    - `port (int)`: The port of the socket

    #### Methods:
    - `validate_socket_string(socket_str: str) -> tuple[str, int] | None`: Validate if the string is in the `ip:port` format.
    - `from_string(socket_str: str) -> Socket`: Create a `Socket` object from a string.
    - `update_from_string(socket_str: str) -> None`: Update the `Socket` object from a string.
    """

    ip: str = "0.0.0.0"
    port: int = 0

    @staticmethod
    def validate_socket_string(socket_str: str) -> tuple[str, int] | None:
        """
        Validate if the string is in the `ip:port` format.

        Args:
            socket_str (str): The string to validate.

        Returns:
            tuple[str, int] | None: The IP address and port if the string is valid, else `None`.
        """
        pattern = re.compile(r"^(?:[0-9]{1,3}\.){3}[0-9]{1,3}:[0-9]{1,5}$")
        if not pattern.match(socket_str):
            return None

        ip, port = socket_str.split(":")
        port = int(port)
        if not 0 <= port <= 65535 or not all(
            0 <= int(num) <= 255 for num in ip.split(".")
        ):
            return None

        return ip, port

    @classmethod
    def from_string(cls, socket_str: str) -> "Socket | None":
        """
        Create a `Socket` object from a string.

        Args:
            socket_str (str): The string to create the `Socket` object from. Expects string in format `ip:port`.

        Returns:
            Socket | None: The `Socket` object created from the string, else `None`.
        """
        socket = cls.validate_socket_string(socket_str)
        return cls(socket[0], socket[1]) if socket else None

    def update_from_string(self, socket_str: str) -> bool:
        """
        Update the `Socket` object from a string.

        Args:
            socket_str (str): The string to update the `Socket` object from. Expects string in format `ip:port`.

        Returns:
            bool: `True` if the string is valid and the `Socket` object is updated, else `False`.
        """
        socket = self.validate_socket_string(socket_str)
        if not socket:
            return False

        self.ip, self.port = socket
        return True

    def __str__(self) -> str:
        return f"{self.ip}:{self.port}"


@dataclass
class Network:
    """
    ### A class to represent a network.

    #### Attributes:
    - `local_port (int)`: The local port of the network.
    - `public_socket (Socket)`: The public socket of the network.

    #### Methods:
    - `from_string(local_port: int, public_socket_str: str) -> Network | None`: Create a `Network` object from a string.
    - `from_network(network: Network) -> Network`: Create a `Network` object from another `Network` object.
    - `copy() -> Network`: Clone the `Network` object.
    """

    local_port: int
    public_socket: Socket = field(default_factory=Socket)

    @classmethod
    def from_string(cls, local_port: int, public_socket_str: str) -> "Network | None":
        """
        Create a `Network` object from a string.

        Args:
            local_port (int): The local port of the network.
            public_socket_str (str): The public socket string in format `ip:port`.

        Returns:
            Network: The `Network` object created from the string, else `None`.
        """
        public_socket = Socket.from_string(public_socket_str)
        return cls(local_port, public_socket) if public_socket else None

    @classmethod
    def from_network(cls, network: "Network") -> "Network":
        """
        Create a `Network` object from another `Network` object.

        Args:
            network (Network): The `Network` object to create the new object from.

        Returns:
            Network: The new `Network` object.
        """
        return cls(network.local_port, network.public_socket)

    def copy(self) -> "Network":
        """
        Clone the `Network` object.

        Returns:
            Network: The cloned `Network` object.
        """
        return self.from_network(self)

    def __str__(self) -> str:
        return f"Local Port: {self.local_port}, Public Socket: {self.public_socket}"
