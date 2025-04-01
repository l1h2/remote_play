from utils import Network, Subprocess

from .base_subprocess_worker import SubprocessNetWorker


class StunQueryWorker(SubprocessNetWorker):
    """
    ### Worker that calls a subprocess to query the STUN server.

    #### Inherits from:
    - `SubprocessNetWorker`
    """

    def __init__(self, network: Network, exe_path: str = Subprocess.STUN_CLIENT):
        super().__init__(network, exe_path)
        self.set_args([str(self.network.local_port)])

    def _handle_process_output(self, process_output: str) -> None:
        """
        Handle the output from the process.

        Args:
            process_output (str): The output from the process.
        """
        if self.network.public_socket.update_from_string(process_output):
            self.output.emit(f"{self.network.public_socket}")


class UdpPeerWorker(SubprocessNetWorker):
    """
    ### Worker that calls a subprocess to establish a UDP connection.

    #### Inherits from:
    - `SubprocessNetWorker`
    """

    def __init__(self, network: Network, exe_path: str = Subprocess.UDP_CONNECTION):
        super().__init__(network, exe_path)
        self.set_args(
            ["-p", str(self.network.local_port), str(self.network.public_socket)]
        )

    def _handle_process_output(self, process_output: str) -> None:
        """
        Handle the output from the process.

        Args:
            process_output (str): The output from the process.
        """
        self.output.emit(process_output)


class UdpServerWorker(SubprocessNetWorker):
    """
    ### Worker that calls a subprocess to establish a UDP server.

    #### Inherits from:
    - `SubprocessNetWorker`
    """

    def __init__(self, network: Network, exe_path: str = Subprocess.UDP_SERVER):
        super().__init__(network, exe_path)
        self.set_args(
            ["-p", str(self.network.local_port), str(self.network.public_socket)]
        )

    def _handle_process_output(self, process_output: str) -> None:
        """
        Handle the output from the process.

        Args:
            process_output (str): The output from the process.
        """
        self.output.emit(process_output)


class UdpClientWorker(SubprocessNetWorker):
    """
    ### Worker that calls a subprocess to establish a UDP client.

    #### Inherits from:
    - `SubprocessNetWorker`
    """

    def __init__(self, network: Network, exe_path: str = Subprocess.UDP_CLIENT):
        super().__init__(network, exe_path)
        self.set_args(
            ["-p", str(self.network.local_port), str(self.network.public_socket)]
        )

    def _handle_process_output(self, process_output: str) -> None:
        """
        Handle the output from the process.

        Args:
            process_output (str): The output from the process.
        """
        self.output.emit(process_output)
